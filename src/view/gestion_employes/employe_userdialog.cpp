#include "employe_userdialog.h"
#include "ui_employe_userdialog.h"

namespace employes {

EmployeUserDialog::EmployeUserDialog(QWidget *parent, const QString &title)
    : QDialog(parent)
    , ui(new Ui::EmployeUserDialog)
{
    ui->setupUi(this);
    setWindowTitle(title);

    connect(ui->btnOk, &QPushButton::clicked, this, &QDialog::accept);
    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

EmployeUserDialog::~EmployeUserDialog()
{
    delete ui;
}

void EmployeUserDialog::setUser(const EmployeUser &u)
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
}

EmployeUser EmployeUserDialog::user() const
{
    EmployeUser u;
    u.cin = ui->lineCIN->text().trimmed();
    u.nom = ui->lineNom->text().trimmed();
    u.prenom = ui->linePrenom->text().trimmed();
    u.email = ui->lineEmail->text().trimmed();
    u.login = ui->lineLogin->text().trimmed();
    u.password = ui->linePassword->text();
    u.role = ui->comboRole->currentText();
    u.statut = ui->comboStatut->currentText();
    u.heures = ui->spinHeures->value();
    return u;
}

void EmployeUserDialog::setReadOnly(bool ro)
{
    ui->lineCIN->setReadOnly(ro);
    ui->lineNom->setReadOnly(ro);
    ui->linePrenom->setReadOnly(ro);
    ui->lineEmail->setReadOnly(ro);
    ui->lineLogin->setReadOnly(ro);
    ui->linePassword->setReadOnly(ro);
    ui->spinHeures->setReadOnly(ro);
    ui->comboRole->setEnabled(!ro);
    ui->comboStatut->setEnabled(!ro);
    if (ro) { ui->btnOk->setText(tr("Fermer")); ui->btnCancel->hide(); }
}

} // namespace employes
