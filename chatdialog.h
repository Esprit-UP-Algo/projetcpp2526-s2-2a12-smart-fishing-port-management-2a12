#ifndef CHATDIALOG_H
#define CHATDIALOG_H

#include <QDialog>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QVector>
#include "transaction.h"
#include "transactionchatbot.h"

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ChatDialog(const QVector<Transaction> &transactions, 
                       const QMap<QString, QString> &fishPrices,
                       QWidget *parent = nullptr);
    ~ChatDialog();

private slots:
    void onSendMessage();
    void onSuggestedQuestionClicked();
    void onClearChat();

private:
    void setupUI();
    void styleUI();
    void addMessageToChat(const QString &message, bool isUser);
    void displayInitialGreeting();
    void setupSuggestedQuestions();
    
    // UI Components
    QTextEdit *m_chatDisplay;
    QLineEdit *m_inputField;
    QPushButton *m_sendButton;
    QPushButton *m_clearButton;
    QHBoxLayout *m_suggestedQuestionsLayout;
    
    // Chatbot
    TransactionChatBot *m_chatBot;
    
    // Style
    QString m_userMessageStyle;
    QString m_assistantMessageStyle;
};

#endif // CHATDIALOG_H
