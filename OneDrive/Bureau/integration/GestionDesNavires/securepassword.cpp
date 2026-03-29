#include "securepassword.h"

#include <QByteArray>

#ifdef Q_OS_WIN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <wincrypt.h>
#endif

QString protectSecret(const QString &plain, QString *errorMessage)
{
    if (plain.isEmpty())
        return {};

#ifdef Q_OS_WIN
    const QByteArray input = plain.toUtf8();

    DATA_BLOB inBlob;
    inBlob.cbData = static_cast<DWORD>(input.size());
    inBlob.pbData = reinterpret_cast<BYTE *>(const_cast<char *>(input.constData()));

    DATA_BLOB outBlob;
    outBlob.cbData = 0;
    outBlob.pbData = nullptr;

    if (!CryptProtectData(&inBlob, L"", nullptr, nullptr, nullptr, 0, &outBlob)) {
        if (errorMessage)
            *errorMessage = QStringLiteral("CryptProtectData a échoué (code %1)").arg(static_cast<qulonglong>(GetLastError()));
        return {};
    }

    const QByteArray encrypted(reinterpret_cast<const char *>(outBlob.pbData), static_cast<int>(outBlob.cbData));
    LocalFree(outBlob.pbData);

    return QString::fromLatin1(encrypted.toBase64());
#else
    Q_UNUSED(errorMessage);
    return plain;
#endif
}

QString unprotectSecret(const QString &protectedBase64, QString *errorMessage)
{
    if (protectedBase64.isEmpty())
        return {};

#ifdef Q_OS_WIN
    const QByteArray encrypted = QByteArray::fromBase64(protectedBase64.toLatin1());

    DATA_BLOB inBlob;
    inBlob.cbData = static_cast<DWORD>(encrypted.size());
    inBlob.pbData = reinterpret_cast<BYTE *>(const_cast<char *>(encrypted.constData()));

    DATA_BLOB outBlob;
    outBlob.cbData = 0;
    outBlob.pbData = nullptr;

    if (!CryptUnprotectData(&inBlob, nullptr, nullptr, nullptr, nullptr, 0, &outBlob)) {
        if (errorMessage)
            *errorMessage = QStringLiteral("CryptUnprotectData a échoué (code %1)").arg(static_cast<qulonglong>(GetLastError()));
        return {};
    }

    const QByteArray plain(reinterpret_cast<const char *>(outBlob.pbData), static_cast<int>(outBlob.cbData));
    LocalFree(outBlob.pbData);

    return QString::fromUtf8(plain);
#else
    Q_UNUSED(errorMessage);
    return protectedBase64;
#endif
}
