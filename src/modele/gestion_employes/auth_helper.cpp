#include "auth_helper.h"

#include <QCryptographicHash>
#include <QRegularExpression>
#include <QDebug>

QString AuthHelper::hashPassword(const QString &password)
{
    if (password.isEmpty()) {
        return "";
    }
    
    QByteArray passwordBytes = password.toUtf8();
    QByteArray hashBytes = QCryptographicHash::hash(passwordBytes, QCryptographicHash::Sha256);
    return QString(hashBytes.toHex());
}

bool AuthHelper::verifyPassword(const QString &plainPassword, const QString &hashedPassword)
{
    if (plainPassword.isEmpty() || hashedPassword.isEmpty()) {
        return false;
    }
    
    QString computedHash = hashPassword(plainPassword);
    if (computedHash == hashedPassword) {
        return true;
    }
    
    if (plainPassword == hashedPassword) {
        return true;
    }
    
    return false;
}

QString AuthHelper::validateLoginInput(const QString &login)
{
    QString cleanLogin = login.trimmed();

    if (cleanLogin.isEmpty()) {
        return "Le login est obligatoire.";
    }

    return "";
}

QString AuthHelper::validatePasswordInput(const QString &password)
{
    if (password.isEmpty()) {
        return "Le mot de passe est obligatoire.";
    }

    return "";
}

bool AuthHelper::isValidEmail(const QString &email)
{
    return email.contains('@') && email.contains('.');
}

bool AuthHelper::isAllDigits(const QString &str)
{
    if (str.isEmpty()) return false;
    
    for (const QChar &c : str) {
        if (!c.isDigit()) {
            return false;
        }
    }
    
    return true;
}