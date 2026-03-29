#pragma once

#include <QDate>
#include <QDateTime>
#include <QString>

class QAbstractItemModel;

class PdfExporter
{
public:
    static bool exportTableToPdf(const QString &filePath,
                                const QString &title,
                                const QAbstractItemModel *model,
                                QString *error = nullptr);

    static bool exportInvoiceToPdf(const QString &filePath,
                                  const QString &title,
                                  const QAbstractItemModel *model,
                                  int row,
                                  QString *error = nullptr);

    static bool exportDailyOccupationReportToPdf(const QString &filePath,
                                                const QString &title,
                                                const QAbstractItemModel *model,
                                                const QDate &day,
                                                QString *error = nullptr);

    static bool exportMonthlyRevenueByQuaiToPdf(const QString &filePath,
                                               const QString &title,
                                               const QAbstractItemModel *model,
                                               int year,
                                               int month,
                                               QString *error = nullptr);

    static bool exportAnomaliesReportToPdf(const QString &filePath,
                                          const QString &title,
                                          const QAbstractItemModel *model,
                                          const QDateTime &now,
                                          QString *error = nullptr);
};
