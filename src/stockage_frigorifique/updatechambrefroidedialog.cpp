#include "updatechambrefroidedialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>

#include "chambrefroide.h"

namespace stockage {

UpdateChambreFroideDialog::UpdateChambreFroideDialog(const ChambreFroide &chambre, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(tr("Modifier une chambre froide"));
    setMinimumWidth(450);
    buildUi();
    applyTheme();
    wireSignals();

    // Load data
    m_idChambre->setText(chambre.idChambre);
    m_idChambre->setReadOnly(true);
    m_capacite->setValue(chambre.capacite);
    m_tempConsigne->setValue(chambre.tempConsigne);
    m_emplacement->setText(chambre.emplacement);
    m_temperature->setValue(chambre.temperature);
}

void UpdateChambreFroideDialog::buildUi()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Title
    auto *title = new QLabel(tr("Modifier une chambre froide"), this);
    QFont titleFont = title->font();
    titleFont.setPixelSize(16);
    titleFont.setBold(true);
    title->setFont(titleFont);
    mainLayout->addWidget(title);

    // Form layout
    auto *formLayout = new QFormLayout();
    formLayout->setSpacing(12);
    formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    // Chambre (read-only)
    m_idChambre = new QLineEdit(this);
    m_idChambre->setObjectName("Input");
    m_idChambre->setMinimumHeight(32);
    m_idChambre->setReadOnly(true);
    formLayout->addRow(tr("Chambre"), m_idChambre);

    // Capacité
    m_capacite = new QSpinBox(this);
    m_capacite->setObjectName("Input");
    m_capacite->setMinimumHeight(32);
    m_capacite->setRange(0, 100000);
    formLayout->addRow(tr("Capacité"), m_capacite);

    // Température consigne
    m_tempConsigne = new QDoubleSpinBox(this);
    m_tempConsigne->setObjectName("Input");
    m_tempConsigne->setMinimumHeight(32);
    m_tempConsigne->setRange(-50.0, 50.0);
    m_tempConsigne->setSingleStep(0.1);
    formLayout->addRow(tr("Temp Consigne"), m_tempConsigne);

    // Emplacement
    m_emplacement = new QLineEdit(this);
    m_emplacement->setObjectName("Input");
    m_emplacement->setMinimumHeight(32);
    formLayout->addRow(tr("Emplacement"), m_emplacement);

    // Température
    m_temperature = new QDoubleSpinBox(this);
    m_temperature->setObjectName("Input");
    m_temperature->setMinimumHeight(32);
    m_temperature->setRange(-50.0, 50.0);
    m_temperature->setSingleStep(0.1);
    formLayout->addRow(tr("Température"), m_temperature);

    mainLayout->addLayout(formLayout);

    // Buttons
    auto *btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(10);

    m_btnModifier = new QPushButton(tr("Modifier"), this);
    m_btnModifier->setObjectName("AltPrimaryButton");
    m_btnModifier->setMinimumHeight(40);
    btnLayout->addWidget(m_btnModifier);

    m_btnEnregistrer = new QPushButton(tr("Enregistrer"), this);
    m_btnEnregistrer->setObjectName("PrimaryButton");
    m_btnEnregistrer->setMinimumHeight(40);
    btnLayout->addWidget(m_btnEnregistrer);

    mainLayout->addLayout(btnLayout);
}

void UpdateChambreFroideDialog::applyTheme()
{
    setStyleSheet(R"(
        QDialog {
            background: #0c1a29;
        }
        QLabel {
            color: #e6eef6;
        }
        QLineEdit#Input, QSpinBox#Input, QDoubleSpinBox#Input {
            background: #101c2c;
            border: 1px solid #23374e;
            border-radius: 8px;
            padding: 4px 10px;
            color: #e6eef6;
            selection-background-color: #1e8de0;
        }
        QDoubleSpinBox#Input { padding-right: 30px; }
        QSpinBox#Input { padding-right: 30px; }
        QPushButton#AltPrimaryButton {
            background: #1e8de0;
            color: white;
            border: 0px;
            border-radius: 10px;
            padding: 9px 14px;
            font-weight: 700;
        }
        QPushButton#AltPrimaryButton:hover { background: #2a99ec; }
        QPushButton#PrimaryButton {
            background: #23374e;
            color: #e6eef6;
            border: 1px solid #2b4561;
            border-radius: 10px;
            padding: 9px 14px;
        }
        QPushButton#PrimaryButton:hover { background: #2b4561; }
    )");
}

void UpdateChambreFroideDialog::wireSignals()
{
    connect(m_btnModifier, &QPushButton::clicked, this, &UpdateChambreFroideDialog::onModifier);
    connect(m_btnEnregistrer, &QPushButton::clicked, this, &QDialog::accept);
}

void UpdateChambreFroideDialog::onModifier()
{
    // Just close the dialog and return to form
    reject();
}

ChambreFroide UpdateChambreFroideDialog::getChambreFroide() const
{
    ChambreFroide c;
    c.idChambre = m_idChambre->text().trimmed();
    c.capacite = m_capacite->value();
    c.tempConsigne = m_tempConsigne->value();
    c.emplacement = m_emplacement->text().trimmed();
    c.temperature = m_temperature->value();
    return c;
}

} // namespace stockage
