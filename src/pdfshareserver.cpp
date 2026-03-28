#include "pdfshareserver.h"

#include <QFile>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUuid>

static QByteArray httpResponse(int status,
                               const QByteArray &contentType,
                               const QByteArray &body,
                               const QByteArray &extraHeaders = {})
{
    QByteArray out;
    if (status == 200)
        out += "HTTP/1.1 200 OK\r\n";
    else if (status == 404)
        out += "HTTP/1.1 404 Not Found\r\n";
    else
        out += "HTTP/1.1 500 Internal Server Error\r\n";

    out += "Connection: close\r\n";
    if (!contentType.isEmpty())
        out += "Content-Type: " + contentType + "\r\n";
    if (!extraHeaders.isEmpty())
        out += extraHeaders;
    out += "Content-Length: " + QByteArray::number(body.size()) + "\r\n\r\n";
    out += body;
    return out;
}

PdfShareServer &PdfShareServer::instance()
{
    static PdfShareServer inst;
    return inst;
}

PdfShareServer::PdfShareServer(QObject *parent)
    : QObject(parent)
    , m_server(new QTcpServer(this))
{
    connect(m_server, &QTcpServer::newConnection, this, [this]() {
        while (m_server->hasPendingConnections())
        {
            QTcpSocket *sock = m_server->nextPendingConnection();
            sock->setParent(this);

            connect(sock, &QTcpSocket::readyRead, this, [this, sock]() {
                const QByteArray data = sock->readAll();
                const int firstLineEnd = data.indexOf("\r\n");
                const QByteArray firstLine = firstLineEnd >= 0 ? data.left(firstLineEnd) : data;

                // Very small parser: only handle "GET /invoice/<token>.pdf HTTP/..."
                const QList<QByteArray> parts = firstLine.split(' ');
                if (parts.size() < 2 || parts[0] != "GET")
                {
                    const QByteArray body = "Method not supported";
                    sock->write(httpResponse(500, "text/plain; charset=utf-8", body));
                    sock->disconnectFromHost();
                    return;
                }

                const QByteArray path = parts[1];
                const QByteArray expected = QByteArray("/invoice/") + m_token.toUtf8() + ".pdf";

                if (m_token.isEmpty() || path != expected)
                {
                    const QByteArray body = "Not found";
                    sock->write(httpResponse(404, "text/plain; charset=utf-8", body));
                    sock->disconnectFromHost();
                    return;
                }

                QFile f(m_pdfFilePath);
                if (!f.open(QIODevice::ReadOnly))
                {
                    const QByteArray body = "File not available";
                    sock->write(httpResponse(500, "text/plain; charset=utf-8", body));
                    sock->disconnectFromHost();
                    return;
                }

                const QByteArray pdf = f.readAll();
                QByteArray extra;
                extra += "Content-Disposition: inline; filename=\"facture.pdf\"\r\n";
                sock->write(httpResponse(200, "application/pdf", pdf, extra));
                sock->disconnectFromHost();
            });

            connect(sock, &QTcpSocket::disconnected, sock, &QObject::deleteLater);
        }
    });
}

bool PdfShareServer::isListening() const
{
    return m_server && m_server->isListening();
}

void PdfShareServer::ensureListening()
{
    if (!m_server)
        return;
    if (m_server->isListening())
        return;

    // Bind to all interfaces on an ephemeral port.
    if (m_server->listen(QHostAddress::AnyIPv4, 0))
        m_port = m_server->serverPort();
}

QString PdfShareServer::pickLanAddress() const
{
    const QList<QHostAddress> addrs = QNetworkInterface::allAddresses();
    for (const QHostAddress &a : addrs)
    {
        if (a.protocol() != QAbstractSocket::IPv4Protocol)
            continue;
        if (a.isLoopback())
            continue;
        const QString s = a.toString();
        if (!s.isEmpty())
            return s;
    }
    return QStringLiteral("127.0.0.1");
}

QString PdfShareServer::registerPdf(const QString &pdfFilePath)
{
    m_pdfFilePath = pdfFilePath;
    m_token = QUuid::createUuid().toString(QUuid::WithoutBraces);

    ensureListening();

    const QString host = pickLanAddress();
    if (!isListening() || m_port == 0)
        return QString();

    return QStringLiteral("http://%1:%2/invoice/%3.pdf").arg(host).arg(m_port).arg(m_token);
}
