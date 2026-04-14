#include "input_validator.h"

#include <QRegularExpression>
#include <QRegularExpressionValidator>

namespace employes {

namespace {
QRegularExpression nameRegex(bool allowSpaces)
{
    return allowSpaces
        ? QRegularExpression("^[A-Za-z]+(?: [A-Za-z]+)*$")
        : QRegularExpression("^[A-Za-z]+$");
}

QRegularExpression nameTypingRegex(bool allowSpaces)
{
    return allowSpaces
        ? QRegularExpression("^[A-Za-z ]*$")
        : QRegularExpression("^[A-Za-z]*$");
}

QRegularExpression digitsRegex(int fixedLength)
{
    return QRegularExpression(QString("^\\d{%1}$").arg(fixedLength));
}

QRegularExpression digitsTypingRegex(int maxLength)
{
    return QRegularExpression(QString("^\\d{0,%1}$").arg(maxLength));
}

QRegularExpression generalTextRegex(bool allowSpaces)
{
    return allowSpaces
        ? QRegularExpression("^[A-Za-z0-9 ]+$")
        : QRegularExpression("^[A-Za-z0-9]+$");
}

QRegularExpression generalTextTypingRegex(bool allowSpaces)
{
    return allowSpaces
        ? QRegularExpression("^[A-Za-z0-9 ]*$")
        : QRegularExpression("^[A-Za-z0-9]*$");
}
}

bool InputValidator::isValidName(const QString &value, bool allowSpaces)
{
    const QString text = value.trimmed();
    if (text.isEmpty()) {
        return false;
    }
    return nameRegex(allowSpaces).match(text).hasMatch();
}

bool InputValidator::isValidCIN(const QString &value, int fixedLength)
{
    return digitsRegex(fixedLength).match(value.trimmed()).hasMatch();
}

bool InputValidator::isValidPhone(const QString &value, int fixedLength)
{
    return digitsRegex(fixedLength).match(value.trimmed()).hasMatch();
}

bool InputValidator::isValidEmail(const QString &value)
{
    const QString text = value.trimmed();
    if (text.isEmpty()) {
        return false;
    }

    static const QRegularExpression emailRegex(
        "^[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\\.[A-Za-z]{2,}$");
    return emailRegex.match(text).hasMatch();
}

bool InputValidator::isValidPositiveInt(int value)
{
    return value >= 0;
}

bool InputValidator::isValidPositiveDouble(double value)
{
    return value >= 0.0;
}

bool InputValidator::isValidGeneralText(const QString &value, bool allowSpaces)
{
    const QString text = value.trimmed();
    if (text.isEmpty()) {
        return false;
    }
    return generalTextRegex(allowSpaces).match(text).hasMatch();
}

QString InputValidator::validateName(const QString &value, const QString &fieldName, bool allowSpaces)
{
    const QString text = value.trimmed();
    if (text.isEmpty()) {
        return fieldName + " est obligatoire.";
    }
    if (!isValidName(text, allowSpaces)) {
        return fieldName + " doit contenir uniquement des lettres"
               + QString(allowSpaces ? " et des espaces." : ".");
    }
    return "";
}

QString InputValidator::validateCIN(const QString &value, int fixedLength)
{
    const QString text = value.trimmed();
    if (text.isEmpty()) {
        return "Le CIN est obligatoire.";
    }
    if (!isValidCIN(text, fixedLength)) {
        return QString("Le CIN doit contenir exactement %1 chiffres.").arg(fixedLength);
    }
    return "";
}

QString InputValidator::validatePhone(const QString &value, int fixedLength)
{
    const QString text = value.trimmed();
    if (text.isEmpty()) {
        return "Le numero de telephone est obligatoire.";
    }
    if (!isValidPhone(text, fixedLength)) {
        return QString("Le numero de telephone doit contenir exactement %1 chiffres.").arg(fixedLength);
    }
    return "";
}

QString InputValidator::validateEmail(const QString &value)
{
    const QString text = value.trimmed();
    if (text.isEmpty()) {
        return "L'email est obligatoire.";
    }
    if (!isValidEmail(text)) {
        return "L'email n'est pas valide (exemple: nom@domaine.com).";
    }
    return "";
}

QString InputValidator::validatePositiveInt(int value, const QString &fieldName)
{
    if (!isValidPositiveInt(value)) {
        return fieldName + " doit etre un nombre positif ou nul.";
    }
    return "";
}

QString InputValidator::validatePositiveDouble(double value, const QString &fieldName)
{
    if (!isValidPositiveDouble(value)) {
        return fieldName + " doit etre un nombre positif ou nul.";
    }
    return "";
}

QString InputValidator::validateGeneralText(const QString &value, const QString &fieldName, bool allowSpaces)
{
    const QString text = value.trimmed();
    if (text.isEmpty()) {
        return fieldName + " est obligatoire.";
    }
    if (!isValidGeneralText(text, allowSpaces)) {
        return fieldName + " contient des caracteres non autorises.";
    }
    return "";
}

QRegularExpressionValidator *InputValidator::createNameTypingValidator(QObject *parent, bool allowSpaces)
{
    return new QRegularExpressionValidator(nameTypingRegex(allowSpaces), parent);
}

QRegularExpressionValidator *InputValidator::createDigitsTypingValidator(QObject *parent, int maxLength)
{
    return new QRegularExpressionValidator(digitsTypingRegex(maxLength), parent);
}

QRegularExpressionValidator *InputValidator::createGeneralTextTypingValidator(QObject *parent, bool allowSpaces)
{
    return new QRegularExpressionValidator(generalTextTypingRegex(allowSpaces), parent);
}

} // namespace employes
