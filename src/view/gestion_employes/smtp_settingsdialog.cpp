#include "smtp_settingsdialog.h"

#include "config_manager.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>

namespace employes {

SmtpSettingsDialog::SmtpSettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Paramètres SMTP"));
    setModal(true);

    auto *layout = new QFormLayout(this);

    m_host = new QLineEdit(this);
    m_port = new QLineEdit(this);
    m_user = new QLineEdit(this);
    m_password = new QLineEdit(this);
    m_from = new QLineEdit(this);
    m_password->setEchoMode(QLineEdit::Password);

    layout->addRow(tr("Hôte SMTP"), m_host);
    layout->addRow(tr("Port"), m_port);
    layout->addRow(tr("Utilisateur"), m_user);
    layout->addRow(tr("Mot de passe"), m_password);
    layout->addRow(tr("From"), m_from);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Save | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &SmtpSettingsDialog::onSaveClicked);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    layout->addRow(buttons);

    loadValues();
}

void SmtpSettingsDialog::loadValues()
{
    m_host->setText(ConfigManager::get("VS_SMTP_HOST"));
    m_port->setText(ConfigManager::get("VS_SMTP_PORT"));
    m_user->setText(ConfigManager::get("VS_SMTP_USER"));
    m_password->setText(ConfigManager::get("VS_SMTP_PASS"));
    m_from->setText(ConfigManager::get("VS_SMTP_FROM"));
}

void SmtpSettingsDialog::onSaveClicked()
{
    ConfigManager::set("VS_SMTP_HOST", m_host->text());
    ConfigManager::set("VS_SMTP_PORT", m_port->text());
    ConfigManager::set("VS_SMTP_USER", m_user->text());
    ConfigManager::set("VS_SMTP_PASS", m_password->text());
    ConfigManager::set("VS_SMTP_FROM", m_from->text());

    accept();
}

} // namespace employes