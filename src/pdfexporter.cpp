#include "pdfexporter.h"

#include <QAbstractItemModel>
#include <QDate>
#include <QDateTime>
#include <QFile>
#include <QFileInfo>
#include <QLocale>
#include <QMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPdfWriter>
#include <QUrl>
#include <QUrlQuery>
#include <QBuffer>
#include <QEventLoop>
#include <QTimer>
#include <QTextDocument>

#include "quaimodel.h"
#include "pdfshareserver.h"

static QByteArray downloadBytes(const QUrl &url, int timeoutMs = 5000)
{
    QNetworkAccessManager nam;
    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

    QNetworkReply *reply = nam.get(req);
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    timer.start(timeoutMs);
    loop.exec();

    QByteArray out;
    if (timer.isActive() && reply->error() == QNetworkReply::NoError)
        out = reply->readAll();
    reply->deleteLater();
    return out;
}

static QByteArray uploadBytesPut(const QUrl &url, const QByteArray &bytes, const QByteArray &contentType, int timeoutMs = 20000)
{
    QNetworkAccessManager nam;
    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
    if (!contentType.isEmpty())
        req.setHeader(QNetworkRequest::ContentTypeHeader, QString::fromLatin1(contentType));

    QNetworkReply *reply = nam.put(req, bytes);
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    timer.start(timeoutMs);
    loop.exec();

    QByteArray out;
    if (timer.isActive() && reply->error() == QNetworkReply::NoError)
        out = reply->readAll();
    reply->deleteLater();
    return out;
}

static QString uploadPdfToTransferSh(const QString &pdfFilePath)
{
    QFile f(pdfFilePath);
    if (!f.open(QIODevice::ReadOnly))
        return {};

    const QByteArray bytes = f.readAll();
    if (bytes.isEmpty())
        return {};

    const QString fileName = QFileInfo(pdfFilePath).fileName().isEmpty() ? QStringLiteral("facture_quai.pdf") : QFileInfo(pdfFilePath).fileName();
    const QByteArray encoded = QUrl::toPercentEncoding(fileName);
    const QUrl url(QStringLiteral("https://transfer.sh/") + QString::fromLatin1(encoded));

    const QByteArray resp = uploadBytesPut(url, bytes, "application/pdf", 25000);
    const QString link = QString::fromUtf8(resp).trimmed();
    if (link.startsWith(QStringLiteral("https://")))
        return link;
    if (link.startsWith(QStringLiteral("http://")))
        return link;
    return {};
}

static QString pngBytesToDataUri(const QByteArray &png)
{
    if (png.isEmpty())
        return {};
    return QStringLiteral("data:image/png;base64,%1").arg(QString::fromLatin1(png.toBase64()));
}

static QString htmlEscape(const QString &s)
{
    QString out = s;
    out.replace('&', "&amp;");
    out.replace('<', "&lt;");
    out.replace('>', "&gt;");
    out.replace('"', "&quot;");
    return out;
}

static bool exportHtmlToPdf(const QString &filePath, const QString &html, QString *error)
{
    QPdfWriter writer(filePath);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setPageMargins(QMarginsF(15, 15, 15, 15));

    QTextDocument doc;
    doc.setHtml(html);
    doc.setPageSize(writer.pageLayout().fullRectPoints().size());
    doc.print(&writer);

    if (!QFile::exists(filePath))
    {
        if (error)
            *error = QStringLiteral("PDF export failed");
        return false;
    }
    return true;
}

static QString htmlDocHeader(const QString &title)
{
    QString html;
    html += "<div style='font-family:Segoe UI,Arial;'>";
    html += "<h2 style='color:#0c1a29; margin:0 0 6px 0;'>" + htmlEscape(title) + "</h2>";
    html += "<div style='color:#444; font-size:10pt; margin-bottom:12px;'>";
    html += "Généré le " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm") + "</div>";
    return html;
}

static QString htmlDocFooter()
{
    return "</div>";
}

static QDateTime modelDateTime(const QAbstractItemModel *model, int row, int col)
{
    if (!model)
        return {};
    const QModelIndex idx = model->index(row, col);
    QVariant v = idx.data(QuaiModel::SortValueRole);
    if (v.canConvert<QDateTime>())
        return v.toDateTime();

    const QString s = idx.data(Qt::DisplayRole).toString();
    const QDateTime dt = QDateTime::fromString(s, "yyyy-MM-dd HH:mm");
    return dt;
}

static double modelDouble(const QAbstractItemModel *model, int row, int col)
{
    if (!model)
        return 0.0;
    const QModelIndex idx = model->index(row, col);
    QVariant v = idx.data(QuaiModel::SortValueRole);
    if (v.isValid() && v.canConvert<double>())
        return v.toDouble();

    bool ok = false;
    const double d = QLocale().toDouble(idx.data(Qt::DisplayRole).toString(), &ok);
    return ok ? d : idx.data(Qt::DisplayRole).toDouble();
}

static int modelInt(const QAbstractItemModel *model, int row, int col)
{
    if (!model)
        return 0;
    const QModelIndex idx = model->index(row, col);
    QVariant v = idx.data(QuaiModel::SortValueRole);
    if (v.isValid() && v.canConvert<int>())
        return v.toInt();
    return idx.data(Qt::DisplayRole).toInt();
}

static QString modelString(const QAbstractItemModel *model, int row, int col)
{
    if (!model)
        return {};
    return model->index(row, col).data(Qt::DisplayRole).toString();
}

static double overlapHours(const QDateTime &aStart, const QDateTime &aEnd, const QDateTime &bStart, const QDateTime &bEnd)
{
    if (!aStart.isValid() || !aEnd.isValid() || !bStart.isValid() || !bEnd.isValid())
        return 0.0;
    const QDateTime start = (aStart > bStart) ? aStart : bStart;
    const QDateTime end = (aEnd < bEnd) ? aEnd : bEnd;
    const qint64 secs = start.secsTo(end);
    if (secs <= 0)
        return 0.0;
    return static_cast<double>(secs) / 3600.0;
}

bool PdfExporter::exportTableToPdf(const QString &filePath,
                                  const QString &title,
                                  const QAbstractItemModel *model,
                                  QString *error)
{
    if (!model)
    {
        if (error)
            *error = QStringLiteral("Model is null");
        return false;
    }

    QString html = htmlDocHeader(title);

    html += "<table cellspacing='0' cellpadding='4' style='width:100%; border-collapse:collapse; font-family:Segoe UI,Arial; font-size:9.5pt;'>";

    // header
    html += "<tr>";
    for (int c = 0; c < model->columnCount(); ++c)
    {
        html += "<th style='text-align:left; background:#1e8de0; color:white; border:1px solid #1e8de0;'>";
        html += htmlEscape(model->headerData(c, Qt::Horizontal, Qt::DisplayRole).toString());
        html += "</th>";
    }
    html += "</tr>";

    // rows
    for (int r = 0; r < model->rowCount(); ++r)
    {
        const QString rowBg = (r % 2 == 0) ? "#f4f7fb" : "#ffffff";
        html += "<tr style='background:" + rowBg + "'>";
        for (int c = 0; c < model->columnCount(); ++c)
        {
            html += "<td style='border:1px solid #dbe5f0;'>";
            html += htmlEscape(model->index(r, c).data(Qt::DisplayRole).toString());
            html += "</td>";
        }
        html += "</tr>";
    }

    html += "</table>";

    html += htmlDocFooter();
    return exportHtmlToPdf(filePath, html, error);
}

bool PdfExporter::exportInvoiceToPdf(const QString &filePath,
                                    const QString &title,
                                    const QAbstractItemModel *model,
                                    int row,
                                    QString *error)
{
    if (!model)
    {
        if (error)
            *error = QStringLiteral("Model is null");
        return false;
    }
    if (row < 0 || row >= model->rowCount())
    {
        if (error)
            *error = QStringLiteral("Invalid row selection");
        return false;
    }

    const QString matricule = modelString(model, row, QuaiModel::Matricule);
    const double taille = modelDouble(model, row, QuaiModel::Taille);
    const QDateTime arrivee = modelDateTime(model, row, QuaiModel::Arrivee);
    const QDateTime depart = modelDateTime(model, row, QuaiModel::Depart);
    const QString etat = modelString(model, row, QuaiModel::Etat);
    const double prixH = modelDouble(model, row, QuaiModel::Prix);
    const int retardMin = modelInt(model, row, QuaiModel::Retard);

    const qint64 secs = arrivee.isValid() && depart.isValid() ? arrivee.secsTo(depart) : 0;
    const double dureeHeures = secs > 0 ? static_cast<double>(secs) / 3600.0 : 0.0;
    const double total = dureeHeures * prixH;

    const QLocale loc;

    QString htmlCore = htmlDocHeader(title);
    htmlCore += "<div style='margin-bottom:10px; font-size:10.5pt; color:#0c1a29;'><b>Facture - Location de quai</b></div>";
    htmlCore += "<table cellspacing='0' cellpadding='6' style='width:100%; border-collapse:collapse; font-family:Segoe UI,Arial; font-size:10pt;'>";

    auto addRow = [&htmlCore](const QString &k, const QString &v) {
        htmlCore += "<tr>";
        htmlCore += "<td style='width:34%; background:#f4f7fb; border:1px solid #dbe5f0;'><b>" + htmlEscape(k) + "</b></td>";
        htmlCore += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(v) + "</td>";
        htmlCore += "</tr>";
    };

    addRow("Matricule", matricule);
    addRow("Taille (m)", loc.toString(taille, 'f', 1));
    addRow("Arrivée", arrivee.isValid() ? arrivee.toString("yyyy-MM-dd HH:mm") : QString());
    addRow("Départ", depart.isValid() ? depart.toString("yyyy-MM-dd HH:mm") : QString());
    addRow("Durée (h)", loc.toString(dureeHeures, 'f', 2));
    addRow("Prix par heure (DT/h)", loc.toString(prixH, 'f', 2));
    addRow("Total (DT)", loc.toString(total, 'f', 2));
    addRow("État quai", etat);
    addRow("Retard (min)", QString::number(retardMin));

    htmlCore += "</table>";

    // First pass: export PDF without QR.
    const QString htmlNoQr = htmlCore + htmlDocFooter();
    if (!exportHtmlToPdf(filePath, htmlNoQr, error))
        return false;

    // Preferred: public link (not dependent on LAN/Wi‑Fi).
    const QString publicUrl = uploadPdfToTransferSh(filePath);
    QString qrTargetUrl = publicUrl;
    QString qrHint;

    if (!publicUrl.isEmpty())
        qrHint = QStringLiteral("Scannez le QR code pour ouvrir/télécharger la facture (Internet).\nLien : %1").arg(publicUrl);
    else
    {
        // Fallback: LAN link (requires same network). Keep as a backup in case upload fails.
        const QString lanUrl = PdfShareServer::instance().registerPdf(filePath);
        qrTargetUrl = lanUrl;
        if (!lanUrl.isEmpty())
            qrHint = QStringLiteral("Scannez le QR code pour ouvrir/télécharger la facture (même réseau).\nLien : %1").arg(lanUrl);
        else
            qrHint = QStringLiteral("Lien QR indisponible");
    }

    if (qrTargetUrl.isEmpty())
        return true; // PDF already exported.

    // Generate QR PNG and regenerate PDF embedding it.
    QUrl api(QStringLiteral("https://api.qrserver.com/v1/create-qr-code/"));
    QUrlQuery q;
    q.addQueryItem(QStringLiteral("size"), QStringLiteral("180x180"));
    q.addQueryItem(QStringLiteral("format"), QStringLiteral("png"));
    q.addQueryItem(QStringLiteral("data"), qrTargetUrl);
    api.setQuery(q);

    const QByteArray png = downloadBytes(api);
    const QString dataUri = pngBytesToDataUri(png);

    QString html = htmlCore;
    html += "<div style='margin-top:14px; font-size:10pt; color:#0c1a29;'><b>Ouvrir sur téléphone</b></div>";
    html += "<table cellspacing='0' cellpadding='6' style='width:100%; border-collapse:collapse; font-family:Segoe UI,Arial; font-size:9.8pt;'>";
    html += "<tr>";
    html += "<td style='width:190px; border:1px solid #dbe5f0; background:#ffffff; text-align:center;'>";
    if (!dataUri.isEmpty())
        html += "<img src='" + dataUri + "' style='width:170px; height:170px;'/>";
    else
        html += "<div style='color:#777;'>QR indisponible</div>";
    html += "</td>";
    html += "<td style='border:1px solid #dbe5f0; background:#f4f7fb; white-space:pre-line;'>";
    html += htmlEscape(qrHint);
    html += "</td>";
    html += "</tr></table>";
    html += htmlDocFooter();

    // Overwrite the same file with QR-enabled PDF.
    exportHtmlToPdf(filePath, html, nullptr);
    return true;
}

bool PdfExporter::exportDailyOccupationReportToPdf(const QString &filePath,
                                                  const QString &title,
                                                  const QAbstractItemModel *model,
                                                  const QDate &day,
                                                  QString *error)
{
    if (!model)
    {
        if (error)
            *error = QStringLiteral("Model is null");
        return false;
    }

    const QDateTime dayStart(day, QTime(0, 0));
    const QDateTime dayEnd = dayStart.addDays(1);
    const QLocale loc;

    QString html = htmlDocHeader(title);
    html += "<div style='margin-bottom:10px; font-size:10.5pt; color:#0c1a29;'><b>Rapport journalier d’occupation</b> — " + htmlEscape(day.toString("yyyy-MM-dd")) + "</div>";

    html += "<table cellspacing='0' cellpadding='5' style='width:100%; border-collapse:collapse; font-family:Segoe UI,Arial; font-size:9.8pt;'>";
    html += "<tr>";
    const QStringList headers = {"ID Quai", "Matricule", "Arrivée", "Départ", "Heures (jour)", "État", "Prix (DT/h)"};
    for (const QString &h : headers)
        html += "<th style='text-align:left; background:#1e8de0; color:white; border:1px solid #1e8de0;'>" + htmlEscape(h) + "</th>";
    html += "</tr>";

    double totalHours = 0.0;
    int shown = 0;
    for (int r = 0; r < model->rowCount(); ++r)
    {
        const QDateTime a = modelDateTime(model, r, QuaiModel::Arrivee);
        const QDateTime d = modelDateTime(model, r, QuaiModel::Depart);
        const double h = overlapHours(a, d, dayStart, dayEnd);
        if (h <= 0.0)
            continue;

        totalHours += h;
        ++shown;

        const QString rowBg = (shown % 2 == 1) ? "#f4f7fb" : "#ffffff";
        html += "<tr style='background:" + rowBg + "'>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(modelString(model, r, QuaiModel::IdQuai)) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(modelString(model, r, QuaiModel::Matricule)) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(a.toString("yyyy-MM-dd HH:mm")) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(d.toString("yyyy-MM-dd HH:mm")) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(loc.toString(h, 'f', 2)) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(modelString(model, r, QuaiModel::Etat)) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(loc.toString(modelDouble(model, r, QuaiModel::Prix), 'f', 2)) + "</td>";
        html += "</tr>";
    }
    html += "</table>";

    html += "<div style='margin-top:10px; color:#444; font-size:10pt;'>";
    html += "Total enregistrements: <b>" + QString::number(shown) + "</b> — Heures cumulées: <b>" + loc.toString(totalHours, 'f', 2) + "</b></div>";

    html += htmlDocFooter();
    return exportHtmlToPdf(filePath, html, error);
}

bool PdfExporter::exportMonthlyRevenueByQuaiToPdf(const QString &filePath,
                                                 const QString &title,
                                                 const QAbstractItemModel *model,
                                                 int year,
                                                 int month,
                                                 QString *error)
{
    if (!model)
    {
        if (error)
            *error = QStringLiteral("Model is null");
        return false;
    }

    const QDate startDate(year, month, 1);
    if (!startDate.isValid())
    {
        if (error)
            *error = QStringLiteral("Invalid month");
        return false;
    }

    const QDateTime periodStart(startDate, QTime(0, 0));
    const QDateTime periodEnd = periodStart.addMonths(1);
    const QLocale loc;

    QMap<QString, double> revenue;
    QMap<QString, double> hours;

    for (int r = 0; r < model->rowCount(); ++r)
    {
        const QString id = modelString(model, r, QuaiModel::IdQuai);
        const double priceH = modelDouble(model, r, QuaiModel::Prix);
        const QDateTime a = modelDateTime(model, r, QuaiModel::Arrivee);
        const QDateTime d = modelDateTime(model, r, QuaiModel::Depart);
        const double h = overlapHours(a, d, periodStart, periodEnd);
        if (h <= 0.0)
            continue;
        hours[id] += h;
        revenue[id] += h * priceH;
    }

    // Sort keys by revenue desc
    QList<QString> keys = revenue.keys();
    std::sort(keys.begin(), keys.end(), [&revenue](const QString &a, const QString &b) {
        return revenue.value(a) > revenue.value(b);
    });

    QString html = htmlDocHeader(title);
    html += "<div style='margin-bottom:10px; font-size:10.5pt; color:#0c1a29;'><b>Rapport mensuel de revenus par quai</b> — " + htmlEscape(startDate.toString("yyyy-MM")) + "</div>";

    html += "<table cellspacing='0' cellpadding='5' style='width:100%; border-collapse:collapse; font-family:Segoe UI,Arial; font-size:9.8pt;'>";
    html += "<tr>";
    const QStringList headers = {"ID Quai", "Heures", "Revenu (DT)"};
    for (const QString &h : headers)
        html += "<th style='text-align:left; background:#1e8de0; color:white; border:1px solid #1e8de0;'>" + htmlEscape(h) + "</th>";
    html += "</tr>";

    int idx = 0;
    double totalRevenue = 0.0;
    for (const QString &id : keys)
    {
        ++idx;
        const QString rowBg = (idx % 2 == 1) ? "#f4f7fb" : "#ffffff";
        const double r = revenue.value(id);
        totalRevenue += r;
        html += "<tr style='background:" + rowBg + "'>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(id) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(loc.toString(hours.value(id), 'f', 2)) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'><b>" + htmlEscape(loc.toString(r, 'f', 2)) + "</b></td>";
        html += "</tr>";
    }
    html += "</table>";
    html += "<div style='margin-top:10px; color:#444; font-size:10pt;'>Total revenu: <b>" + loc.toString(totalRevenue, 'f', 2) + "</b> DT</div>";
    html += htmlDocFooter();
    return exportHtmlToPdf(filePath, html, error);
}

bool PdfExporter::exportAnomaliesReportToPdf(const QString &filePath,
                                            const QString &title,
                                            const QAbstractItemModel *model,
                                            const QDateTime &now,
                                            QString *error)
{
    if (!model)
    {
        if (error)
            *error = QStringLiteral("Model is null");
        return false;
    }

    QString html = htmlDocHeader(title);
    html += "<div style='margin-bottom:10px; font-size:10.5pt; color:#0c1a29;'><b>Rapport anomalies</b> (retards, dépassements)</div>";

    html += "<table cellspacing='0' cellpadding='5' style='width:100%; border-collapse:collapse; font-family:Segoe UI,Arial; font-size:9.6pt;'>";
    html += "<tr>";
    const QStringList headers = {"ID Quai", "Matricule", "État", "Arrivée", "Départ", "Retard (min)", "Dépassement (min)"};
    for (const QString &h : headers)
        html += "<th style='text-align:left; background:#1e8de0; color:white; border:1px solid #1e8de0;'>" + htmlEscape(h) + "</th>";
    html += "</tr>";

    int shown = 0;
    for (int r = 0; r < model->rowCount(); ++r)
    {
        const QString etat = modelString(model, r, QuaiModel::Etat);
        const QDateTime d = modelDateTime(model, r, QuaiModel::Depart);
        const int retard = modelInt(model, r, QuaiModel::Retard);
        int depassement = 0;
        if (d.isValid() && now.isValid() && now > d)
        {
            if (etat.compare("Libre", Qt::CaseInsensitive) != 0 && etat.compare("En maintenance", Qt::CaseInsensitive) != 0)
                depassement = static_cast<int>(d.secsTo(now) / 60);
        }

        if (retard <= 0 && depassement <= 0)
            continue;

        ++shown;
        const QString rowBg = (shown % 2 == 1) ? "#f4f7fb" : "#ffffff";
        const QDateTime a = modelDateTime(model, r, QuaiModel::Arrivee);
        html += "<tr style='background:" + rowBg + "'>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(modelString(model, r, QuaiModel::IdQuai)) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(modelString(model, r, QuaiModel::Matricule)) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(etat) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(a.isValid() ? a.toString("yyyy-MM-dd HH:mm") : QString()) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(d.isValid() ? d.toString("yyyy-MM-dd HH:mm") : QString()) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + QString::number(retard) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'><b>" + QString::number(depassement) + "</b></td>";
        html += "</tr>";
    }
    html += "</table>";

    if (shown == 0)
        html += "<div style='margin-top:10px; color:#444; font-size:10pt;'>Aucune anomalie détectée.</div>";

    html += htmlDocFooter();
    return exportHtmlToPdf(filePath, html, error);
}
