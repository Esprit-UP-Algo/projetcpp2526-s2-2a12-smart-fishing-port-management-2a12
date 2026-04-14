#ifndef TRANSACTIONCHATBOT_H
#define TRANSACTIONCHATBOT_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QMap>
#include "transaction.h"

class TransactionChatBot
{
public:
    // Fish price structure
    struct FishPriceInfo {
        double minPrice = 0.0;
        double maxPrice = 0.0;
        double avgPrice = 0.0;
    };
    
    explicit TransactionChatBot(const QVector<Transaction> &transactions, const QMap<QString, QString> &fishPrices);
    
    // Main chatbot method
    QString processMessage(const QString &userMessage);
    
    // Set fish prices from file
    void setFishPrices(const QMap<QString, FishPriceInfo> &prices);
    
    // Intent detection
    enum ChatIntent {
        INTENT_GREETING,
        INTENT_PRICE_RECOMMENDATION,
        INTENT_ANOMALY_CHECK,
        INTENT_SEARCH_TRANSACTION,
        INTENT_STATISTICS,
        INTENT_CREATE_TRANSACTION,
        INTENT_FISH_PRICE,
        INTENT_HELP,
        INTENT_UNKNOWN
    };
    
private:
    const QVector<Transaction> &m_transactions;
    const QMap<QString, QString> &m_fishPrices;
    QMap<QString, FishPriceInfo> m_fileFishPrices;  // Prices loaded from file
    
    // Intent detection
    ChatIntent detectIntent(const QString &message);
    
    // Response handlers
    QString handleGreeting();
    QString handlePriceRecommendation(const QString &message);
    QString handleAnomalyCheck(const QString &message);
    QString handleSearch(const QString &message);
    QString handleStatistics(const QString &message);
    QString handleCreateTransaction(const QString &message);
    QString handleFishPrice(const QString &message);
    QString handleHelp();
    QString handleUnknown();
    
    // Helper functions
    double estimateFishPrice(const QString &fishType);
    QVector<Transaction> findTransactionsByFish(const QString &fishType);
    QVector<Transaction> findTransactionsByFisherman(const QString &fishermanName);
    bool isAnomalous(const Transaction &t, double priceAvg, double priceStdDev);
    QString formatPrice(double price);
    QString formatTransactionInfo(const Transaction &t);
    QStringList suggestedQuestions();
};

#endif // TRANSACTIONCHATBOT_H
