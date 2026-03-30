#include "fishpricingdialog.h"
#include "connection.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QCoreApplication>
#include <algorithm>

FishPricingDialog::FishPricingDialog(QObject *parent)
    : QObject(parent)
{
}

void FishPricingDialog::createDefaultPriceFileIfNeeded()
{
    QString filePath = QCoreApplication::applicationDirPath() + "/prix_poissons.txt";
    
    // Check if file already exists
    QFile file(filePath);
    if (file.exists()) {
        qDebug() << "Price file already exists at:" << filePath;
        return;
    }

    qDebug() << "Creating default price file at:" << filePath;

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Failed to create price file:" << filePath;
        emit errorOccurred("Impossible de créer le fichier de prix");
        return;
    }

    QTextStream out(&file);
    out.setEncoding(QStringConverter::Utf8);

    // Write price file content
    out << "LISTE DES PRIX DE POISSONS - ESTIMATION DE VENTE\n";
    out << "================================================\n\n";
    out << "Mise à jour: 28 Mars 2026\n";
    out << "Devise: TND (Dinar Tunisien)\n\n";
    out << "-------------------------------------------\n";
    out << "NOM DU POISSON         | PRIX ESTIMÉ (TND/kg)\n";
    out << "-------------------------------------------\n";
    out << "Sardine                | 8 - 10\n";
    out << "Anchois                | 7 - 9\n";
    out << "Maquereau              | 9 - 11\n";
    out << "Dorade                 | 12 - 16\n";
    out << "Rouget                 | 14 - 18\n";
    out << "Sole                   | 16 - 20\n";
    out << "Poulpe                 | 11 - 14\n";
    out << "Crevette               | 24 - 30\n";
    out << "Lieu                   | 10 - 13\n";
    out << "Encornet               | 12 - 16\n";
    out << "Merlan                 | 8 - 11\n";
    out << "Cabillaud              | 10 - 14\n";
    out << "Trout                  | 13 - 17\n";
    out << "Turbot                 | 18 - 24\n";
    out << "-------------------------------------------\n\n";
    out << "NOTES:\n";
    out << "------\n";
    out << "- Les prix varient en fonction de la qualité et de la taille\n";
    out << "- Les prix sont basés sur les transactions historiques\n";
    out << "- Pour les prix exacts, consultez la base de données\n";
    out << "- Mise à jour régulière des tarifs recommandée\n";

    file.close();
    qDebug() << "Default price file created successfully";
}

void FishPricingDialog::loadFishPrices()
{
    m_fishPrices.clear();

    QSqlDatabase db = Connection::createInstance().database();
    if (!db.isOpen()) {
        emit errorOccurred("Database not connected");
        return;
    }

    // First, let's debug what's in the LOT column
    QString testQuery = "SELECT DISTINCT LOT FROM source_projet2A WHERE LOT IS NOT NULL AND LOT != '' ORDER BY LOT";
    QSqlQuery testQ(db);
    qDebug() << "\n===== DEBUG: CHECKING LOT VALUES =====";
    if (testQ.exec(testQuery)) {
        int count = 0;
        while (testQ.next()) {
            QString lotValue = testQ.value(0).toString();
            qDebug() << "  LOT VALUE:" << lotValue << "| Length:" << lotValue.length() << "| Trimmed:" << lotValue.trimmed();
            count++;
        }
        qDebug() << "  Total LOT values found:" << count;
    } else {
        qDebug() << "  Query failed:" << testQ.lastError().text();
    }
    qDebug() << "===== END DEBUG =====\n";

    // Query to get all transactions and extract fish types from LOT column
    QString query = "SELECT LOT AS fish_type, "
                    "       AVG(PRIX_KG) AS avg_price, "
                    "       MIN(PRIX_KG) AS min_price, "
                    "       MAX(PRIX_KG) AS max_price, "
                    "       COUNT(*) AS count_records "
                    "FROM source_projet2A "
                    "WHERE LOT IS NOT NULL AND LOT != '' "
                    "GROUP BY LOT "
                    "ORDER BY LOT";

    QSqlQuery q(db);
    if (!q.exec(query)) {
        qDebug() << "Query failed:" << q.lastError().text();
        emit errorOccurred("Failed to load fish prices: " + q.lastError().text());
        return;
    }

    int loadedCount = 0;
    while (q.next()) {
        FishPrice fp;
        fp.name = q.value("fish_type").toString().trimmed();
        fp.averagePrice = q.value("avg_price").toDouble();
        fp.minPrice = q.value("min_price").toDouble();
        fp.maxPrice = q.value("max_price").toDouble();
        fp.count = q.value("count_records").toInt();

        if (!fp.name.isEmpty()) {
            m_fishPrices.insert(fp.name, fp);
            qDebug() << "[FISH LOADED]" << fp.name 
                     << "| Avg:" << fp.averagePrice 
                     << "| Min:" << fp.minPrice 
                     << "| Max:" << fp.maxPrice 
                     << "| Count:" << fp.count;
            loadedCount++;
        }
    }

    qDebug() << "===== TOTAL FISH TYPES LOADED:" << loadedCount << "=====";
    if (m_fishPrices.isEmpty()) {
        qDebug() << "WARNING: NO FISH LOADED FROM DATABASE!";
    }
    for (auto it = m_fishPrices.constBegin(); it != m_fishPrices.constEnd(); ++it) {
        qDebug() << "    -" << it.key();
    }
    qDebug() << "=====\n";
    emit fishListLoaded(getAvailableFishes());
}

void FishPricingDialog::loadFishPricesFromFile(const QString &filePath)
{
    m_fishPrices.clear();

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file:" << filePath;
        emit errorOccurred("Impossible d'ouvrir le fichier: " + filePath);
        return;
    }

    QTextStream in(&file);
    in.setEncoding(QStringConverter::Utf8);

    int loadedCount = 0;
    qDebug() << "\n===== LOADING FISH PRICES FROM FILE =====";
    qDebug() << "File:" << filePath;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        // Skip empty lines and header lines
        if (line.isEmpty() || line.startsWith("-") || line.startsWith("LISTE") || 
            line.startsWith("Mise") || line.startsWith("Devise") || line.startsWith("NOM") ||
            line.startsWith("NOTES") || line.startsWith("Les prix") || 
            line.startsWith("Pour les") || line.startsWith("Mise à jour")) {
            continue;
        }

        // Parse lines with fish data (format: "NOM | PRIX_MIN - PRIX_MAX")
        if (line.contains("|")) {
            QStringList parts = line.split("|");
            if (parts.size() >= 2) {
                QString fishName = parts[0].trimmed();
                QString priceStr = parts[1].trimmed();

                // Skip header and separator lines
                if (fishName.startsWith("=") || fishName == "NOM DU POISSON") {
                    continue;
                }

                // Parse price range (e.g., "400 - 500")
                QStringList priceParts = priceStr.split("-");
                if (priceParts.size() >= 2) {
                    bool minOk = false, maxOk = false;
                    double minPrice = priceParts[0].trimmed().toDouble(&minOk);
                    double maxPrice = priceParts[1].trimmed().toDouble(&maxOk);

                    if (minOk && maxOk && !fishName.isEmpty()) {
                        FishPrice fp;
                        fp.name = fishName;
                        fp.minPrice = minPrice;
                        fp.maxPrice = maxPrice;
                        fp.averagePrice = (minPrice + maxPrice) / 2.0;
                        fp.count = 1; // From file, we don't have transaction count

                        m_fishPrices.insert(fp.name, fp);
                        qDebug() << "[FISH LOADED]" << fp.name 
                                 << "| Avg:" << fp.averagePrice 
                                 << "| Min:" << fp.minPrice 
                                 << "| Max:" << fp.maxPrice;
                        loadedCount++;
                    }
                }
            }
        }
    }

    file.close();

    qDebug() << "===== TOTAL FISH TYPES LOADED FROM FILE:" << loadedCount << "=====";
    if (m_fishPrices.isEmpty()) {
        qDebug() << "WARNING: NO FISH LOADED FROM FILE!";
        emit errorOccurred("Aucun poisson trouvé dans le fichier");
    } else {
        for (auto it = m_fishPrices.constBegin(); it != m_fishPrices.constEnd(); ++it) {
            qDebug() << "    -" << it.key();
        }
    }
    qDebug() << "=====\n";
    emit fishListLoaded(getAvailableFishes());
}

double FishPricingDialog::estimatePrice(const QString &fishName)
{
    QString key = fishName.trimmed();

    // Try exact match first
    if (m_fishPrices.contains(key)) {
        return m_fishPrices[key].averagePrice;
    }

    // Try case-insensitive match
    for (auto it = m_fishPrices.constBegin(); it != m_fishPrices.constEnd(); ++it) {
        if (it.key().compare(key, Qt::CaseInsensitive) == 0) {
            return it.value().averagePrice;
        }
    }

    // Try partial match (substring)
    for (auto it = m_fishPrices.constBegin(); it != m_fishPrices.constEnd(); ++it) {
        if (it.key().contains(key, Qt::CaseInsensitive) ||
            key.contains(it.key(), Qt::CaseInsensitive)) {
            return it.value().averagePrice;
        }
    }

    emit errorOccurred("Fish type not found: " + fishName);
    return -1.0;
}

QStringList FishPricingDialog::getAvailableFishes() const
{
    return m_fishPrices.keys();
}

FishPrice FishPricingDialog::getFishInfo(const QString &fishName) const
{
    QString key = fishName.trimmed();

    qDebug() << "Looking for fish:" << key;
    qDebug() << "Available fishes in map:" << m_fishPrices.keys();

    // Try exact match first
    if (m_fishPrices.contains(key)) {
        qDebug() << "Found exact match for:" << key;
        return m_fishPrices[key];
    }

    // Try case-insensitive match
    for (auto it = m_fishPrices.constBegin(); it != m_fishPrices.constEnd(); ++it) {
        if (it.key().compare(key, Qt::CaseInsensitive) == 0) {
            qDebug() << "Found case-insensitive match:" << it.key();
            return it.value();
        }
    }

    // Try partial match (substring)
    for (auto it = m_fishPrices.constBegin(); it != m_fishPrices.constEnd(); ++it) {
        if (it.key().contains(key, Qt::CaseInsensitive) ||
            key.contains(it.key(), Qt::CaseInsensitive)) {
            qDebug() << "Found partial match:" << it.key();
            return it.value();
        }
    }

    // Return empty result if not found
    qDebug() << "Fish not found in map:" << key;
    return FishPrice{key, -1.0, -1.0, -1.0, 0};
}
