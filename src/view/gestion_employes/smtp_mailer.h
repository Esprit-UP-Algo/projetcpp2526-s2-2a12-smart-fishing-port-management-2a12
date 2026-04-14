#ifndef SMTP_MAILER_H
#define SMTP_MAILER_H

#include <QString>
#include <QObject>

namespace employes {

struct SmtpConfig
{
    QString host;
    QString port;
    QString user;
    QString password;
    QString from;

    bool isComplete() const;
    QString missingDescription() const;
};

class SmtpMailer
{
public:
    // Envoie un email SMTP complet avec corps texte et pièce jointe PDF.
    // Retourne true si l'envoi aboutit, false sinon.
    static bool envoyerEmailSMTP(const QString &to,
                                 const QString &sujet,
                                 const QString &corps,
                                 const QString &cheminPDF,
                                 QString *errorMessage = nullptr);

    // Validation email simple et lisible côté UI.
    static bool isValidEmail(const QString &email);

    // Charge la configuration SMTP via ConfigManager, avec priorité aux variables d'environnement.
    static SmtpConfig loadConfig();

private:
    static QString readConfigValue(const QString &key);
    static QString buildMimeMessage(const QString &from,
                                    const QString &to,
                                    const QString &sujet,
                                    const QString &corps,
                                    const QString &cheminPDF,
                                    QString *errorMessage);
    static QByteArray wrapBase64(const QByteArray &data, int lineLength = 76);
};

} // namespace employes

#endif // SMTP_MAILER_H