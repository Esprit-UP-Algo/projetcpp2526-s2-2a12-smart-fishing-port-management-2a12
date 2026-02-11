#include "transactiondialog.h"
#include "./ui_transactiondialog.h"

#include <QLocale>

TransactionDialog::TransactionDialog(QWidget *parent, const QString &title)
    : QDialog(parent)
    , ui(new Ui::TransactionDialog)
{
    ui->setupUi(this);
    ui->dialogTitle->setText(title);
    ui->dialogSubtitle->setText(tr("Veuillez remplir les informations de la transaction."));
    setWindowTitle(title);

    ui->dateTransaction->setDateTime(QDateTime::currentDateTime());

    connect(ui->btnOk,     &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::reject);

    connect(ui->spinPrixKg,  QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &TransactionDialog::recalcTotal);
    connect(ui->spinQuantite, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &TransactionDialog::recalcTotal);
}

TransactionDialog::~TransactionDialog()
{
    delete ui;
}

void TransactionDialog::recalcTotal()
{
    double t = ui->spinPrixKg->value() * ui->spinQuantite->value();
    ui->totalValue->setText(QLocale().toString(t, 'f', 2) + " DT");
}

void TransactionDialog::setTransaction(const Transaction &t)
{
    ui->lineNumFacture->setText(t.numFacture);
    ui->linePecheur->setText(t.pecheur);
    ui->lineLot->setText(t.lot);
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
    t.lot             = ui->lineLot->text().trimmed();
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
    ui->lineLot->setReadOnly(ro);
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
