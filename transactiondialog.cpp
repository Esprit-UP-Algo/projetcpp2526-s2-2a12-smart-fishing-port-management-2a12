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
    // Contrôle Pêcheur
    const QString pecheur = ui->linePecheur->text().trimmed();
    if (pecheur.isEmpty()) {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Warning);
        msg.setWindowTitle(tr("Erreur de saisie"));
        msg.setText(tr("❌ Le nom du pêcheur est obligatoire."));
        msg.setStyleSheet(
            "QMessageBox { background-color: #2c3e50; }"
            "QMessageBox QLabel { color: #FFFFFF; font-weight: bold; font-size: 13px; }"
            "QPushButton { background-color: #3498db; color: white; border-radius: 4px; padding: 6px 18px; }"
        );
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        ui->linePecheur->setFocus();
        return;
    }
    
    // Contrôle Référence - MAINTENANT OBLIGATOIRE
    const QString reference = ui->lineReference->text().trimmed();
    if (reference.isEmpty()) {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Warning);
        msg.setWindowTitle(tr("Erreur de saisie"));
        msg.setText(tr("❌ La référence est obligatoire."));
        msg.setStyleSheet(
            "QMessageBox { background-color: #2c3e50; }"
            "QMessageBox QLabel { color: #FFFFFF; font-weight: bold; font-size: 13px; }"
            "QPushButton { background-color: #3498db; color: white; border-radius: 4px; padding: 6px 18px; }"
        );
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        ui->lineReference->setFocus();
        return;
    }
    if (reference.length() < 2) {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Warning);
        msg.setWindowTitle(tr("Erreur de saisie"));
        msg.setText(tr("❌ La référence doit avoir au moins 2 caractères."));
        msg.setStyleSheet(
            "QMessageBox { background-color: #2c3e50; }"
            "QMessageBox QLabel { color: #FFFFFF; font-weight: bold; font-size: 13px; }"
            "QPushButton { background-color: #3498db; color: white; border-radius: 4px; padding: 6px 18px; }"
        );
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        ui->lineReference->setFocus();
        return;
    }
    if (reference.length() > 100) {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Warning);
        msg.setWindowTitle(tr("Erreur de saisie"));
        msg.setText(tr("❌ La référence ne doit pas dépasser 100 caractères."));
        msg.setStyleSheet(
            "QMessageBox { background-color: #2c3e50; }"
            "QMessageBox QLabel { color: #FFFFFF; font-weight: bold; font-size: 13px; }"
            "QPushButton { background-color: #3498db; color: white; border-radius: 4px; padding: 6px 18px; }"
        );
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        ui->lineReference->setFocus();
        return;
    }
    
    // Contrôle Poisson
    if (ui->comboLot->currentIndex() == 0) {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Warning);
        msg.setWindowTitle(tr("Erreur de saisie"));
        msg.setText(tr("❌ Veuillez sélectionner un poisson."));
        msg.setStyleSheet(
            "QMessageBox { background-color: #2c3e50; }"
            "QMessageBox QLabel { color: #FFFFFF; font-weight: bold; font-size: 13px; }"
            "QPushButton { background-color: #3498db; color: white; border-radius: 4px; padding: 6px 18px; }"
        );
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        ui->comboLot->setFocus();
        return;
    }
    
    // Contrôle Prix/kg
    if (ui->spinPrixKg->value() <= 0) {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Warning);
        msg.setWindowTitle(tr("Erreur de saisie"));
        msg.setText(tr("❌ Le prix doit être supérieur à 0."));
        msg.setStyleSheet(
            "QMessageBox { background-color: #2c3e50; }"
            "QMessageBox QLabel { color: #FFFFFF; font-weight: bold; font-size: 13px; }"
            "QPushButton { background-color: #3498db; color: white; border-radius: 4px; padding: 6px 18px; }"
        );
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        ui->spinPrixKg->setFocus();
        return;
    }
    
    // Contrôle Quantité
    if (ui->spinQuantite->value() <= 0) {
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Warning);
        msg.setWindowTitle(tr("Erreur de saisie"));
        msg.setText(tr("❌ La quantité doit être supérieure à 0."));
        msg.setStyleSheet(
            "QMessageBox { background-color: #2c3e50; }"
            "QMessageBox QLabel { color: #FFFFFF; font-weight: bold; font-size: 13px; }"
            "QPushButton { background-color: #3498db; color: white; border-radius: 4px; padding: 6px 18px; }"
        );
        msg.setStandardButtons(QMessageBox::Ok);
        msg.exec();
        ui->spinQuantite->setFocus();
        return;
    }
    
    // Tous les contrôles sont validés
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
    ui->lineReference->setText(t.reference);
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
    t.reference       = ui->lineReference->text().trimmed();
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
    ui->linePecheur->setReadOnly(ro);
    ui->lineReference->setReadOnly(ro);
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
