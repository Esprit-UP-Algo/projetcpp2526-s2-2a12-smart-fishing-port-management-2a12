#pragma once

#include <QString>

class QAbstractItemModel;

namespace stockage {

class PdfExporter
{
public:
    static bool exportTableToPdf(const QString &filePath,
                                const QString &title,
                                const QAbstractItemModel *model,
                                QString *errorOut = nullptr);
};

} // namespace stockage
