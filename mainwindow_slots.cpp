// These are the chat assistant slots that should be added to mainwindow.cpp

// ─── Chat Assistant ────────────────────────────────────

void MainWindow::onSendChatMessage()
{
    QString userMsg = ui->chatInput->text().trimmed();
    if (userMsg.isEmpty()) return;

    // Display user message
    ui->chatDisplay->append("<div style='color: #e6eef6; text-align: right; margin: 8px 0;'><b>Vous:</b> " + userMsg + "</div>");
    
    // Initialize assistant if not done
    if (!m_assistantInitialized && m_assistant) {
        m_assistant->startDetectionAssistant(m_transactions);
        m_assistantInitialized = true;
    }

    // Get assistant response
    QString response = m_assistant->processUserMessage(userMsg);
    
    // Display assistant response
    ui->chatDisplay->append("<div style='color: #39c0fa; margin: 8px 0;'><b>Assistant:</b><br>" + response + "</div>");
    
    // Clear input
    ui->chatInput->clear();
    
    // Auto-scroll to bottom
    ui->chatDisplay->verticalScrollBar()->setValue(ui->chatDisplay->verticalScrollBar()->maximum());
}

void MainWindow::onChatInputReturnPressed()
{
    onSendChatMessage();
}
