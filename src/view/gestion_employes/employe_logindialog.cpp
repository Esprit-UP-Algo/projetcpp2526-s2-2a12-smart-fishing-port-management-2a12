#include "employe_logindialog.h"
#include "ui_employe_logindialog.h"
#include <QMessageBox>

namespace employes {

EmployeLoginDialog::EmployeLoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmployeLoginDialog)
{
    ui->setupUi(this);
    
    connect(ui->btnLogin, &QPushButton::clicked, this, &EmployeLoginDialog::onLoginClicked);
}

EmployeLoginDialog::~EmployeLoginDialog()
{
    delete ui;
}

void EmployeLoginDialog::onLoginClicked()
{
    QString user = ui->lineUser->text();
    QString pass = ui->linePass->text();

    if (user == "admin" && pass == "admin") {
        accept();
    } else {
        QMessageBox::warning(this, "Erreur", "Identifiant ou mot de passe incorrect.");
    }
}

} // namespace employes
