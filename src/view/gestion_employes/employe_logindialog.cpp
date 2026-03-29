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
    QString login = ui->lineUser->text();
    QString password = ui->linePass->text();
    
    QString loginError = AuthHelper::validateLoginInput(login);
    if (!loginError.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez verifiez vos cordonnes");
        return;
    }
    
    QString passError = AuthHelper::validatePasswordInput(password);
    if (!passError.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez verifiez vos cordonnes");
        return;
    }
    
    EmployeUser employe = EmployeDAO::findByLogin(login);
    
    if (employe.login.isEmpty()) {
        qDebug() << "DEBUG: User not found:" << login;
        QMessageBox::warning(this, "Erreur",
                            "Utilisateur non trouvé");
        return;
    }
    
    qDebug() << "DEBUG: User found:" << employe.login << "Status:" << employe.statut;
    
    if (employe.statut != "Actif" && employe.statut != "actif") {
        qDebug() << "DEBUG: Status not Actif:" << employe.statut;
        QMessageBox::warning(this, "Erreur", "Compte désactivé");
        return;
    }
    
    bool passwordMatch = AuthHelper::verifyPassword(password, employe.password);
    qDebug() << "DEBUG: Password match:" << passwordMatch;
    qDebug() << "DEBUG: Stored password hash:" << employe.password;
    
    if (!passwordMatch) {
        QMessageBox::warning(this, "Erreur",
                            "Mot de passe incorrect");
        return;
    }
    
    QMessageBox::information(this, "Succès",
                            "Bienvenue " + employe.prenom + " " + employe.nom + "!");
    accept();
}

} // namespace employes
