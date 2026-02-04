#ifndef DOCUMENTEXPORTER_H
#define DOCUMENTEXPORTER_H

#include <QString>
#include <QSortFilterProxyModel>

class DocumentExporter
{
public:
    /**
     * Exporte les données de la table en PDF
     * @param filePath Chemin du fichier de sortie (*.pdf)
     * @param title Titre du document
     * @param model Modèle proxy contenant les données
     * @param error Pointeur pour stocker le message d'erreur (optionnel)
     * @return true en cas de succès, false sinon
     */
    static bool exportTableToPdf(const QString &filePath, 
                                 const QString &title,
                                 const QSortFilterProxyModel *model,
                                 QString *error = nullptr);

    /**
     * Exporte les données de la table en Excel
     * @param filePath Chemin du fichier de sortie (*.xlsx)
     * @param title Titre du document
     * @param model Modèle proxy contenant les données
     * @param error Pointeur pour stocker le message d'erreur (optionnel)
     * @return true en cas de succès, false sinon
     */
    static bool exportTableToExcel(const QString &filePath,
                                   const QString &title,
                                   const QSortFilterProxyModel *model,
                                   QString *error = nullptr);

private:
    DocumentExporter() = default;
};

#endif // DOCUMENTEXPORTER_H
