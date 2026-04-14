#include "chatdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QDateTime>
#include <QDebug>

ChatDialog::ChatDialog(const QVector<Transaction> &transactions,
                       const QMap<QString, QString> &fishPrices,
                       QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Assistant Transactions - Chatbot Métier");
    setGeometry(100, 100, 900, 700);
    
    // Create chatbot
    m_chatBot = new TransactionChatBot(transactions, fishPrices);
    
    setupUI();
    styleUI();
    displayInitialGreeting();
    
    // Connect signals
    connect(m_sendButton, &QPushButton::clicked, this, &ChatDialog::onSendMessage);
    connect(m_inputField, &QLineEdit::returnPressed, this, &ChatDialog::onSendMessage);
    connect(m_clearButton, &QPushButton::clicked, this, &ChatDialog::onClearChat);
}

ChatDialog::~ChatDialog()
{
    delete m_chatBot;
}

void ChatDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);
    
    // Title
    QLabel *titleLabel = new QLabel("🤖 Assistant Métier - Transactions de Poisson");
    titleLabel->setStyleSheet(
        "QLabel {"
        "  font-size: 15px;"
        "  font-weight: bold;"
        "  color: #00bcd4;"
        "  padding-bottom: 10px;"
        "}"
    );
    mainLayout->addWidget(titleLabel);
    
    // Chat display area
    m_chatDisplay = new QTextEdit();
    m_chatDisplay->setReadOnly(true);
    m_chatDisplay->setMinimumHeight(400);
    m_chatDisplay->setStyleSheet(
        "QTextEdit {"
        "  border: 2px solid #00bcd4;"
        "  border-radius: 8px;"
        "  padding: 12px;"
        "  background-color: #0d1b2a;"
        "  color: #ecf0f1;"
        "  font-family: 'Segoe UI', Arial, sans-serif;"
        "  font-size: 12px;"
        "  line-height: 1.6;"
        "}"
    );
    mainLayout->addWidget(m_chatDisplay);
    
    // Suggested questions
    QLabel *suggestedLabel = new QLabel("Suggestions:");
    suggestedLabel->setStyleSheet(
        "QLabel {"
        "  color: #00bcd4;"
        "  font-weight: bold;"
        "  font-size: 11px;"
        "}"
    );
    mainLayout->addWidget(suggestedLabel);
    
    QHBoxLayout *suggestedLayout = new QHBoxLayout();
    suggestedLayout->setSpacing(8);
    m_suggestedQuestionsLayout = suggestedLayout;
    setupSuggestedQuestions();
    mainLayout->addLayout(suggestedLayout);
    
    // Input area
    QHBoxLayout *inputLayout = new QHBoxLayout();
    inputLayout->setSpacing(8);
    
    m_inputField = new QLineEdit();
    m_inputField->setPlaceholderText("Posez une question ou un ordre... (ex: \"Quel prix pour une sardine?\")");
    m_inputField->setMinimumHeight(40);
    m_inputField->setStyleSheet(
        "QLineEdit {"
        "  border: 2px solid #00bcd4;"
        "  border-radius: 6px;"
        "  padding: 8px 12px;"
        "  background-color: #0d1b2a;"
        "  color: #00bcd4;"
        "  font-size: 12px;"
        "  selection-background-color: #00bcd4;"
        "  selection-color: #0d1b2a;"
        "}"
        "QLineEdit:focus {"
        "  border: 2px solid #00e5ff;"
        "  outline: none;"
        "}"
        "QLineEdit::placeholder {"
        "  color: #64b5f6;"
        "}"
    );
    inputLayout->addWidget(m_inputField);
    
    m_sendButton = new QPushButton("Envoyer");
    m_sendButton->setMinimumWidth(100);
    m_sendButton->setMinimumHeight(40);
    m_sendButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #00bcd4;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 6px;"
        "  padding: 8px 16px;"
        "  font-weight: bold;"
        "  font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #00e5ff;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #0097a7;"
        "}"
    );
    inputLayout->addWidget(m_sendButton);
    
    m_clearButton = new QPushButton("Effacer");
    m_clearButton->setMinimumWidth(80);
    m_clearButton->setMinimumHeight(40);
    m_clearButton->setStyleSheet(
        "QPushButton {"
        "  background-color: #ff6b6b;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 6px;"
        "  padding: 8px 16px;"
        "  font-weight: bold;"
        "  font-size: 12px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #ff8787;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #f06060;"
        "}"
    );
    inputLayout->addWidget(m_clearButton);
    
    mainLayout->addLayout(inputLayout);
    
    setLayout(mainLayout);
}

void ChatDialog::styleUI()
{
    setStyleSheet(
        "ChatDialog {"
        "  background-color: #0a0e27;"
        "  color: #ecf0f1;"
        "}"
    );
    
    m_userMessageStyle = 
        "<div style='margin: 8px 0; padding: 10px; background-color: #1a73e8; border-radius: 8px; color: white; text-align: right;'>"
        "<b>Vous:</b> %1"
        "</div>";
    
    m_assistantMessageStyle =
        "<div style='margin: 8px 0; padding: 10px; background-color: #0d4a6f; border-radius: 8px; color: #ecf0f1;'>"
        "<b>🤖 Assistant:</b><br/>%1"
        "</div>";
}

void ChatDialog::onSendMessage()
{
    QString userMessage = m_inputField->text().trimmed();
    if (userMessage.isEmpty()) return;
    
    // Add user message to chat
    addMessageToChat(userMessage, true);
    m_inputField->clear();
    
    // Get chatbot response
    QString response = m_chatBot->processMessage(userMessage);
    addMessageToChat(response, false);
    
    // Scroll to bottom
    QTextCursor cursor = m_chatDisplay->textCursor();
    cursor.movePosition(QTextCursor::End);
    m_chatDisplay->setTextCursor(cursor);
    
    // Set focus back to input
    m_inputField->setFocus();
}

void ChatDialog::onSuggestedQuestionClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (button) {
        m_inputField->setText(button->text());
        m_inputField->setFocus();
    }
}

void ChatDialog::onClearChat()
{
    m_chatDisplay->clear();
    displayInitialGreeting();
}

void ChatDialog::addMessageToChat(const QString &message, bool isUser)
{
    QString htmlMessage;
    if (isUser) {
        htmlMessage = m_userMessageStyle.arg(message);
    } else {
        htmlMessage = m_assistantMessageStyle.arg(message);
    }
    
    m_chatDisplay->append(htmlMessage);
}

void ChatDialog::displayInitialGreeting()
{
    QString greeting = 
        "<div style='text-align: center; margin: 20px; color: #00bcd4;'>"
        "<h2>🤖 Bienvenue dans l'Assistant Métier</h2>"
        "<p style='font-size: 13px; line-height: 1.6;'>"
        "Je suis votre assistant intelligent pour la gestion des transactions de poisson.<br/>"
        "Je peux vous aider à recommander des prix, détecter les anomalies,<br/>"
        "analyser vos données et bien plus encore!<br/><br/>"
        "<b>Tapez votre question ou cliquez sur une suggestion.</b>"
        "</p>"
        "</div>";
    
    m_chatDisplay->setHtml(greeting);
}

void ChatDialog::setupSuggestedQuestions()
{
    QStringList questions = {
        "Quel prix pour une sardine?",
        "Y a-t-il des anomalies?",
        "Montre les statistiques",
        "Prix des poissons",
        "Aide"
    };
    
    // Clear existing buttons
    while (m_suggestedQuestionsLayout->count() > 0) {
        QLayoutItem *item = m_suggestedQuestionsLayout->takeAt(0);
        delete item->widget();
        delete item;
    }
    
    // Add suggested question buttons
    for (const QString &question : questions) {
        QPushButton *btn = new QPushButton(question);
        btn->setMaximumWidth(180);
        btn->setMinimumHeight(32);
        btn->setStyleSheet(
            "QPushButton {"
            "  background-color: #1a237e;"
            "  color: #00bcd4;"
            "  border: 1px solid #00bcd4;"
            "  border-radius: 4px;"
            "  padding: 6px 12px;"
            "  font-size: 11px;"
            "  font-weight: bold;"
            "}"
            "QPushButton:hover {"
            "  background-color: #283593;"
            "  color: #00e5ff;"
            "}"
            "QPushButton:pressed {"
            "  background-color: #000a56;"
            "}"
        );
        connect(btn, &QPushButton::clicked, this, &ChatDialog::onSuggestedQuestionClicked);
        m_suggestedQuestionsLayout->addWidget(btn);
    }
    
    m_suggestedQuestionsLayout->addStretch();
}
