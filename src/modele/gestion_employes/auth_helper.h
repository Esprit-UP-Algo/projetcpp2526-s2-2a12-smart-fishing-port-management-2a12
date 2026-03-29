#ifndef AUTH_HELPER_H
#define AUTH_HELPER_H

#include <QString>

class AuthHelper
{
public:
    // Hashing & Verification
    static QString hashPassword(const QString &password);
    static bool verifyPassword(const QString &plainPassword, const QString &hashedPassword);
    
    // Validation
    static QString validateLoginInput(const QString &login);
    static QString validatePasswordInput(const QString &password);
    
private:
    static bool isValidEmail(const QString &email);
    static bool isAllDigits(const QString &str);
};

#endif // AUTH_HELPER_H
