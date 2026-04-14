#include "smtp_mailer.h"
#include "config_manager.h"

#include <QByteArray>
#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QObject>
#include <QSslSocket>
#include <QRegularExpression>
#include <QTextStream>

namespace employes {

bool SmtpConfig::isComplete() const
{
    return !host.trimmed().isEmpty()
        && !port.trimmed().isEmpty()
        && !user.trimmed().isEmpty()
        && !password.trimmed().isEmpty()
        && !from.trimmed().isEmpty();
}

QString SmtpConfig::missingDescription() const
{
    QStringList missing;
    if (host.trimmed().isEmpty()) missing << "VS_SMTP_HOST";
    if (port.trimmed().isEmpty()) missing << "VS_SMTP_PORT";
    if (user.trimmed().isEmpty()) missing << "VS_SMTP_USER";
    if (password.trimmed().isEmpty()) missing << "VS_SMTP_PASS";
    if (from.trimmed().isEmpty()) missing << "VS_SMTP_FROM";
    return missing.join(", ");
}

namespace {
QString encodeBase64Header(const QString &value)
{
    return QString("=?UTF-8?B?%1?=").arg(QString::fromLatin1(value.toUtf8().toBase64()));
}

bool writeLine(QSslSocket &socket, const QByteArray &line, QString *errorMessage)
{
    const QByteArray payload = line + "\r\n";
    if (socket.write(payload) != payload.size() || !socket.waitForBytesWritten(10000)) {
        if (errorMessage) {
            *errorMessage = QObject::tr("Connexion SMTP échouée");
        }
        return false;
    }
    qDebug().noquote() << "[SMTP >>]" << line;
    return true;
}

struct SmtpResponse {
    int code = -1;
    QString text;
};

SmtpResponse readResponse(QSslSocket &socket, QString *errorMessage)
{
    SmtpResponse response;
    QStringList lines;

    while (true) {
        if (!socket.canReadLine() && !socket.waitForReadyRead(10000)) {
            if (errorMessage) {
                *errorMessage = QObject::tr("Connexion SMTP échouée");
            }
            return response;
        }

        while (socket.canReadLine()) {
            const QByteArray rawLine = socket.readLine();
            const QString line = QString::fromUtf8(rawLine).trimmed();
            if (line.isEmpty()) {
                continue;
            }
            qDebug().noquote() << "[SMTP <<]" << line;
            lines.append(line);

            if (line.size() >= 4 && line.left(3).at(0).isDigit() && line.left(3).at(1).isDigit() && line.left(3).at(2).isDigit()) {
                const QChar separator = line.at(3);
                if (separator == ' ') {
                    response.code = line.left(3).toInt();
                    response.text = lines.join("\n");
                    return response;
                }
            }
        }
    }
}

bool expectCode(QSslSocket &socket,
                int expectedCode,
                const QString &failureMessage,
                QString *errorMessage)
{
    const SmtpResponse response = readResponse(socket, errorMessage);
    if (response.code != expectedCode) {
        if (errorMessage) {
            *errorMessage = failureMessage;
            if (!response.text.isEmpty()) {
                *errorMessage += QString(" (%1)").arg(response.text);
            }
        }
        return false;
    }
    return true;
}

bool expectAnyOf(QSslSocket &socket,
                 const QList<int> &expectedCodes,
                 const QString &failureMessage,
                 QString *errorMessage,
                 int *actualCode = nullptr)
{
    const SmtpResponse response = readResponse(socket, errorMessage);
    if (actualCode) {
        *actualCode = response.code;
    }
    if (!expectedCodes.contains(response.code)) {
        if (errorMessage) {
            *errorMessage = failureMessage;
            if (!response.text.isEmpty()) {
                *errorMessage += QString(" (%1)").arg(response.text);
            }
        }
        return false;
    }
    return true;
}
} // namespace

QString SmtpMailer::readConfigValue(const QString &key)
{
    return ConfigManager::get(key);
}

SmtpConfig SmtpMailer::loadConfig()
{
    SmtpConfig config;
    config.host = readConfigValue("VS_SMTP_HOST");
    config.port = readConfigValue("VS_SMTP_PORT");
    config.user = readConfigValue("VS_SMTP_USER");
    config.password = readConfigValue("VS_SMTP_PASS");
    config.from = readConfigValue("VS_SMTP_FROM");
    return config;
}

bool SmtpMailer::isValidEmail(const QString &email)
{
    // Validation simple: utile pour l'UI avant de tenter l'envoi SMTP.
    static const QRegularExpression regex(
        R"(^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Za-z]{2,}$)"
    );
    return regex.match(email.trimmed()).hasMatch();
}

QByteArray SmtpMailer::wrapBase64(const QByteArray &data, int lineLength)
{
    // Le MIME préfère des lignes courtes; 76 caractères par ligne est une valeur standard.
    QByteArray encoded = data.toBase64();
    QByteArray wrapped;
    wrapped.reserve(encoded.size() + (encoded.size() / lineLength) + 8);

    for (qsizetype i = 0; i < encoded.size(); i += lineLength) {
        wrapped.append(encoded.mid(i, lineLength));
        wrapped.append("\r\n");
    }
    return wrapped;
}

QString SmtpMailer::buildMimeMessage(const QString &from,
                                     const QString &to,
                                     const QString &sujet,
                                     const QString &corps,
                                     const QString &cheminPDF,
                                     QString *errorMessage)
{
    QFile pdfFile(cheminPDF);
    if (!pdfFile.exists()) {
        if (errorMessage) {
            *errorMessage = QObject::tr("PDF introuvable");
        }
        return {};
    }
    if (!pdfFile.open(QIODevice::ReadOnly)) {
        if (errorMessage) {
            *errorMessage = QObject::tr("Impossible d'ouvrir le PDF");
        }
        return {};
    }

    const QByteArray pdfData = pdfFile.readAll();
    const QByteArray pdfB64 = wrapBase64(pdfData);
    const QString fileName = QFileInfo(cheminPDF).fileName();

    // Structure MIME:
    // 1) headers SMTP/mail
    // 2) multipart/mixed: une partie texte + une partie fichier PDF encodée en base64
    // Les en-têtes (headers) décrivent le message pour le serveur SMTP et le client mail.
    // Subject est encodé en base64 RFC 2047 pour préserver les accents français.
    const QString encodedSubject = encodeBase64Header(sujet);

    QString mime;
    mime += "From: " + from + "\r\n";
    mime += "To: " + to + "\r\n";
    mime += "Subject: " + encodedSubject + "\r\n";
    mime += "MIME-Version: 1.0\r\n";
    mime += "Content-Type: multipart/mixed; boundary=boundary_mail_vision_sight\r\n";
    mime += "\r\n";
    mime += "--boundary_mail_vision_sight\r\n";
    mime += "Content-Type: text/plain; charset=UTF-8\r\n";
    mime += "Content-Transfer-Encoding: 8bit\r\n\r\n";
    mime += corps + "\r\n\r\n";
    mime += "--boundary_mail_vision_sight\r\n";
    mime += "Content-Type: application/pdf; name=\"" + fileName + "\"\r\n";
    mime += "Content-Transfer-Encoding: base64\r\n";
    mime += "Content-Disposition: attachment; filename=\"" + fileName + "\"\r\n\r\n";
    mime += QString::fromUtf8(pdfB64);
    mime += "--boundary_mail_vision_sight--\r\n";

    return mime;
}

bool SmtpMailer::envoyerEmailSMTP(const QString &to,
                                  const QString &sujet,
                                  const QString &corps,
                                  const QString &cheminPDF,
                                  QString *errorMessage)
{
    qDebug() << "[SMTP] Debut de l'envoi vers" << to;

    if (!isValidEmail(to)) {
        if (errorMessage) {
            *errorMessage = QObject::tr("Email invalide");
        }
        qWarning() << "[SMTP] Email invalide:" << to;
        return false;
    }

    const SmtpConfig smtp = loadConfig();

    if (!smtp.isComplete()) {
        if (errorMessage) {
            *errorMessage = QObject::tr("Configuration SMTP manquante: %1").arg(smtp.missingDescription());
        }
        qWarning() << "[SMTP] Configuration SMTP manquante:" << smtp.missingDescription();
        return false;
    }

    bool portOk = false;
    const quint16 smtpPort = smtp.port.toUShort(&portOk);
    if (!portOk || smtpPort == 0) {
        if (errorMessage) {
            *errorMessage = QObject::tr("Configuration SMTP manquante: VS_SMTP_PORT invalide");
        }
        qWarning() << "[SMTP] Port SMTP invalide:" << smtp.port;
        return false;
    }

    if (!isValidEmail(smtp.from)) {
        if (errorMessage) {
            *errorMessage = QObject::tr("Configuration SMTP manquante: VS_SMTP_FROM invalide");
        }
        qWarning() << "[SMTP] Adresse FROM invalide:" << smtp.from;
        return false;
    }

    if (!QFileInfo::exists(cheminPDF)) {
        if (errorMessage) {
            *errorMessage = QObject::tr("PDF introuvable");
        }
        qWarning() << "[SMTP] PDF introuvable:" << cheminPDF;
        return false;
    }

    QString mimeError;
    const QString mimeMessage = buildMimeMessage(smtp.from, to, sujet, corps, cheminPDF, &mimeError);
    if (mimeMessage.isEmpty()) {
        if (errorMessage) {
            *errorMessage = mimeError.isEmpty() ? QObject::tr("Erreur MIME") : mimeError;
        }
        qWarning() << "[SMTP] Echec construction MIME:" << mimeError;
        return false;
    }

    // Port 587 = SMTP avec STARTTLS.
    // On parle d'abord SMTP en clair, puis on upgrade la connexion en TLS.
    QSslSocket socket;
    socket.setPeerVerifyMode(QSslSocket::VerifyNone);

    qDebug() << "[SMTP] Connexion au serveur SMTP:" << smtp.host << smtp.port;
    qDebug() << "[SMTP] From:" << smtp.from << "To:" << to;
    qDebug() << "[SMTP] Etape 1/4: connexion reseau au serveur";

    socket.connectToHost(smtp.host, smtpPort);
    if (!socket.waitForConnected(15000)) {
        if (errorMessage) {
            *errorMessage = QObject::tr("Connexion SMTP échouée");
        }
        qWarning() << "[SMTP] Impossible de se connecter:" << socket.errorString();
        return false;
    }

    if (!expectCode(socket, 220, QObject::tr("Connexion SMTP échouée"), errorMessage)) {
        qWarning() << "[SMTP] Reponse initiale invalide";
        return false;
    }

    qDebug() << "[SMTP] Etape 2/4: negotiation STARTTLS";
    if (!writeLine(socket, "EHLO localhost", errorMessage)) {
        return false;
    }
    if (!expectAnyOf(socket, {250}, QObject::tr("Connexion SMTP échouée"), errorMessage)) {
        return false;
    }

    if (!writeLine(socket, "STARTTLS", errorMessage)) {
        return false;
    }
    if (!expectCode(socket, 220, QObject::tr("Connexion SMTP échouée"), errorMessage)) {
        return false;
    }

    socket.startClientEncryption();
    if (!socket.waitForEncrypted(15000)) {
        if (errorMessage) {
            *errorMessage = QObject::tr("Connexion SMTP échouée");
        }
        qWarning() << "[SMTP] Echec handshake TLS:" << socket.errorString();
        return false;
    }

    qDebug() << "[SMTP] Etape 3/4: authentification SMTP";
    if (!writeLine(socket, "EHLO localhost", errorMessage)) {
        return false;
    }
    if (!expectAnyOf(socket, {250}, QObject::tr("Connexion SMTP échouée"), errorMessage)) {
        return false;
    }

    if (!writeLine(socket, "AUTH LOGIN", errorMessage)) {
        return false;
    }
    if (!expectCode(socket, 334, QObject::tr("Erreur d'authentification"), errorMessage)) {
        return false;
    }

    if (!writeLine(socket, smtp.user.toUtf8().toBase64(), errorMessage)) {
        return false;
    }
    if (!expectCode(socket, 334, QObject::tr("Erreur d'authentification"), errorMessage)) {
        return false;
    }

    if (!writeLine(socket, smtp.password.toUtf8().toBase64(), errorMessage)) {
        return false;
    }
    if (!expectAnyOf(socket, {235}, QObject::tr("Erreur d'authentification"), errorMessage)) {
        return false;
    }

    qDebug() << "[SMTP] Etape 4/4: transfert du message MIME";
    if (!writeLine(socket, QString("MAIL FROM:<%1>").arg(smtp.from).toUtf8(), errorMessage)) {
        return false;
    }
    if (!expectCode(socket, 250, QObject::tr("Échec d'envoi"), errorMessage)) {
        return false;
    }

    if (!writeLine(socket, QString("RCPT TO:<%1>").arg(to).toUtf8(), errorMessage)) {
        return false;
    }
    if (!expectAnyOf(socket, {250, 251}, QObject::tr("Échec d'envoi"), errorMessage)) {
        return false;
    }

    if (!writeLine(socket, "DATA", errorMessage)) {
        return false;
    }
    if (!expectCode(socket, 354, QObject::tr("Échec d'envoi"), errorMessage)) {
        return false;
    }

    const QByteArray payload = mimeMessage.toUtf8();
    if (socket.write(payload) != payload.size() || !socket.waitForBytesWritten(30000)) {
        if (errorMessage) {
            *errorMessage = QObject::tr("Échec d'envoi");
        }
        qWarning() << "[SMTP] Echec d'envoi du message:" << socket.errorString();
        return false;
    }
    qDebug() << "[SMTP >>] [MIME payload]";

    if (!writeLine(socket, ".", errorMessage)) {
        return false;
    }
    if (!expectCode(socket, 250, QObject::tr("Échec d'envoi"), errorMessage)) {
        return false;
    }

    writeLine(socket, "QUIT", nullptr);
    expectAnyOf(socket, {221}, QObject::tr("Connexion SMTP échouée"), nullptr);

    qDebug() << "[SMTP] Email envoyé avec succès";
    if (errorMessage) {
        errorMessage->clear();
    }
    return true;
}

} // namespace employes