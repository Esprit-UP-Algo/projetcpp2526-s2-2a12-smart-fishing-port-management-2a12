#include "pdfshareserver.h"

#include <QFile>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QTcpServer>
#include <QTcpSocket>
#include <QUuid>

static bool isPrivateIPv4(const QHostAddress &addr)
{
    if (addr.protocol() != QAbstractSocket::IPv4Protocol)
        return false;
    const quint32 v = addr.toIPv4Address();

    // 10.0.0.0/8
    if ((v & 0xFF000000u) == 0x0A000000u)
        return true;
    // 172.16.0.0/12
    if ((v & 0xFFF00000u) == 0xAC100000u)
        return true;
    // 192.168.0.0/16
    if ((v & 0xFFFF0000u) == 0xC0A80000u)
        return true;
    return false;
}

static bool isLinkLocalIPv4(const QHostAddress &addr)
{
    if (addr.protocol() != QAbstractSocket::IPv4Protocol)
        return false;
    const quint32 v = addr.toIPv4Address();
    // 169.254.0.0/16
    return (v & 0xFFFF0000u) == 0xA9FE0000u;
}

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
                if (!sock->canReadLine())
                    return;

                const QByteArray firstLine = sock->readLine().trimmed();

                // Very small parser: handle "GET <path> HTTP/..."
                const QList<QByteArray> parts = firstLine.split(' ');
                if (parts.size() < 2 || parts[0] != "GET")
                {
                    const QByteArray body = "Method not supported";
                    sock->write(httpResponse(500, "text/plain; charset=utf-8", body));
                    sock->disconnectFromHost();
                    return;
                }

                QByteArray path = parts[1];
                const int q = path.indexOf('?');
                if (q >= 0)
                    path = path.left(q);

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

    // Try a stable port first (helps with firewall rules / predictable URLs), then fall back.
    static constexpr quint16 kPreferredPort = 8080;
    if (m_server->listen(QHostAddress::AnyIPv4, kPreferredPort))
    {
        m_port = m_server->serverPort();
        return;
    }
    if (m_server->listen(QHostAddress::AnyIPv4, 0))
        m_port = m_server->serverPort();
}

QString PdfShareServer::pickLanAddress() const
{
    // Prefer a private IPv4 on an active, non-loopback interface.
    QString bestPrivate;
    QString bestAny;

    const QList<QNetworkInterface> ifaces = QNetworkInterface::allInterfaces();
    for (const QNetworkInterface &iface : ifaces)
    {
        const auto flags = iface.flags();
        if (!(flags & QNetworkInterface::IsUp) || !(flags & QNetworkInterface::IsRunning))
            continue;
        if (flags & QNetworkInterface::IsLoopBack)
            continue;

        const QList<QNetworkAddressEntry> entries = iface.addressEntries();
        for (const QNetworkAddressEntry &e : entries)
        {
            const QHostAddress ip = e.ip();
            if (ip.protocol() != QAbstractSocket::IPv4Protocol)
                continue;
            if (ip.isNull() || ip.isLoopback())
                continue;
            if (isLinkLocalIPv4(ip))
                continue;

            const QString s = ip.toString();
            if (s.isEmpty())
                continue;

            if (bestAny.isEmpty())
                bestAny = s;
            if (isPrivateIPv4(ip) && bestPrivate.isEmpty())
                bestPrivate = s;
        }
    }

    if (!bestPrivate.isEmpty())
        return bestPrivate;
    if (!bestAny.isEmpty())
        return bestAny;
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
