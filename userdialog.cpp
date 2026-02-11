#include "userdialog.h"
#include "ui_userdialog.h"

#include <QDateTime>
#include <QTime>

UserDialog::UserDialog(QWidget *parent, const QString &title)
    : QDialog(parent)
    , ui(new Ui::UserDialog)
{
    ui->setupUi(this);
    setWindowTitle(title);
    ui->dateCreation->setDate(QDate::currentDate());

    connect(ui->btnOk, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

UserDialog::~UserDialog()
{
    delete ui;
}

void UserDialog::setUser(const User &u)
{
    ui->lineCIN->setText(u.cin);
    ui->lineNom->setText(u.nom);
    ui->linePrenom->setText(u.prenom);
    ui->lineEmail->setText(u.email);
    ui->lineLogin->setText(u.login);
    ui->linePassword->setText(u.password);
    ui->comboRole->setCurrentText(u.role);
    ui->comboStatut->setCurrentText(u.statut);
    ui->spinHeures->setValue(u.heures);
    ui->dateCreation->setDate(u.dateCreation.date());
}

User UserDialog::user() const
{
    User u;
    u.cin = ui->lineCIN->text().trimmed();
    u.nom = ui->lineNom->text().trimmed();
    u.prenom = ui->linePrenom->text().trimmed();
    u.email = ui->lineEmail->text().trimmed();
    u.login = ui->lineLogin->text().trimmed();
    u.password = ui->linePassword->text();
    u.role = ui->comboRole->currentText();
    u.statut = ui->comboStatut->currentText();
    u.heures = ui->spinHeures->value();
    u.dateCreation = QDateTime(ui->dateCreation->date(), QTime(0,0));
    return u;
}

void UserDialog::setReadOnly(bool ro)
{
    ui->lineCIN->setReadOnly(ro);
    ui->lineNom->setReadOnly(ro);
    ui->linePrenom->setReadOnly(ro);
    ui->lineEmail->setReadOnly(ro);
    ui->lineLogin->setReadOnly(ro);
    ui->linePassword->setReadOnly(ro);
    ui->spinHeures->setReadOnly(ro);
    ui->dateCreation->setReadOnly(ro);
    ui->comboRole->setEnabled(!ro);
    ui->comboStatut->setEnabled(!ro);
    if (ro) { ui->btnOk->setText(tr("Fermer")); ui->btnCancel->hide(); }
}
