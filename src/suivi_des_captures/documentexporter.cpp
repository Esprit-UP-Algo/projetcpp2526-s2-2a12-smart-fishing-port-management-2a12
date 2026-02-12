#include "documentexporter.h"
#include <QFile>
#include <QTextStream>
#include <QAbstractTableModel>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QDateTime>
#include <QStandardPaths>

bool DocumentExporter::exportTableToPdf(const QString &filePath,
                                         const QString &title,
                                         const QSortFilterProxyModel *model,
                                         QString *error)
{
    if (!model) {
        if (error) *error = QObject::tr("Modèle invalide");
        return false;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        if (error) *error = QObject::tr("Impossible d'ouvrir le fichier PDF pour l'écriture.");
        return false;
    }

    QPdfWriter pdfWriter(&file);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageOrientation(QPageLayout::Landscape);
    pdfWriter.setCreator("Vision Sight Poissons");
    pdfWriter.setTitle(title);

    QPainter painter(&pdfWriter);
    if (!painter.isActive()) {
        if (error) *error = QObject::tr("Erreur lors de l'initialisation du peintre PDF.");
        return false;
    }

    // Configuration des polices
    QFont titleFont = painter.font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);

    QFont headerFont = painter.font();
    headerFont.setPointSize(10);
    headerFont.setBold(true);

    QFont textFont = painter.font();
    textFont.setPointSize(10);

    // Marges
    const int margin = 300; 
    
    // On va utiliser des coordonnées logiques
    int y = margin;
    const int pageWidth = pdfWriter.width();
    const int pageHeight = pdfWriter.height();

    // Titre
    painter.setFont(titleFont);
    painter.drawText(QRect(0, y, pageWidth, 500), Qt::AlignCenter, title);
    y += 600;

    // Date
    painter.setFont(textFont);
    painter.drawText(QRect(margin, y, pageWidth - 2*margin, 300), Qt::AlignRight, QObject::tr("Date: %1").arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm")));
    y += 400;

    // Tableau
    const int cols = model->columnCount();
    const int rows = model->rowCount();
    if (cols == 0) return true;

    // Calcul des largeurs de colonnes
    QVector<int> colWidths(cols);
    
    // Largeurs relatives approximatives
    // ID: 10%, Espece: 15%, Poids: 10%, Zone: 15%, Date: 15%, Navire: 15%, Qualite: 10%, Reste: 10%
    int contentWidth = pageWidth - 2 * margin;
    colWidths[0] = contentWidth * 0.10; // ID
    colWidths[1] = contentWidth * 0.12; // Espece
    colWidths[2] = contentWidth * 0.08; // Poids
    colWidths[3] = contentWidth * 0.12; // Zone
    colWidths[4] = contentWidth * 0.14; // Date Capture
    colWidths[5] = contentWidth * 0.08; // Etat
    colWidths[6] = contentWidth * 0.12; // Navire
    colWidths[7] = contentWidth * 0.10; // Qualité
    colWidths[8] = contentWidth * 0.14; // Date Entrée

    // Dessiner les en-têtes
    painter.setFont(headerFont);
    painter.setPen(Qt::black);
    painter.setBrush(QColor(240, 240, 240));

    int x = margin;
    int rowHeight = 400;

    // Fond des en-têtes
    painter.drawRect(margin, y, contentWidth, rowHeight);
    
    for (int col = 0; col < cols; ++col) {
        QRect rect(x, y, colWidths[col], rowHeight);
        painter.drawText(rect, Qt::AlignCenter, model->headerData(col, Qt::Horizontal).toString());
        painter.drawLine(x + colWidths[col], y, x + colWidths[col], y + rowHeight); // Ligne verticale
        x += colWidths[col];
    }
    y += rowHeight;

    // Dessiner les données
    painter.setFont(textFont);
    painter.setBrush(Qt::NoBrush);

    for (int row = 0; row < rows; ++row) {
        // Vérifier saut de page
        if (y + rowHeight > pageHeight - margin) {
            pdfWriter.newPage();
            y = margin;
            
            // Redessiner en-têtes
            painter.setFont(headerFont);
            painter.setBrush(QColor(240, 240, 240));
            painter.drawRect(margin, y, contentWidth, rowHeight);
             x = margin;
            for (int col = 0; col < cols; ++col) {
                QRect rect(x, y, colWidths[col], rowHeight);
                painter.drawText(rect, Qt::AlignCenter, model->headerData(col, Qt::Horizontal).toString());
                painter.drawLine(x + colWidths[col], y, x + colWidths[col], y + rowHeight);
                x += colWidths[col];
            }
            y += rowHeight;
            painter.setFont(textFont);
            painter.setBrush(Qt::NoBrush);
        }

        x = margin;
        // Alternance de couleurs
        if (row % 2 == 1) {
            painter.fillRect(margin, y, contentWidth, rowHeight, QColor(245, 250, 255));
        }

        for (int col = 0; col < cols; ++col) {
            QRect rect(x, y, colWidths[col], rowHeight);
            
            // Récupérer la donnée (DisplayRole pour l'affichage formaté)
            QString text = model->data(model->index(row, col), Qt::DisplayRole).toString();
            
            // Alignement
            Qt::Alignment align = Qt::AlignVCenter | Qt::AlignLeft;
            if (col == 2) align = Qt::AlignVCenter | Qt::AlignRight; // Poids à droite
            
            // Padding
            rect.adjust(50, 0, -50, 0); 
            
            painter.drawText(rect, align, text);
            
            // Restaurer rect pour ligne verticale
            rect.adjust(-50, 0, 50, 0); 
            painter.drawLine(x + colWidths[col], y, x + colWidths[col], y + rowHeight);
            
            x += colWidths[col];
        }
        
        // Cadre autour de la ligne
        painter.drawRect(margin, y, contentWidth, rowHeight);
        
        y += rowHeight;
    }

    painter.end();
    file.close();
    return true;
}

bool DocumentExporter::exportTableToExcel(const QString &filePath,
                                           const QString &title,
                                           const QSortFilterProxyModel *model,
                                           QString *error)
{
    Q_UNUSED(title);
    
    if (!model)
    {
        if (error)
            *error = QObject::tr("Modèle invalide");
        return false;
    }

    try
    {
        // Créer un fichier CSV (compatible Excel)
        QString csvPath = filePath;
        if (csvPath.endsWith(".xlsx", Qt::CaseInsensitive))
        {
            csvPath = csvPath.left(csvPath.length() - 5) + ".csv";
        }
        
        QFile file(csvPath);

        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            if (error)
                *error = QObject::tr("Impossible d'ouvrir le fichier Excel");
            return false;
        }

        QTextStream stream(&file);
        
         // BOM pour aider Excel à détecter l'UTF-8
        stream << QString::fromUtf8("\xEF\xBB\xBF");
        stream.setEncoding(QStringConverter::Utf8);
        
        const QString separator = ";";

        // En-têtes avec séparateur de colonnes (point-virgule pour Excel)
        for (int col = 0; col < model->columnCount(); ++col)
        {
            if (col > 0)
                stream << separator;
            QString header = model->headerData(col, Qt::Horizontal).toString();
            stream << "\"" << header << "\"";
        }
        stream << "\n";

        // Contenu
        for (int row = 0; row < model->rowCount(); ++row)
        {
            for (int col = 0; col < model->columnCount(); ++col)
            {
                if (col > 0)
                    stream << separator;
                const QVariant data = model->data(model->index(row, col), Qt::DisplayRole);
                
                // Échapper les guillemets et retours à la ligne
                QString value = data.toString();
                if (value.contains(separator) || value.contains("\"") || value.contains("\n"))
                {
                    value = "\"" + value.replace("\"", "\"\"") + "\"";
                }
                stream << value;
            }
            stream << "\n";
        }

        file.close();
        return true;
    }
    catch (const std::exception &e)
    {
        if (error)
            *error = QString::fromStdString(std::string("Erreur: ") + e.what());
        return false;
    }
}
