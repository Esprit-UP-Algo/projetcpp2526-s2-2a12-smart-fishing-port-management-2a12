#ifndef ASSISTANTDIALOG_H
#define ASSISTANTDIALOG_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include "transaction.h"

class AssistantDialog : public QObject
{
    Q_OBJECT

public:
    explicit AssistantDialog(QObject *parent = nullptr);
    
    // Start the conversation
    void startDetectionAssistant(const QVector<Transaction> &transactions);
    
    // Process user message
    QString processUserMessage(const QString &userMessage);
    
    // Get the initial greeting from the assistant
    QString getInitialGreeting() const;
    
    // Reset the conversation
    void reset();

private:
    enum ConversationState {
        GREETING,
        ASKING_DETAILS,
        ANALYZING,
        SHOWING_RESULTS,
        COMPLETED
    };
    
    QString analyzeTransactions();
    QString detectAnomalies();
    QStringList findAnomalousTransactions() const;
    
    ConversationState m_state;
    QVector<Transaction> m_transactions;
    int m_messageCount;
    QString m_currentAnalysis;
    QStringList m_detectedAnomalies;
    
    // User input tracking
    QString m_lastUserMessage;
    bool m_hasAnalyzed;
};

#endif // ASSISTANTDIALOG_H
