#include "pdfexporter.h"

#include <QAbstractItemModel>
#include <QDate>
#include <QDateTime>
#include <QBuffer>
#include <QFile>
#include <QImage>
#include <QLocale>
#include <QMap>
#include <QPdfWriter>
#include <QPainter>
#include <QTextDocument>

#include "quaimodel.h"

// Reuse the QR generator already present in the integrated project.
#include "../../GestionDesNavires/qrcodegen.hpp"

static QString htmlEscape(const QString &s)
{
    QString out = s;
    out.replace('&', "&amp;");
    out.replace('<', "&lt;");
    out.replace('>', "&gt;");
    out.replace('"', "&quot;");
    return out;
}

static QString qrPngDataUriForText(const QString &payload, int targetSizePx = 160)
{
    using qrcodegen::QrCode;

    const QByteArray utf8 = payload.toUtf8();
    const QrCode qr = QrCode::encodeText(utf8.constData(), QrCode::Ecc::MEDIUM);

    const int border = 2;
    const int qrSize = qr.getSize();
    int scale = targetSizePx / (qrSize + border * 2);
    if (scale < 1)
        scale = 1;

    const int imgSize = (qrSize + border * 2) * scale;
    QImage img(imgSize, imgSize, QImage::Format_ARGB32);
    img.fill(Qt::white);

    QPainter p(&img);
    p.setRenderHint(QPainter::Antialiasing, false);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);

    for (int y = 0; y < qrSize; ++y)
    {
        for (int x = 0; x < qrSize; ++x)
        {
            if (!qr.getModule(x, y))
                continue;
            const int rx = (x + border) * scale;
            const int ry = (y + border) * scale;
            p.drawRect(rx, ry, scale, scale);
        }
    }

    QByteArray png;
    QBuffer buf(&png);
    buf.open(QIODevice::WriteOnly);
    img.save(&buf, "PNG");

    return QStringLiteral("data:image/png;base64,%1").arg(QString::fromLatin1(png.toBase64()));
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

static QString htmlDocHeader(const QString &title,
                             const QString &titleExtraStyle = QString(),
                             const QString &metaExtraStyle = QString())
{
    QString html;
    html += "<div style='font-family:Segoe UI,Arial;'>";
    html += "<h2 style='color:#0c1a29; margin:0 0 6px 0;" + titleExtraStyle + "'>" + htmlEscape(title) + "</h2>";
    html += "<div style='color:#444; font-size:10pt; margin-bottom:12px;" + metaExtraStyle + "'>";
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

    const QString idQuai = modelString(model, row, QuaiModel::IdQuai);
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
    const QString totalDisplay = loc.toString(total, 'f', 2);
    const QString qrPayload = QStringLiteral("Total (DT): %1").arg(QString::number(total, 'f', 2));
    const QString qrImg = qrPngDataUriForText(qrPayload, 160);

    QString html = htmlDocHeader(title,
                                 " text-align:center; color:#1e8de0; margin:0 0 8px 0;",
                                 " text-align:center; margin-bottom:18px;");

    html += "<div style='margin:0 0 22px 0; padding:12px 14px; background:#f4f7fb; border:1px solid #dbe5f0; border-radius:12px; font-family:Segoe UI,Arial;'>";
    html += "<div style='font-size:10.6pt; color:#0c1a29; font-weight:700; margin:0 0 6px 0;'>Résumé</div>";
    html += "<div style='font-size:9.6pt; color:#444; margin:0;'>Matricule : <b>" + htmlEscape(matricule) + "</b></div>";
    html += "</div>";

    html += "<div style='margin:0 0 10px 0; font-size:10.8pt; color:#0c1a29; font-weight:700; border-left:4px solid #1e8de0; padding:2px 0 2px 10px;'>Détails de la facture</div>";

    html += "<table cellspacing='0' cellpadding='0' style='width:100%; border-collapse:collapse; font-family:Segoe UI,Arial;'>";
    html += "<tr>";
    html += "<td style='width:64%; vertical-align:top;'>";
    html += "<div style='border:1px solid #dbe5f0; border-radius:12px; overflow:hidden;'>";
    html += "<table cellspacing='0' cellpadding='8' style='width:100%; border-collapse:collapse; font-family:Segoe UI,Arial; font-size:10pt;'>";

    auto addRow = [&html](const QString &k, const QString &v) {
        html += "<tr>";
        html += "<td style='width:38%; background:#f4f7fb; border:1px solid #dbe5f0;'><b>" + htmlEscape(k) + "</b></td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(v) + "</td>";
        html += "</tr>";
    };

    addRow("Matricule", matricule);
    addRow("Taille (m)", loc.toString(taille, 'f', 1));
    addRow("Arrivée", arrivee.isValid() ? arrivee.toString("yyyy-MM-dd HH:mm") : QString());
    addRow("Départ", depart.isValid() ? depart.toString("yyyy-MM-dd HH:mm") : QString());
    addRow("Durée (h)", loc.toString(dureeHeures, 'f', 2));
    addRow("Prix par heure (DT/h)", loc.toString(prixH, 'f', 2));
    addRow("Total (DT)", totalDisplay);
    addRow("État quai", etat);
    addRow("Retard (min)", QString::number(retardMin));

    html += "</table>";
    html += "</div>";
    html += "</td>";

    html += "<td style='width:36%; vertical-align:top; padding-left:14px;'>";
    html += "<div style='border:1px solid #dbe5f0; border-radius:12px; padding:12px; background:#f4f7fb; text-align:center; font-family:Segoe UI,Arial;'>";
    html += "<div style='font-size:10.6pt; color:#0c1a29; font-weight:700; margin:0 0 10px 0;'>QR Code</div>";
    html += "<img src='" + qrImg + "' width='160' height='160' style='display:block; margin:0 auto;'/>";
    html += "</div>";
    html += "</td>";
    html += "</tr>";
    html += "</table>";

    html += htmlDocFooter();
    return exportHtmlToPdf(filePath, html, error);
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

    QString html = htmlDocHeader(title,
                                 " text-align:center; color:#1e8de0; margin:0 0 8px 0;",
                                 " text-align:center; margin-bottom:18px;");

    html += "<div style='margin:0 0 22px 0; padding:12px 14px; background:#f4f7fb; border:1px solid #dbe5f0; border-radius:12px; font-family:Segoe UI,Arial;'>";
    html += "<div style='font-size:10.6pt; color:#0c1a29; font-weight:700; margin:0 0 6px 0;'>Résumé</div>";
    html += "<div style='font-size:9.6pt; color:#444; margin:0;'>Date : <b>" + htmlEscape(day.toString("yyyy-MM-dd")) + "</b></div>";
    html += "</div>";

    html += "<div style='margin:0 0 10px 0; font-size:10.8pt; color:#0c1a29; font-weight:700; border-left:4px solid #1e8de0; padding:2px 0 2px 10px;'>Tableau d’occupation</div>";

    html += "<div style='border:1px solid #dbe5f0; border-radius:12px; overflow:hidden; margin:0 0 8px 0;'>";
    html += "<table cellspacing='0' cellpadding='8' style='width:100%; border-collapse:collapse; font-family:Segoe UI,Arial; font-size:9.8pt;'>";
    html += "<tr>";
    const QStringList headers = {"Matricule", "Arrivée", "Départ", "Heures (jour)", "État", "Prix (DT/h)"};
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
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(modelString(model, r, QuaiModel::Matricule)) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(a.toString("yyyy-MM-dd HH:mm")) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(d.toString("yyyy-MM-dd HH:mm")) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(loc.toString(h, 'f', 2)) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(modelString(model, r, QuaiModel::Etat)) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(loc.toString(modelDouble(model, r, QuaiModel::Prix), 'f', 2)) + "</td>";
        html += "</tr>";
    }
    html += "</table>";
    html += "</div>";

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
        QString key = modelString(model, r, QuaiModel::Matricule);
        if (key.trimmed().isEmpty())
            key = modelString(model, r, QuaiModel::IdQuai);
        const double priceH = modelDouble(model, r, QuaiModel::Prix);
        const QDateTime a = modelDateTime(model, r, QuaiModel::Arrivee);
        const QDateTime d = modelDateTime(model, r, QuaiModel::Depart);
        const double h = overlapHours(a, d, periodStart, periodEnd);
        if (h <= 0.0)
            continue;
        hours[key] += h;
        revenue[key] += h * priceH;
    }

    // Sort keys by revenue desc
    QList<QString> keys = revenue.keys();
    std::sort(keys.begin(), keys.end(), [&revenue](const QString &a, const QString &b) {
        return revenue.value(a) > revenue.value(b);
    });

    QString html = htmlDocHeader(title,
                                 " text-align:center; color:#1e8de0; margin:0 0 8px 0;",
                                 " text-align:center; margin-bottom:18px;");

    html += "<div style='margin:0 0 22px 0; padding:12px 14px; background:#f4f7fb; border:1px solid #dbe5f0; border-radius:12px; font-family:Segoe UI,Arial;'>";
    html += "<div style='font-size:10.6pt; color:#0c1a29; font-weight:700; margin:0 0 6px 0;'>Résumé</div>";
    html += "<div style='font-size:9.6pt; color:#444; margin:0;'>Période : <b>" + htmlEscape(startDate.toString("yyyy-MM")) + "</b></div>";
    html += "</div>";

    html += "<div style='margin:0 0 10px 0; font-size:10.8pt; color:#0c1a29; font-weight:700; border-left:4px solid #1e8de0; padding:2px 0 2px 10px;'>Tableau des revenus</div>";

    html += "<div style='border:1px solid #dbe5f0; border-radius:12px; overflow:hidden; margin:0 0 8px 0;'>";
    html += "<table cellspacing='0' cellpadding='8' style='width:100%; border-collapse:collapse; font-family:Segoe UI,Arial; font-size:9.8pt;'>";
    html += "<tr>";
    const QStringList headers = {"Matricule quai", "Heures", "Revenu (DT)"};
    for (const QString &h : headers)
        html += "<th style='text-align:left; background:#1e8de0; color:white; border:1px solid #1e8de0;'>" + htmlEscape(h) + "</th>";
    html += "</tr>";

    int idx = 0;
    double totalRevenue = 0.0;
    for (const QString &key : keys)
    {
        ++idx;
        const QString rowBg = (idx % 2 == 1) ? "#f4f7fb" : "#ffffff";
        const double r = revenue.value(key);
        totalRevenue += r;
        html += "<tr style='background:" + rowBg + "'>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(key) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(loc.toString(hours.value(key), 'f', 2)) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'><b>" + htmlEscape(loc.toString(r, 'f', 2)) + "</b></td>";
        html += "</tr>";
    }
    html += "</table>";
    html += "</div>";

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

    QString html = htmlDocHeader(title,
                                 " text-align:center; color:#1e8de0; margin:0 0 8px 0;",
                                 " text-align:center; margin-bottom:18px;");
    // More visible structure + breathing room before the table.
    html += "<div style='margin:0 0 22px 0; padding:12px 14px; background:#f4f7fb; border:1px solid #dbe5f0; border-radius:12px; font-family:Segoe UI,Arial;'>";
    html += "<div style='font-size:10.6pt; color:#0c1a29; font-weight:700; margin:0 0 6px 0;'>Résumé</div>";
    html += "<div style='font-size:9.6pt; color:#444; margin:0;'>Les retards et dépassements sont calculés à partir des dates d'arrivée/départ.</div>";
    html += "</div>";

    html += "<div style='margin:0 0 10px 0; font-size:10.8pt; color:#0c1a29; font-weight:700; border-left:4px solid #1e8de0; padding:2px 0 2px 10px;'>Tableau des anomalies</div>";

    html += "<div style='border:1px solid #dbe5f0; border-radius:12px; overflow:hidden; margin:0 0 8px 0;'>";
    html += "<table cellspacing='0' cellpadding='8' style='width:100%; border-collapse:collapse; font-family:Segoe UI,Arial; font-size:9.6pt;'>";
    html += "<tr>";
    const QStringList headers = {"Matricule", "État", "Arrivée", "Départ", "Retard (min)", "Dépassement (min)"};
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
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(modelString(model, r, QuaiModel::Matricule)) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(etat) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(a.isValid() ? a.toString("yyyy-MM-dd HH:mm") : QString()) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + htmlEscape(d.isValid() ? d.toString("yyyy-MM-dd HH:mm") : QString()) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'>" + QString::number(retard) + "</td>";
        html += "<td style='border:1px solid #dbe5f0;'><b>" + QString::number(depassement) + "</b></td>";
        html += "</tr>";
    }
    html += "</table>";
    html += "</div>";

    if (shown == 0)
        html += "<div style='margin-top:10px; color:#444; font-size:10pt;'>Aucune anomalie détectée.</div>";

    html += htmlDocFooter();
    return exportHtmlToPdf(filePath, html, error);
}
