#include "updatestockagelotdialog.h"

#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QDateTimeEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFont>
#include <QDateTime>

#include "stockagelot.h"
#include "chambrefroidemodel.h"

namespace stockage {

UpdateStockageLotDialog::UpdateStockageLotDialog(const StockageLot &lot, ChambreFroideModel *chambreModel, QWidget *parent)
    : QDialog(parent), m_chambreModel(chambreModel)
{
    setWindowTitle(tr("Modifier un lot"));
    setMinimumWidth(450);
    buildUi();
    applyTheme();
    wireSignals();

    // Load data
    m_idLot->setText(lot.idLot);
    m_idLot->setReadOnly(true);
    m_espece->setText(lot.espece);
    m_quantite->setValue(lot.quantite);
    m_entree->setDateTime(lot.dateEntree);
    m_sortieRenseignee->setChecked(lot.dateSortie.isValid());
    m_sortie->setDateTime(lot.dateSortie.isValid() ? lot.dateSortie : QDateTime::currentDateTime());
    m_sortie->setEnabled(lot.dateSortie.isValid());

    const int idx = m_chambreLot->findText(lot.idChambre);
    if (idx >= 0)
        m_chambreLot->setCurrentIndex(idx);
}

void UpdateStockageLotDialog::buildUi()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(15);

    // Title
    auto *title = new QLabel(tr("Modifier un lot"), this);
    QFont titleFont = title->font();
    titleFont.setPixelSize(16);
    titleFont.setBold(true);
    title->setFont(titleFont);
    mainLayout->addWidget(title);

    // Form layout
    auto *formLayout = new QFormLayout();
    formLayout->setSpacing(12);
    formLayout->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);

    // ID Lot (read-only)
    m_idLot = new QLineEdit(this);
    m_idLot->setObjectName("Input");
    m_idLot->setMinimumHeight(32);
    m_idLot->setReadOnly(true);
    formLayout->addRow(tr("ID Lot"), m_idLot);

    // Espace
    m_espece = new QLineEdit(this);
    m_espece->setObjectName("Input");
    m_espece->setMinimumHeight(32);
    formLayout->addRow(tr("Espèce"), m_espece);

    // Quantité
    m_quantite = new QSpinBox(this);
    m_quantite->setObjectName("Input");
    m_quantite->setMinimumHeight(32);
    m_quantite->setRange(0, 100000);
    formLayout->addRow(tr("Quantité"), m_quantite);

    // Date/Heure arrivée
    m_entree = new QDateTimeEdit(this);
    m_entree->setObjectName("Input");
    m_entree->setMinimumHeight(32);
    m_entree->setCalendarPopup(true);
    formLayout->addRow(tr("Date/Heure arrivée"), m_entree);

    // Sortie renseignée
    m_sortieRenseignee = new QCheckBox(tr("Date de sortie renseignée"), this);
    m_sortieRenseignee->setObjectName("Check");
    formLayout->addRow(QString(), m_sortieRenseignee);

    // Date/Heure sortie
    m_sortie = new QDateTimeEdit(this);
    m_sortie->setObjectName("Input");
    m_sortie->setMinimumHeight(32);
    m_sortie->setCalendarPopup(true);
    m_sortie->setEnabled(false);
    formLayout->addRow(tr("Date/Heure sortie"), m_sortie);

    // Chambre
    m_chambreLot = new QComboBox(this);
    m_chambreLot->setObjectName("Input");
    m_chambreLot->setMinimumHeight(32);

    if (m_chambreModel)
    {
        const auto &items = m_chambreModel->items();
        for (const auto &chambre : items)
            m_chambreLot->addItem(chambre.idChambre);
    }

    formLayout->addRow(tr("Chambre"), m_chambreLot);

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

void UpdateStockageLotDialog::applyTheme()
{
    setStyleSheet(R"(
        QDialog {
            background: #0c1a29;
        }
        QLabel {
            color: #e6eef6;
        }
        QLineEdit#Input, QSpinBox#Input, QDateTimeEdit#Input, QComboBox#Input {
            background: #101c2c;
            border: 1px solid #23374e;
            border-radius: 8px;
            padding: 4px 10px;
            color: #e6eef6;
            selection-background-color: #1e8de0;
        }
        QSpinBox#Input { padding-right: 30px; }
        QDateTimeEdit#Input { padding-right: 28px; }
        QComboBox#Input::drop-down { border: 0px; width: 20px; }
        QCheckBox#Check { color: #e6eef6; }
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

void UpdateStockageLotDialog::wireSignals()
{
    connect(m_sortieRenseignee, &QCheckBox::toggled, this, &UpdateStockageLotDialog::onSortieCheckChanged);
    connect(m_btnModifier, &QPushButton::clicked, this, &UpdateStockageLotDialog::onModifier);
    connect(m_btnEnregistrer, &QPushButton::clicked, this, &UpdateStockageLotDialog::onEnregistrer);
}

void UpdateStockageLotDialog::onModifier()
{
    reject();
}

void UpdateStockageLotDialog::onEnregistrer()
{
    accept();
}

void UpdateStockageLotDialog::onSortieCheckChanged(bool checked)
{
    m_sortie->setEnabled(checked);
}

StockageLot UpdateStockageLotDialog::getStockageLot() const
{
    StockageLot l;
    l.idLot = m_idLot->text().trimmed();
    l.espece = m_espece->text().trimmed();
    l.quantite = m_quantite->value();
    l.dateEntree = m_entree->dateTime();
    l.dateSortie = m_sortieRenseignee->isChecked() ? m_sortie->dateTime() : QDateTime();
    l.idChambre = m_chambreLot->currentText().trimmed();
    return l;
}

} // namespace stockage
