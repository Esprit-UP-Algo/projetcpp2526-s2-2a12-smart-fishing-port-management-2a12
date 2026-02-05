#include "pdfexporter.h"

#include <QAbstractItemModel>
#include <QDateTime>
#include <QFile>
#include <QPdfWriter>
#include <QTextDocument>

static QString htmlEscape(const QString &s)
{
    QString out = s;
    out.replace('&', "&amp;");
    out.replace('<', "&lt;");
    out.replace('>', "&gt;");
    out.replace('"', "&quot;");
    return out;
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

    QPdfWriter writer(filePath);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setPageMargins(QMarginsF(15, 15, 15, 15));

    QString html;
    html += "<h2 style='font-family:Segoe UI,Arial; color:#0c1a29; margin:0 0 6px 0;'>" + htmlEscape(title) + "</h2>";
    html += "<div style='font-family:Segoe UI,Arial; color:#444; font-size:10pt; margin-bottom:10px;'>";
    html += "Généré le " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm") + "</div>";

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

    QTextDocument doc;
    doc.setHtml(html);
    doc.setPageSize(writer.pageLayout().fullRectPoints().size());
    doc.print(&writer);

    return true;
}
