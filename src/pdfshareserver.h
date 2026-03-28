#pragma once

#include <QObject>
#include <QString>

class QTcpServer;

class PdfShareServer final : public QObject
{
    Q_OBJECT

public:
    static PdfShareServer &instance();

    // Registers a PDF file to be served and returns a URL that can be opened
    // from another device on the same network (e.g. phone).
    QString registerPdf(const QString &pdfFilePath);

    bool isListening() const;

private:
    explicit PdfShareServer(QObject *parent = nullptr);

    QString pickLanAddress() const;
    void ensureListening();

    class QTcpServer *m_server = nullptr;
    quint16 m_port = 0;
    QString m_token;
    QString m_pdfFilePath;
};
