#include "employe_userdialog.h"
#include "ui_employe_userdialog.h"
#include "../../modele/gestion_employes/employe_dao.h"
#include "../../modele/gestion_employes/input_validator.h"

#include <QMessageBox>
#include <QPushButton>
#include <QStyle>

namespace employes {

EmployeUserDialog::EmployeUserDialog(QWidget *parent, const QString &title)
    : QDialog(parent)
    , ui(new Ui::EmployeUserDialog)
{
    ui->setupUi(this);
    setWindowTitle(title);

    setupFieldValidators();
    setupRealtimeValidation();
    updateFieldState();

    setStyleSheet(styleSheet() +
        "\nQLineEdit[invalid=\"true\"], QDoubleSpinBox[invalid=\"true\"] {"
        " border: 1px solid #ff6b6b; background-color: #2c1d27; }"
        "\nQLineEdit[invalid=\"false\"], QDoubleSpinBox[invalid=\"false\"] {"
        " border: 1px solid #23374e; }");

    connect(ui->btnOk, &QPushButton::clicked, this, [this]() {
        if (validateForm(true)) {
            accept();
        }
    });
    connect(ui->btnCancel, &QPushButton::clicked, this, &QDialog::reject);
}

EmployeUserDialog::~EmployeUserDialog()
{
    delete ui;
}

void EmployeUserDialog::setUser(const EmployeUser &u)
{
    m_isEditMode = true;
    ui->lineCIN->setText(u.cin);
    ui->lineNom->setText(u.nom);
    ui->linePrenom->setText(u.prenom);
    ui->lineEmail->setText(u.email);
    ui->lineLogin->setText(u.login);
    ui->linePassword->setText(u.password);
    ui->comboRole->setCurrentText(u.role);
    ui->comboStatut->setCurrentText(u.statut);
    ui->spinHeures->setValue(u.heures);
    updateFieldState();
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

void EmployeUserDialog::setupFieldValidators()
{
    ui->lineNom->setValidator(InputValidator::createNameTypingValidator(this, true));
    ui->linePrenom->setValidator(InputValidator::createNameTypingValidator(this, true));
    ui->lineCIN->setValidator(InputValidator::createDigitsTypingValidator(this, 8));
    ui->lineCIN->setMaxLength(8);

    ui->lineLogin->setValidator(InputValidator::createGeneralTextTypingValidator(this, false));
    ui->spinHeures->setRange(0.0, 9999.0);
    ui->spinHeures->setDecimals(1);
}

void EmployeUserDialog::setupRealtimeValidation()
{
    connect(ui->lineCIN, &QLineEdit::textChanged, this, [this]() { updateFieldState(); });
    connect(ui->lineNom, &QLineEdit::textChanged, this, [this]() { updateFieldState(); });
    connect(ui->linePrenom, &QLineEdit::textChanged, this, [this]() { updateFieldState(); });
    connect(ui->lineEmail, &QLineEdit::textChanged, this, [this]() { updateFieldState(); });
    connect(ui->lineLogin, &QLineEdit::textChanged, this, [this]() { updateFieldState(); });
    connect(ui->linePassword, &QLineEdit::textChanged, this, [this]() { updateFieldState(); });
    connect(ui->spinHeures, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, [this]() { updateFieldState(); });
}

void EmployeUserDialog::markInvalid(QWidget *widget, bool invalid, const QString &toolTip)
{
    widget->setProperty("invalid", invalid);
    widget->setToolTip(toolTip);
    widget->style()->unpolish(widget);
    widget->style()->polish(widget);
    widget->update();
}

void EmployeUserDialog::updateFieldState()
{
    const QString cinError = InputValidator::validateCIN(ui->lineCIN->text(), 8);
    markInvalid(ui->lineCIN, !cinError.isEmpty(), cinError);

    const QString nomError = InputValidator::validateName(ui->lineNom->text(), tr("Le nom"), true);
    markInvalid(ui->lineNom, !nomError.isEmpty(), nomError);

    const QString prenomError = InputValidator::validateName(ui->linePrenom->text(), tr("Le prenom"), true);
    markInvalid(ui->linePrenom, !prenomError.isEmpty(), prenomError);

    const QString emailError = InputValidator::validateEmail(ui->lineEmail->text());
    markInvalid(ui->lineEmail, !emailError.isEmpty(), emailError);

    const QString loginError = InputValidator::validateGeneralText(ui->lineLogin->text(), tr("Le login"), false);
    markInvalid(ui->lineLogin, !loginError.isEmpty(), loginError);

    const QString passError = EmployeDAO::validatePassword(ui->linePassword->text());
    markInvalid(ui->linePassword, !passError.isEmpty(), passError);

    const QString heuresError = InputValidator::validatePositiveDouble(ui->spinHeures->value(), tr("Les heures de travail"));
    markInvalid(ui->spinHeures, !heuresError.isEmpty(), heuresError);

    const bool allValid = cinError.isEmpty()
                       && nomError.isEmpty()
                       && prenomError.isEmpty()
                       && emailError.isEmpty()
                       && loginError.isEmpty()
                       && passError.isEmpty()
                       && heuresError.isEmpty();

    ui->btnOk->setEnabled(allValid);
}

bool EmployeUserDialog::validateForm(bool showMessage)
{
    const QString cinError = InputValidator::validateCIN(ui->lineCIN->text(), 8);
    if (!cinError.isEmpty()) {
        if (showMessage) QMessageBox::warning(this, tr("Saisie invalide"), cinError);
        return false;
    }

    if (!m_isEditMode) {
        const QString cinUniqueError = EmployeDAO::validateCINUnique(ui->lineCIN->text().trimmed());
        if (!cinUniqueError.isEmpty()) {
            if (showMessage) QMessageBox::warning(this, tr("Saisie invalide"), cinUniqueError);
            return false;
        }
    }

    const QString nomError = InputValidator::validateName(ui->lineNom->text(), tr("Le nom"), true);
    if (!nomError.isEmpty()) {
        if (showMessage) QMessageBox::warning(this, tr("Saisie invalide"), nomError);
        return false;
    }

    const QString prenomError = InputValidator::validateName(ui->linePrenom->text(), tr("Le prenom"), true);
    if (!prenomError.isEmpty()) {
        if (showMessage) QMessageBox::warning(this, tr("Saisie invalide"), prenomError);
        return false;
    }

    const QString emailError = InputValidator::validateEmail(ui->lineEmail->text());
    if (!emailError.isEmpty()) {
        if (showMessage) QMessageBox::warning(this, tr("Saisie invalide"), emailError);
        return false;
    }

    const QString loginError = InputValidator::validateGeneralText(ui->lineLogin->text(), tr("Le login"), false);
    if (!loginError.isEmpty()) {
        if (showMessage) QMessageBox::warning(this, tr("Saisie invalide"), loginError);
        return false;
    }

    const QString passwordError = EmployeDAO::validatePassword(ui->linePassword->text());
    if (!passwordError.isEmpty()) {
        if (showMessage) QMessageBox::warning(this, tr("Saisie invalide"), passwordError);
        return false;
    }

    const QString heuresError = InputValidator::validatePositiveDouble(ui->spinHeures->value(), tr("Les heures de travail"));
    if (!heuresError.isEmpty()) {
        if (showMessage) QMessageBox::warning(this, tr("Saisie invalide"), heuresError);
        return false;
    }

    return true;
}

} // namespace employes
