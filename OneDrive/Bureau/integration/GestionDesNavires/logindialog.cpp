#include "logindialog.h"

#include "connection.h"
#include "securepassword.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSettings>
#include <QVBoxLayout>

namespace {
static constexpr const char *kSettingsGroup = "db";
static constexpr const char *kKeyDsn = "dsn";
static constexpr const char *kKeyUser = "user";
static constexpr const char *kKeyPassProtected = "pass_protected";
}

LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(QStringLiteral("Connexion"));
    setModal(true);

    editDsn = new QLineEdit(this);
    editUser = new QLineEdit(this);
    editPassword = new QLineEdit(this);
    editPassword->setEchoMode(QLineEdit::Password);

    lblStatus = new QLabel(this);
    lblStatus->setWordWrap(true);
    lblStatus->setStyleSheet(QStringLiteral("color: #b71c1c;"));

    auto *form = new QFormLayout();
    form->addRow(QStringLiteral("Source (DSN ODBC)"), editDsn);
    form->addRow(QStringLiteral("Utilisateur"), editUser);
    form->addRow(QStringLiteral("Mot de passe"), editPassword);
    form->addRow(lblStatus);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    auto *btnOk = buttons->button(QDialogButtonBox::Ok);
    btnOk->setText(QStringLiteral("Se connecter"));
    buttons->button(QDialogButtonBox::Cancel)->setText(QStringLiteral("Quitter"));

    connect(buttons, &QDialogButtonBox::accepted, this, &LoginDialog::tryConnect);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto *layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addWidget(buttons);
    setLayout(layout);

    loadSettings();

    if (editUser->text().isEmpty())
        editUser->setFocus();
    else
        editPassword->setFocus();
}

void LoginDialog::loadSettings()
{
    QSettings s;
    s.beginGroup(QLatin1String(kSettingsGroup));
    const QString dsn = s.value(QLatin1String(kKeyDsn), QStringLiteral("source_projet2A")).toString();
    const QString user = s.value(QLatin1String(kKeyUser), QStringLiteral("mouradd")).toString();
    s.endGroup();

    editDsn->setText(dsn);
    editUser->setText(user);
}

void LoginDialog::saveSettings(const QString &dsn, const QString &user, const QString &password)
{
    QSettings s;
    s.beginGroup(QLatin1String(kSettingsGroup));
    s.setValue(QLatin1String(kKeyDsn), dsn);
    s.setValue(QLatin1String(kKeyUser), user);
    QString protectErr;
    const QString protectedPass = protectSecret(password, &protectErr);
    if (!protectedPass.isEmpty())
        s.setValue(QLatin1String(kKeyPassProtected), protectedPass);
    s.endGroup();
}

void LoginDialog::tryConnect()
{
    lblStatus->clear();

    const QString dsn = editDsn->text().trimmed();
    const QString user = editUser->text().trimmed();
    const QString pass = editPassword->text();

    if (dsn.isEmpty() || user.isEmpty()) {
        lblStatus->setText(QStringLiteral("Veuillez saisir la source (DSN) et l'utilisateur."));
        return;
    }

    QString err;
    if (!Connection::instance()->createConnect(dsn, user, pass, &err)) {
        lblStatus->setText(err.isEmpty() ? QStringLiteral("Connexion échouée.") : err);
        editPassword->selectAll();
        editPassword->setFocus();
        return;
    }

    saveSettings(dsn, user, pass);
    accept();
}
