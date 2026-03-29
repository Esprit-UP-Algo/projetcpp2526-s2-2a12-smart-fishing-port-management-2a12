#ifndef SECUREPASSWORD_H
#define SECUREPASSWORD_H

#include <QString>

QString protectSecret(const QString &plain, QString *errorMessage = nullptr);
QString unprotectSecret(const QString &protectedBase64, QString *errorMessage = nullptr);

#endif // SECUREPASSWORD_H
