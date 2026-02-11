#include "logindialog.h"
#include "ui_logindialog.h"
#include <QMessageBox>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);

    // Remove window title bar and make it frameless if desired, 
    // but user asked for "Fenêtre pleine largeur" full window background.
    // However, they also asked for specific QDialog geometry.
    // Let's keep standard frame for now unless requested otherwise, 
    // but apply the requested stylesheet.
    
    connect(ui->btnLogin, &QPushButton::clicked, this, &LoginDialog::onLoginClicked);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::onLoginClicked()
{
    QString user = ui->lineUser->text();
    QString pass = ui->linePass->text();

    // Mock authentication
    if (user == "admin" && pass == "admin") {
        accept();
    } else {
        QMessageBox::warning(this, "Erreur", "Identifiant ou mot de passe incorrect.");
    }
}
