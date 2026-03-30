#ifndef FISHPRICINGDIALOG_H
#define FISHPRICINGDIALOG_H

#include <QObject>
#include <QString>
#include <QMap>
#include <QVector>

struct FishPrice {
    QString name;
    double averagePrice;
    double minPrice;
    double maxPrice;
    int count;
};

class FishPricingDialog : public QObject
{
    Q_OBJECT

public:
    explicit FishPricingDialog(QObject *parent = nullptr);

    // Create default price file if it doesn't exist
    void createDefaultPriceFileIfNeeded();

    // Load all fish prices from database
    void loadFishPrices();

    // Load fish prices from a text file
    void loadFishPricesFromFile(const QString &filePath);

    // Get estimated price for a fish name
    double estimatePrice(const QString &fishName);

    // Get available fish names
    QStringList getAvailableFishes() const;

    // Get detailed info about a fish
    FishPrice getFishInfo(const QString &fishName) const;

signals:
    void priceEstimated(const QString &fishName, double price);
    void fishListLoaded(const QStringList &fishes);
    void errorOccurred(const QString &error);

private:
    QMap<QString, FishPrice> m_fishPrices;
};

#endif // FISHPRICINGDIALOG_H
