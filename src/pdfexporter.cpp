#include "pdfexporter.h"

#include <QAbstractItemModel>
#include <QDateTime>
#include <QPdfWriter>
#include <QPageSize>
#include <QTextDocument>

static QString htmlEscape(const QString &s)
{
    return s.toHtmlEscaped();
}

bool PdfExporter::exportTableToPdf(const QString &filePath,
                                  const QString &title,
                                  const QAbstractItemModel *model,
                                  QString *errorOut)
{
    if (!model)
    {
        if (errorOut)
            *errorOut = QStringLiteral("Model is null.");
        return false;
    }

    if (filePath.trimmed().isEmpty())
    {
        if (errorOut)
            *errorOut = QStringLiteral("Chemin PDF invalide.");
        return false;
    }

    const int rows = model->rowCount();
    const int cols = model->columnCount();

    QString html;
    html += "<html><head><meta charset='utf-8'/>";
    html += "<style>";
    html += "body{font-family:Arial,Helvetica,sans-serif;font-size:10pt;}";
    html += "h1{font-size:14pt;margin:0 0 6px 0;}";
    html += "p.meta{color:#555;margin:0 0 10px 0;}";
    html += "table{border-collapse:collapse;width:100%;}";
    html += "th,td{border:1px solid #bbb;padding:6px;vertical-align:top;}";
    html += "th{background:#eee;}";
    html += "</style></head><body>";

    html += "<h1>" + htmlEscape(title) + "</h1>";
    html += "<p class='meta'>Généré le " + htmlEscape(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm")) + "</p>";

    html += "<table><thead><tr>";
    for (int c = 0; c < cols; ++c)
    {
        const QString header = model->headerData(c, Qt::Horizontal).toString();
        html += "<th>" + htmlEscape(header) + "</th>";
    }
    html += "</tr></thead><tbody>";

    for (int r = 0; r < rows; ++r)
    {
        html += "<tr>";
        for (int c = 0; c < cols; ++c)
        {
            const QString cell = model->data(model->index(r, c), Qt::DisplayRole).toString();
            html += "<td>" + htmlEscape(cell) + "</td>";
        }
        html += "</tr>";
    }

    html += "</tbody></table>";
    html += "</body></html>";

    QTextDocument doc;
    doc.setHtml(html);

    QPdfWriter writer(filePath);
    writer.setPageSize(QPageSize(QPageSize::A4));
    writer.setTitle(title);

    doc.print(&writer);
    return true;
}
