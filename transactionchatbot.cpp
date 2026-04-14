#include "transactionchatbot.h"
#include <QStringList>
#include <QRegularExpression>
#include <cmath>
#include <limits>
#include <algorithm>
#include <QDebug>

TransactionChatBot::TransactionChatBot(const QVector<Transaction> &transactions, const QMap<QString, QString> &fishPrices)
    : m_transactions(transactions), m_fishPrices(fishPrices)
{
}

void TransactionChatBot::setFishPrices(const QMap<QString, FishPriceInfo> &prices)
{
    m_fileFishPrices = prices;
    qDebug() << "Chatbot: Loaded" << prices.size() << "fish prices from file";
}

QString TransactionChatBot::processMessage(const QString &userMessage)
{
    if (userMessage.trimmed().isEmpty()) {
        return "Veuillez entrer une question ou une commande.";
    }
    
    ChatIntent intent = detectIntent(userMessage);
    
    switch (intent) {
    case INTENT_GREETING:
        return handleGreeting();
    case INTENT_PRICE_RECOMMENDATION:
        return handlePriceRecommendation(userMessage);
    case INTENT_ANOMALY_CHECK:
        return handleAnomalyCheck(userMessage);
    case INTENT_SEARCH_TRANSACTION:
        return handleSearch(userMessage);
    case INTENT_STATISTICS:
        return handleStatistics(userMessage);
    case INTENT_CREATE_TRANSACTION:
        return handleCreateTransaction(userMessage);
    case INTENT_FISH_PRICE:
        return handleFishPrice(userMessage);
    case INTENT_HELP:
        return handleHelp();
    default:
        return handleUnknown();
    }
}

TransactionChatBot::ChatIntent TransactionChatBot::detectIntent(const QString &message)
{
    QString msg = message.toLower();
    
    // Greeting
    if (msg.contains("bonjour") || msg.contains("hello") || msg.contains("salut") || msg.contains("coucou")) {
        return INTENT_GREETING;
    }
    
    // Fish price (specific fish type) - BEFORE other checks
    QStringList fishTypes = {"sardine", "anchois", "maquereau", "dorade", "daurade", "dourade", 
                             "rouget", "sole", "poulpe", "crevette", "lieu", "encornet", 
                             "merlan", "cabillaud", "trout", "turbot", "hareng", "moule",
                             "bar", "basse", "seiche", "calmar"};
    for (const QString &fish : fishTypes) {
        if (msg.contains(fish)) {
            if (msg.contains("prix") || msg.contains("quel") || msg.contains("combien") || msg.contains("coût") || msg.contains("combat")) {
                return INTENT_FISH_PRICE;
            }
        }
    }
    
    // Price recommendation
    if (msg.contains("prix") && (msg.contains("recommande") || msg.contains("suggère") || msg.contains("propose") || msg.contains("estimation"))) {
        return INTENT_PRICE_RECOMMENDATION;
    }
    if ((msg.contains("quel prix") || msg.contains("quel est le prix") || msg.contains("combien")) &&
        (msg.contains("recommande") || msg.contains("suggère") || msg.contains("propose"))) {
        return INTENT_PRICE_RECOMMENDATION;
    }
    
    // Anomaly check
    if (msg.contains("anomal") || msg.contains("anormal") || msg.contains("douteux") || msg.contains("bizarre")) {
        return INTENT_ANOMALY_CHECK;
    }
    if ((msg.contains("cette transaction") || msg.contains("cette vente")) && msg.contains("normal")) {
        return INTENT_ANOMALY_CHECK;
    }
    
    // Search
    if (msg.contains("cherch") || msg.contains("recherch") || msg.contains("trouv") || msg.contains("montre")) {
        if (msg.contains("transaction") || msg.contains("vente")) {
            return INTENT_SEARCH_TRANSACTION;
        }
    }
    if (msg.contains("transaction") && (msg.contains("par") || msg.contains("de") || msg.contains("pour"))) {
        return INTENT_SEARCH_TRANSACTION;
    }
    
    // Statistics
    if (msg.contains("statistique") || msg.contains("analys") || msg.contains("tendance") || msg.contains("résumé")) {
        return INTENT_STATISTICS;
    }
    if (msg.contains("moyen") || msg.contains("moyenne") || msg.contains("total")) {
        return INTENT_STATISTICS;
    }
    
    // Create transaction
    if (msg.contains("créer") || msg.contains("ajouter") || msg.contains("nouvelle")) {
        if (msg.contains("transaction") || msg.contains("vente")) {
            return INTENT_CREATE_TRANSACTION;
        }
    }
    
    // Help
    if (msg.contains("aide") || msg.contains("help") || msg.contains("que peux") || msg.contains("quoi faire")) {
        return INTENT_HELP;
    }
    
    return INTENT_UNKNOWN;
}

QString TransactionChatBot::handleGreeting()
{
    return "Bonjour! 👋 Je suis votre assistant pour la gestion des transactions de poisson.\n\n"
           "Je peux vous aider à:\n"
           "• 💰 Recommander un prix pour une transaction\n"
           "• ⚠️ Détecter les transactions anormales\n"
           "• 🔍 Rechercher des transactions\n"
           "• 📊 Analyser vos ventes\n"
           "• 🐟 Consulter les prix des poissons\n\n"
           "Que puis-je faire pour vous?";
}

QString TransactionChatBot::handlePriceRecommendation(const QString &message)
{
    QString msg = message.toLower();
    
    // Extract fish type from message
    QString fishType;
    QStringList fishTypes = {"sardine", "crevette", "dorade", "hareng", "moule", "anchois", "lieu", "rouget", "merlan", "plie"};
    
    for (const QString &fish : fishTypes) {
        if (msg.contains(fish)) {
            fishType = fish;
            break;
        }
    }
    
    if (fishType.isEmpty()) {
        return "Pour recommander un prix, veuillez préciser le type de poisson. "
               "Exemples: sardine, crevette, dorade, hareng, moule, anchois, lieu, rouget, merlan, plie.";
    }
    
    double estimatedPrice = estimateFishPrice(fishType);
    
    if (estimatedPrice <= 0) {
        return QString("Je n'ai pas d'historique de prix pour '%1'. Vérifiez l'orthographe du type de poisson.").arg(fishType);
    }
    
    // Find similar transactions and calculate statistics
    QVector<Transaction> similarTransactions = findTransactionsByFish(fishType);
    
    if (similarTransactions.isEmpty()) {
        return QString("Prix recommandé pour %1: <b>%2 TND/kg</b>\n\n"
                      "Note: C'est une estimation basée sur les prixde référence.")
                .arg(fishType, formatPrice(estimatedPrice));
    }
    
    // Calculate average and standard deviation
    double sum = 0, sumSq = 0;
    for (const Transaction &t : similarTransactions) {
        sum += t.prixKg;
        sumSq += t.prixKg * t.prixKg;
    }
    double avg = sum / similarTransactions.size();
    double variance = (sumSq / similarTransactions.size()) - (avg * avg);
    double stdDev = std::sqrt(std::max(0.0, variance));
    
    return QString("💰 <b>Prix Recommandé pour %1</b>\n\n"
                  "• Estimation: <b>%2 TND/kg</b>\n"
                  "• Moyenne historique: %3 TND/kg\n"
                  "• Écart-type: %4 TND/kg\n"
                  "• Nombre de ventes: %5\n\n"
                  "Basé sur %6 transactions similaires.")
            .arg(fishType)
            .arg(formatPrice(estimatedPrice))
            .arg(formatPrice(avg))
            .arg(formatPrice(stdDev))
            .arg(similarTransactions.size())
            .arg(similarTransactions.size());
}

QString TransactionChatBot::handleAnomalyCheck(const QString &message)
{
    // Anomaly detection: Compare actual price to estimated price
    // Threshold: 2 DT difference
    const double PRICE_THRESHOLD = 2.0;
    
    QString result = "⚠️ <b>Détection d'Anomalie - Prix/kilo</b>\n\n";
    
    if (m_transactions.isEmpty()) {
        return result + "Aucune transaction disponible pour l'analyse.";
    }
    
    // Calculate price statistics by fish type
    QMap<QString, QVector<double>> fishPrices;
    for (const Transaction &t : m_transactions) {
        QString fish = t.reference.toLower();  // Use reference (fish type) for grouping
        if (!fish.isEmpty()) {
            fishPrices[fish].append(t.prixKg);
        }
    }
    
    // Analyze the most recent transaction
    Transaction recent = m_transactions.last();
    QString fishKey = recent.reference.toLower();
    
    result += QString("📊 <b>Transaction Analysée:</b>\n"
                     "• Pêcheur: %1\n"
                     "• Type: %2\n"
                     "• Prix réel: <b>%3 TND/kg</b>\n"
                     "• Quantité: %4 kg\n"
                     "• Date: %5\n\n")
            .arg(recent.pecheur)
            .arg(recent.reference)
            .arg(QString::number(recent.prixKg, 'f', 2))
            .arg(QString::number(recent.quantite, 'f', 2))
            .arg(recent.dateTransaction.toString("dd/MM/yyyy"));
    
    if (fishPrices.contains(fishKey) && fishPrices[fishKey].size() > 1) {
        // Calculate average price (estimated price)
        QVector<double> prices = fishPrices[fishKey];
        double sum = 0;
        
        for (double price : prices) {
            sum += price;
        }
        
        double estimatedPrice = sum / prices.size();
        double minPrice = *std::min_element(prices.begin(), prices.end());
        double maxPrice = *std::max_element(prices.begin(), prices.end());
        
        // Calculate difference from estimated price
        double priceDifference = recent.prixKg - estimatedPrice;
        double absDifference = std::abs(priceDifference);
        
        result += QString("📈 <b>Comparaison avec l'Historique (%1):</b>\n"
                         "• Prix estimé (moyenne): <b>%2 TND/kg</b>\n"
                         "• Prix réel: <b>%3 TND/kg</b>\n"
                         "• Différence: <b>%4 TND/kg</b>\n"
                         "• Fourchette historique: %5 - %6 TND/kg\n"
                         "• Nombre de transactions: %7\n\n")
                .arg(recent.reference)
                .arg(QString::number(estimatedPrice, 'f', 2))
                .arg(QString::number(recent.prixKg, 'f', 2))
                .arg(QString::number(priceDifference, 'f', 2))
                .arg(QString::number(minPrice, 'f', 2))
                .arg(QString::number(maxPrice, 'f', 2))
                .arg(prices.size());
        
        // Check if anomaly: difference > 2 DT threshold
        if (absDifference > PRICE_THRESHOLD) {
            result += QString("<span style='color: #ff6b6b;'><b>🚨 ANOMALIE DÉTECTÉE!</b></span>\n"
                             "Différence de prix: <b>%1 TND/kg</b> (seuil: %2 DT)\n\n")
                    .arg(QString::number(absDifference, 'f', 2))
                    .arg(QString::number(PRICE_THRESHOLD, 'f', 2));
            
            if (priceDifference > 0) {
                result += QString("⬆️ <b>PRIX TROP ÉLEVÉ!</b>\n"
                                 "Ce prix dépasse de %1 DT le prix estimé.\n"
                                 "Vérifiez si c'est justifié (qualité spéciale, urgence, etc.)")
                        .arg(QString::number(absDifference, 'f', 2));
            } else {
                result += QString("⬇️ <b>PRIX TROP BAS!</b>\n"
                                 "Ce prix est inférieur de %1 DT au prix estimé.\n"
                                 "Vérifiez si la quantité ou la qualité est conforme.")
                        .arg(QString::number(absDifference, 'f', 2));
            }
        } else {
            result += QString("<span style='color: #51cf66;'><b>✓ PRIX NORMAL</b></span>\n"
                             "Différence: <b>%1 TND/kg</b> (acceptable, seuil: %2 DT)\n"
                             "Cette transaction est conforme aux prix du marché.")
                    .arg(QString::number(absDifference, 'f', 2))
                    .arg(QString::number(PRICE_THRESHOLD, 'f', 2));
        }
    } else {
        result += "Données insuffisantes pour comparer avec l'historique.";
    }
    
    return result;
}

QString TransactionChatBot::handleSearch(const QString &message)
{
    QString msg = message.toLower();
    QString result = "🔍 <b>Résultats de Recherche</b>\n\n";
    
    // Try to extract fisherman name or fish type
    QStringList tokens = msg.split(" ");
    QVector<Transaction> results;
    
    // Search by fish type
    for (const QString &token : tokens) {
        QVector<Transaction> byFish = findTransactionsByFish(token);
        if (!byFish.isEmpty()) {
            results = byFish;
            break;
        }
    }
    
    // If no results, try to search by general pattern
    if (results.isEmpty()) {
        // Return recent transactions
        int startIndex = m_transactions.size() > 5 ? m_transactions.size() - 5 : 0;
        for (int i = startIndex; i < m_transactions.size(); ++i) {
            results.append(m_transactions[i]);
        }
        result += "Dernières 5 transactions:\n\n";
    } else {
        result += QString("Trouvé %1 transactions:\n\n").arg(results.size());
    }
    
    int maxResults = results.size() > 5 ? 5 : results.size();
    for (int i = 0; i < maxResults; ++i) {
        result += formatTransactionInfo(results[i]) + "\n";
    }
    
    if (results.size() > 5) {
        result += QString("\n... et %1 autres transactions").arg(results.size() - 5);
    }
    
    return result;
}

QString TransactionChatBot::handleStatistics(const QString &message)
{
    if (m_transactions.isEmpty()) {
        return "Aucune transaction disponible pour l'analyse.";
    }
    
    QString result = "📊 <b>Statistiques des Ventes</b>\n\n";
    
    double totalQuantity = 0, totalRevenue = 0, minPrice = 1e9, maxPrice = 0;
    QMap<QString, int> fishCount;
    QMap<QString, double> fishRevenue;
    
    for (const Transaction &t : m_transactions) {
        totalQuantity += t.quantite;
        totalRevenue += t.total;
        minPrice = std::min(minPrice, t.prixKg);
        maxPrice = std::max(maxPrice, t.prixKg);
        fishCount[t.lot]++;
        fishRevenue[t.lot] += t.total;
    }
    
    result += QString("• Nombre de transactions: %1\n"
                     "• Quantité totale: %2 kg\n"
                     "• Revenu total: %3 TND\n"
                     "• Prix min: %4 TND/kg\n"
                     "• Prix max: %5 TND/kg\n"
                     "• Prix moyen: %6 TND/kg\n\n")
            .arg(m_transactions.size())
            .arg(formatPrice(totalQuantity))
            .arg(formatPrice(totalRevenue))
            .arg(formatPrice(minPrice))
            .arg(formatPrice(maxPrice))
            .arg(formatPrice(totalRevenue / totalQuantity));
    
    result += "<b>Top poissons par revenu:</b>\n";
    result += "┌─────────────────┬──────────┐\n";
    
    QStringList fishList = fishRevenue.keys();
    std::sort(fishList.begin(), fishList.end(), [&](const QString &a, const QString &b) {
        return fishRevenue[a] > fishRevenue[b];
    });
    
    int maxFish = fishList.size() > 3 ? 3 : fishList.size();
    for (int i = 0; i < maxFish; ++i) {
        result += QString("│ %1 │ %2 TND │\n")
                .arg(fishList[i].leftJustified(15))
                .arg(QString::number(fishRevenue[fishList[i]], 'f', 0).rightJustified(8));
    }
    result += "└─────────────────┴──────────┘\n";
    
    return result;
}

QString TransactionChatBot::handleCreateTransaction(const QString &message)
{
    return "➕ <b>Créer une Transaction</b>\n\n"
           "Pour créer une transaction, format recommandé:\n"
           "<code>Créer: [Pêcheur] [Poisson] [Quantité]kg à [Prix]TND/kg</code>\n\n"
           "Exemple:\n"
           "\"Créer: Ahmed Sardine 50kg à 9TND/kg\"\n\n"
           "Ou utilisez le bouton 'Créer' dans l'interface pour un formulaire complet.";
}

QString TransactionChatBot::handleFishPrice(const QString &message)
{
    QString msg = message.toLower();
    
    // Extended fish types with variations
    QStringList fishTypes = {"sardine", "anchois", "maquereau", "dorade", "daurade", "dourade", 
                             "rouget", "sole", "poulpe", "crevette", "lieu", "encornet", 
                             "merlan", "cabillaud", "trout", "turbot", "hareng", "moule",
                             "bar", "basse", "seiche", "calmar", "saumon", "esturgeon",
                             "tilapia", "carpe", "broche", "perche", "truite", "flet",
                             "baudroie", "homard", "langouste", "sepia", "mulet", "pageot"};
    
    // Fish type aliases (normalize variations to standard names)
    QMap<QString, QString> fishAliases;
    fishAliases["dourade"] = "dorade";
    fishAliases["daurade"] = "dorade";
    
    QString targetFish;
    
    // Find which fish type is mentioned
    for (const QString &fish : fishTypes) {
        if (msg.contains(fish)) {
            // Normalize if it's an alias
            if (fishAliases.contains(fish)) {
                targetFish = fishAliases[fish];
            } else {
                targetFish = fish;
            }
            break;
        }
    }
    
    if (targetFish.isEmpty()) {
        return "🐟 <b>Prix des Poissons</b>\n\n"
               "Quel poisson vous intéresse?\n"
               "Types disponibles: sardine, anchois, maquereau, dorade, crevette, etc.";
    }
    
    // First, try to get price from file
    if (m_fileFishPrices.contains(targetFish)) {
        FishPriceInfo info = m_fileFishPrices[targetFish];
        QString capitalFish = targetFish.at(0).toUpper() + targetFish.mid(1);
        
        return QString("🐟 <b>Prix Estimé - %1</b>\n\n")
               .arg(capitalFish)
               + QString("<b>Prix minimum:</b> %1 DZD/kg\n")
               .arg(QString::number(info.minPrice, 'f', 0))
               + QString("<b>Prix moyen:</b> %1 DZD/kg\n")
               .arg(QString::number(info.avgPrice, 'f', 0))
               + QString("<b>Prix maximum:</b> %1 DZD/kg\n\n")
               .arg(QString::number(info.maxPrice, 'f', 0))
               + "<b>Source:</b> Tarif de référence (fichier poissons.txt)";
    }
    
    // If not in file, fall back to historical prices
    QVector<Transaction> fishTransactions = findTransactionsByFish(targetFish);
    
    if (fishTransactions.isEmpty()) {
        // Try with the original name from the message
        QString capitalFish = targetFish.at(0).toUpper() + targetFish.mid(1);
        return QString("🐟 <b>%1</b>\n\nAucune transaction trouvée pour ce type de poisson.\n\n"
                      "Vérifiez l'orthographe ou essayez un autre poisson.")
               .arg(capitalFish);
    }
    
    // Calculate average, min, max prices from transactions
    double totalPrice = 0;
    double minPrice = std::numeric_limits<double>::max();
    double maxPrice = 0;
    
    for (const auto &t : fishTransactions) {
        double pricePerKg = (t.quantite > 0) ? (t.prixKg) : 0;
        if (pricePerKg > 0) {
            totalPrice += pricePerKg;
            minPrice = std::min(minPrice, pricePerKg);
            maxPrice = std::max(maxPrice, pricePerKg);
        }
    }
    
    double avgPrice = totalPrice / fishTransactions.size();
    if (minPrice == std::numeric_limits<double>::max()) {
        minPrice = 0;
    }
    
    QString capitalFish = targetFish.at(0).toUpper() + targetFish.mid(1);
    
    return QString("🐟 <b>Prix Estimé - %1</b>\n\n")
           .arg(capitalFish)
           + QString("<b>Prix moyen:</b> %1 TND/kg\n")
           .arg(QString::number(avgPrice, 'f', 2))
           + QString("<b>Fourchette:</b> %1 - %2 TND/kg\n")
           .arg(QString::number(minPrice, 'f', 2), QString::number(maxPrice, 'f', 2))
           + QString("<b>Basé sur:</b> %1 transactions\n\n")
           .arg(QString::number(fishTransactions.size()))
           + "Ceci est une estimation basée sur l'historique des transactions.";
}

QString TransactionChatBot::handleHelp()
{
    return "❓ <b>Aide - Comment utiliser le Chatbot</b>\n\n"
           "<b>Recommandations de Prix:</b>\n"
           "\"Quel prix pour une sardine?\"\n"
           "\"Recommande un prix pour la crevette\"\n\n"
           "<b>Détection d'Anomalies:</b>\n"
           "\"Cette transaction est-elle anormale?\"\n"
           "\"Y a-t-il des éléments bizarres?\"\n\n"
           "<b>Recherche:</b>\n"
           "\"Cherche les transactions de sardine\"\n"
           "\"Montre les ventes d'Ahmed\"\n\n"
           "<b>Statistiques:</b>\n"
           "\"Quelle est la moyenne?\"\n"
           "\"Analyse mes ventes\"\n\n"
           "<b>Prix des Poissons:</b>\n"
           "\"Quel est le prix de la moule?\"\n"
           "\"Prix des crevettes?\"\n\n"
           "<b>Création:</b>\n"
           "\"Créer une transaction\"\n"
           "\"Ajouter une vente\"";
}

QString TransactionChatBot::handleUnknown()
{
    QStringList responses = {
        "Je n'ai pas bien compris. Pouvez-vous reformuler?\n\n"
        "Dites-moi ce que vous voulez faire:\n"
        "• Recommander un prix\n"
        "• Analyser pour anomalies\n"
        "• Rechercher des transactions\n"
        "• Voir les statistiques",
        
        "Je ne suis pas sûr de comprendre. Utilisez l'une de ces commandes:\n"
        "\"Quel prix pour [poisson]?\"\n"
        "\"Y a-t-il des anomalies?\"\n"
        "\"Montre les transactions\"\n"
        "\"Statistiques\"",
        
        "Parlez-moi d'un prix, d'une anomalie, d'une recherche ou des statistiques!"
    };
    
    return responses[rand() % responses.size()];
}

double TransactionChatBot::estimateFishPrice(const QString &fishType)
{
    for (const auto &fish : m_fishPrices.keys()) {
        if (fish.toLower().contains(fishType.toLower()) || fishType.toLower().contains(fish.toLower())) {
            QString priceStr = m_fishPrices[fish];
            QStringList parts = priceStr.split("-");
            if (parts.size() >= 2) {
                bool ok1, ok2;
                double min = parts[0].trimmed().toDouble(&ok1);
                double max = parts[1].trimmed().toDouble(&ok2);
                if (ok1 && ok2) {
                    return (min + max) / 2.0;
                }
            }
        }
    }
    return -1;
}

QVector<Transaction> TransactionChatBot::findTransactionsByFish(const QString &fishType)
{
    QVector<Transaction> result;
    QString searchTerm = fishType.toLower();
    
    for (const Transaction &t : m_transactions) {
        // Search in both reference and lot fields
        if (t.reference.toLower().contains(searchTerm) || 
            t.lot.toLower().contains(searchTerm)) {
            result.append(t);
        }
    }
    return result;
}

QVector<Transaction> TransactionChatBot::findTransactionsByFisherman(const QString &fishermanName)
{
    QVector<Transaction> result;
    for (const Transaction &t : m_transactions) {
        if (t.pecheur.toLower().contains(fishermanName.toLower())) {
            result.append(t);
        }
    }
    return result;
}

bool TransactionChatBot::isAnomalous(const Transaction &t, double priceAvg, double priceStdDev)
{
    if (priceStdDev == 0) return false;
    double zScore = (t.prixKg - priceAvg) / priceStdDev;
    return std::abs(zScore) > 2;
}

QString TransactionChatBot::formatPrice(double price)
{
    return QString::number(price, 'f', 2);
}

QString TransactionChatBot::formatTransactionInfo(const Transaction &t)
{
    return QString("• %1 %2 | %3 kg @ %4 TND/kg = %5 TND [%6]")
            .arg(t.dateTransaction.toString("dd/MM"), t.lot.leftJustified(10))
            .arg(QString::number(t.quantite, 'f', 1))
            .arg(formatPrice(t.prixKg))
            .arg(formatPrice(t.total))
            .arg(t.pecheur);
}

QStringList TransactionChatBot::suggestedQuestions()
{
    return {
        "Quel prix pour une sardine?",
        "Y a-t-il des anomalies?",
        "Montre les statistiques",
        "Cherche les transactions",
        "Prix de la crevette?"
    };
}
