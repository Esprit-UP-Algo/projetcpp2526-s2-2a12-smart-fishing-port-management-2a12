#ifndef INPUT_VALIDATOR_H
#define INPUT_VALIDATOR_H

#include <QString>

class QRegularExpressionValidator;
class QObject;

namespace employes {

class InputValidator
{
public:
    static bool isValidName(const QString &value, bool allowSpaces = true);
    static bool isValidCIN(const QString &value, int fixedLength = 8);
    static bool isValidPhone(const QString &value, int fixedLength = 8);
    static bool isValidEmail(const QString &value);
    static bool isValidPositiveInt(int value);
    static bool isValidPositiveDouble(double value);
    static bool isValidGeneralText(const QString &value, bool allowSpaces = true);

    static QString validateName(const QString &value, const QString &fieldName, bool allowSpaces = true);
    static QString validateCIN(const QString &value, int fixedLength = 8);
    static QString validatePhone(const QString &value, int fixedLength = 8);
    static QString validateEmail(const QString &value);
    static QString validatePositiveInt(int value, const QString &fieldName);
    static QString validatePositiveDouble(double value, const QString &fieldName);
    static QString validateGeneralText(const QString &value, const QString &fieldName, bool allowSpaces = true);

    static QRegularExpressionValidator *createNameTypingValidator(QObject *parent = nullptr, bool allowSpaces = true);
    static QRegularExpressionValidator *createDigitsTypingValidator(QObject *parent = nullptr, int maxLength = 8);
    static QRegularExpressionValidator *createGeneralTextTypingValidator(QObject *parent = nullptr, bool allowSpaces = true);
};

} // namespace employes

#endif // INPUT_VALIDATOR_H
