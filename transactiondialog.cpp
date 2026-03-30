#include "transactiondialog.h"
#include "./ui_transactiondialog.h"

#include <QLocale>
#include <QMessageBox>
#include <QDateTime>
#include <QIntValidator>

TransactionDialog::TransactionDialog(QWidget *parent, const QString &title)
    : QDialog(parent)
    , ui(new Ui::TransactionDialog)
{
    ui->setupUi(this);
    ui->dialogTitle->setText(title);
    ui->dialogSubtitle->setText(tr("Veuillez remplir les informations de la transaction."));
    setWindowTitle(title);

    // Contrôle de saisie : Num Facture doit être un nombre entier
    // car la base de données attend NUMBER(10,0).
    ui->lineNumFacture->setValidator(new QIntValidator(1, 999999999, this));
    ui->lineNumFacture->setPlaceholderText("Ex: 100");

    ui->dateTransaction->setDateTime(QDateTime::currentDateTime());

    connect(ui->btnOk,     &QPushButton::clicked, this, &TransactionDialog::onOkClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::reject);

    connect(ui->spinPrixKg,  QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &TransactionDialog::recalcTotal);
    connect(ui->spinQuantite, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &TransactionDialog::recalcTotal);
}

void TransactionDialog::onOkClicked()
{
    const QString nom = ui->linePecheur->text().trimmed();
    if (nom.isEmpty()) {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Warning);
        msg.setWindowTitle(tr("Contrôle de saisie"));
        msg.setText(tr("Le nom est obligatoire."));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.setStyleSheet(
            "QMessageBox {"
            "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #eaf6fd, stop:1 #ffffff);"
            "  border-radius: 6px;"
            "}"
            "QLabel { color: #000000; font-weight: normal; }"
            "QPushButton { min-width: 80px; padding: 6px 12px; }"
        );
        msg.exec();
        return;
    }

    const QString num = ui->lineNumFacture->text().trimmed();
    if (num.isEmpty()) {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Warning);
        msg.setWindowTitle(tr("Contrôle de saisie"));
        msg.setText(tr("Le numéro de facture est obligatoire."));
        msg.setStandardButtons(QMessageBox::Ok);
        msg.setDefaultButton(QMessageBox::Ok);
        msg.setStyleSheet(
            "QMessageBox {"
            "  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #eaf6fd, stop:1 #ffffff);"
            "  border-radius: 6px;"
            "}"
            "QLabel { color: #000000; font-weight: normal; }"
            "QPushButton { min-width: 80px; padding: 6px 12px; }"
        );
        msg.exec();
        return;
    }

    accept();
}

TransactionDialog::~TransactionDialog()
{
    delete ui;
}

void TransactionDialog::recalcTotal()
{
    double t = ui->spinPrixKg->value() * ui->spinQuantite->value();
    ui->totalValue->setText(QLocale().toString(t, 'f', 2) + " TND");
}

void TransactionDialog::setTransaction(const Transaction &t)
{
    ui->lineNumFacture->setText(t.numFacture);
    ui->linePecheur->setText(t.pecheur);
    int lotIdx = ui->comboLot->findText(t.lot);
    if (lotIdx >= 0) ui->comboLot->setCurrentIndex(lotIdx);
    else ui->comboLot->setCurrentIndex(0);
    ui->spinPrixKg->setValue(t.prixKg);
    ui->spinQuantite->setValue(t.quantite);
    ui->dateTransaction->setDateTime(t.dateTransaction.isValid()
                                         ? t.dateTransaction
                                         : QDateTime::currentDateTime());
    int idx = ui->comboModePaiement->findText(t.modePaiement);
    if (idx >= 0) ui->comboModePaiement->setCurrentIndex(idx);

    idx = ui->comboStatutPaiement->findText(t.statutPaiement);
    if (idx >= 0) ui->comboStatutPaiement->setCurrentIndex(idx);

    recalcTotal();
}

Transaction TransactionDialog::transaction() const
{
    Transaction t;
    t.numFacture      = ui->lineNumFacture->text().trimmed();
    t.pecheur         = ui->linePecheur->text().trimmed();
    t.lot             = ui->comboLot->currentText();
    t.prixKg          = ui->spinPrixKg->value();
    t.quantite        = ui->spinQuantite->value();
    t.dateTransaction = ui->dateTransaction->dateTime();
    t.modePaiement    = ui->comboModePaiement->currentText();
    t.statutPaiement  = ui->comboStatutPaiement->currentText();
    t.calcTotal();
    return t;
}

void TransactionDialog::setReadOnly(bool ro)
{
    ui->lineNumFacture->setReadOnly(ro);
    ui->linePecheur->setReadOnly(ro);
    ui->comboLot->setEnabled(!ro);
    ui->spinPrixKg->setReadOnly(ro);
    ui->spinQuantite->setReadOnly(ro);
    ui->dateTransaction->setReadOnly(ro);
    ui->comboModePaiement->setEnabled(!ro);
    ui->comboStatutPaiement->setEnabled(!ro);
    if (ro) {
        ui->btnOk->setText("Fermer");
        ui->btnCancel->hide();
    }
}
