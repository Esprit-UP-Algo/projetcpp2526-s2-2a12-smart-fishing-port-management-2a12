#include "assistantdialog.h"
#include <QDateTime>
#include <algorithm>
#include <cmath>
#include <utility>

AssistantDialog::AssistantDialog(QObject *parent)
    : QObject(parent)
    , m_state(GREETING)
    , m_messageCount(0)
    , m_hasAnalyzed(false)
{
}

void AssistantDialog::startDetectionAssistant(const QVector<Transaction> &transactions)
{
    m_transactions = transactions;
    m_state = GREETING;
    m_messageCount = 0;
    m_hasAnalyzed = false;
    m_detectedAnomalies.clear();
}

QString AssistantDialog::getInitialGreeting() const
{
    return "👋 Bienvenue dans l'Assistant de Détection d'Anomalies!\n\n"
           "Je vais vous aider à identifier les transactions anormales dans votre historique. "
           "J'analyse actuellement vos données...\n\n"
           "Que cherchez-vous à vérifier?\n"
           "- Une augmentation ou baisse anormale des prix\n"
           "- Des quantités inhabituelles\n"
           "- Des écarts par rapport à votre historique\n"
           "- Autre (précisez)";
}

QString AssistantDialog::processUserMessage(const QString &userMessage)
{
    m_lastUserMessage = userMessage;
    m_messageCount++;
    
    QString response;
    
    switch (m_state) {
        case GREETING:
            m_state = ASKING_DETAILS;
            response = analyzeTransactions();
            break;
            
        case ASKING_DETAILS:
            m_state = ANALYZING;
            response = detectAnomalies();
            break;
            
        case ANALYZING:
            m_state = SHOWING_RESULTS;
            response = "Analyse complète !\n\n";
            response += m_currentAnalysis;
            if (!m_detectedAnomalies.isEmpty()) {
                response += "\n\n🔴 Anomalies détectées:\n";
                for (const QString &anomaly : std::as_const(m_detectedAnomalies)) {
                    response += "• " + anomaly + "\n";
                }
            }
            break;
            
        case SHOWING_RESULTS:
            m_state = COMPLETED;
            response = "Merci d'avoir utilisé l'Assistant de Détection d'Anomalies. "
                      "Souhaitez-vous continuer l'analyse ou vérifier autre chose?";
            break;
            
        case COMPLETED:
            // Loop back to asking details for another search
            m_state = ASKING_DETAILS;
            response = "que cherchez-vous à analyser maintenant?";
            break;
    }
    
    return response;
}

QString AssistantDialog::analyzeTransactions()
{
    if (m_transactions.isEmpty()) {
        return "⚠️ Aucune transaction trouvée dans votre historique. "
               "Veuillez d'abord ajouter des transactions.";
    }
    
    // Calculate statistics
    double totalQuantite = 0;
    double totalPrix = 0;
    double avgPrix = 0;
    double avgQuantite = 0;
    double minPrix = std::numeric_limits<double>::max();
    double maxPrix = 0;
    
    for (const auto &t : std::as_const(m_transactions)) {
        totalQuantite += t.quantite;
        totalPrix += t.prixKg;
        minPrix = std::min(minPrix, t.prixKg);
        maxPrix = std::max(maxPrix, t.prixKg);
    }
    
    avgPrix = totalPrix / m_transactions.size();
    avgQuantite = totalQuantite / m_transactions.size();
    
    QString analysis = QString::asprintf(
        "📊 Analyse de vos %d transactions:\n\n"
        "💰 Prix/kg:\n"
        "  • Minimum: %.2f DZD\n"
        "  • Maximum: %.2f DZD\n"
        "  • Moyenne: %.2f DZD\n\n"
        "📦 Quantités:\n"
        "  • Moyenne: %.2f kg\n"
        "  • Total: %.2f kg\n\n",
        (int)m_transactions.size(),
        minPrix, maxPrix, avgPrix,
        avgQuantite, totalQuantite
    );
    
    m_currentAnalysis = analysis;
    return "📈 Voici les statistiques initiales de vos transactions.\n\n" + analysis
           + "Voulez-vous que je détecte les anomalies basées sur ces données?";
}

QString AssistantDialog::detectAnomalies()
{
    m_detectedAnomalies = findAnomalousTransactions();
    
    if (m_detectedAnomalies.isEmpty()) {
        return "✅ Bonne nouvelle! Aucune anomalie majeure n'a été détectée dans vos transactions. "
               "Vos données semblent cohérentes et conformes.";
    }
    
    return QString("🔍 J'ai détecté %1 anomalie(s) dans vos données.")
        .arg(m_detectedAnomalies.size());
}

QStringList AssistantDialog::findAnomalousTransactions() const
{
    QStringList anomalies;
    
    if (m_transactions.size() < 2) {
        return anomalies;
    }
    
    // Calculate statistics
    double totalPrix = 0;
    double totalQuantite = 0;
    for (const auto &t : std::as_const(m_transactions)) {
        totalPrix += t.prixKg;
        totalQuantite += t.quantite;
    }
    
    double avgPrix = totalPrix / m_transactions.size();
    double avgQuantite = totalQuantite / m_transactions.size();
    
    // Calculate standard deviation
    double variancePrix = 0;
    double varianceQuantite = 0;
    for (const auto &t : std::as_const(m_transactions)) {
        variancePrix += (t.prixKg - avgPrix) * (t.prixKg - avgPrix);
        varianceQuantite += (t.quantite - avgQuantite) * (t.quantite - avgQuantite);
    }
    
    double stdDevPrix = std::sqrt(variancePrix / m_transactions.size());
    double stdDevQuantite = std::sqrt(varianceQuantite / m_transactions.size());
    
    // Detect anomalies (values more than 2 standard deviations away)
    for (const auto &t : m_transactions) {
        QString anomaly;
        
        if (stdDevPrix > 0 && std::abs(t.prixKg - avgPrix) > 2 * stdDevPrix) {
            anomaly = QString("Facture %1: Prix anormal %.2f DZD (moyenne: %.2f DZD)")
                .arg(t.numFacture).arg(t.prixKg).arg(avgPrix);
        } else if (stdDevQuantite > 0 && std::abs(t.quantite - avgQuantite) > 2 * stdDevQuantite) {
            anomaly = QString("Facture %1: Quantité anormale %.2f kg (moyenne: %.2f kg)")
                .arg(t.numFacture).arg(t.quantite).arg(avgQuantite);
        }
        
        // Also detect very old or very recent transactions
        if (!t.dateTransaction.isNull()) {
            qint64 daysAgo = t.dateTransaction.daysTo(QDateTime::currentDateTime());
            if (daysAgo > 365) {
                anomaly += QString(" [Transaction très ancienne: %1 jours]").arg(daysAgo);
            }
        }
        
        if (!anomaly.isEmpty()) {
            anomalies.append(anomaly);
        }
    }
    
    return anomalies;
}

void AssistantDialog::reset()
{
    m_state = GREETING;
    m_messageCount = 0;
    m_hasAnalyzed = false;
    m_detectedAnomalies.clear();
    m_currentAnalysis.clear();
    m_lastUserMessage.clear();
    m_transactions.clear();
}
