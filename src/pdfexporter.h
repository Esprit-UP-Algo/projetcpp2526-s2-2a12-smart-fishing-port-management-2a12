#pragma once

#include <QString>

class QAbstractItemModel;

class PdfExporter
{
public:
    static bool exportTableToPdf(const QString &filePath,
                                const QString &title,
                                const QAbstractItemModel *model,
                                QString *error = nullptr);
};
