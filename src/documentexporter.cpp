#include "documentexporter.h"
#include <QFile>
#include <QTextStream>
#include <QAbstractTableModel>

bool DocumentExporter::exportTableToPdf(const QString &filePath,
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
        // Pour Qt 6 sans QtPrintSupport, exporter en CSV simple
        // (le CSV peut être ouvert dans Excel et imprimé comme PDF)
        QString csvPath = filePath;
        if (csvPath.endsWith(".pdf", Qt::CaseInsensitive))
        {
            csvPath = csvPath.left(csvPath.length() - 4) + ".csv";
        }
        
        QFile file(csvPath);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            if (error)
                *error = QObject::tr("Impossible d'ouvrir le fichier");
            return false;
        }

        QTextStream stream(&file);
        stream.setEncoding(QStringConverter::Utf8);

        // En-têtes
        for (int col = 0; col < model->columnCount(); ++col)
        {
            if (col > 0)
                stream << "\t";
            stream << model->headerData(col, Qt::Horizontal).toString();
        }
        stream << "\n";

        // Contenu
        for (int row = 0; row < model->rowCount(); ++row)
        {
            for (int col = 0; col < model->columnCount(); ++col)
            {
                if (col > 0)
                    stream << "\t";
                const QVariant data = model->data(model->index(row, col));
                stream << data.toString();
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

bool DocumentExporter::exportTableToExcel(const QString &filePath,
                                           const QString &title,
                                           const QSortFilterProxyModel *model,
                                           QString *error)
{
    Q_UNUSED(title);  // Paramètre non utilisé
    
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
        stream.setEncoding(QStringConverter::Utf8);

        // En-têtes avec séparateur de colonnes (point-virgule pour Excel)
        for (int col = 0; col < model->columnCount(); ++col)
        {
            if (col > 0)
                stream << ";";
            stream << model->headerData(col, Qt::Horizontal).toString();
        }
        stream << "\n";

        // Contenu
        for (int row = 0; row < model->rowCount(); ++row)
        {
            for (int col = 0; col < model->columnCount(); ++col)
            {
                if (col > 0)
                    stream << ";";
                const QVariant data = model->data(model->index(row, col));
                
                // Échapper les guillemets et retours à la ligne
                QString value = data.toString();
                if (value.contains(";") || value.contains("\"") || value.contains("\n"))
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
