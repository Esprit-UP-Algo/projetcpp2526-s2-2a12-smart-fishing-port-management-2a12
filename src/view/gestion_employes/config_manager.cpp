#include "config_manager.h"

#include <QProcessEnvironment>
#include <QSettings>

namespace {
const char *kOrganization = "VISION SIGHT";
const char *kApplication = "VISION SIGHT";

QString defaultValueForKey(const QString &key)
{
    if (key == "VS_SMTP_HOST") {
        return "smtp.gmail.com";
    }
    if (key == "VS_SMTP_PORT") {
        return "587";
    }
    if (key == "VS_SMTP_USER") {
        return "rchidmbarek313@gmail.com";
    }
    if (key == "VS_SMTP_PASS") {
        return "yxpw ttcb qcak pljd";
    }
    if (key == "VS_SMTP_FROM") {
        return "rchidmbarek313@gmail.com";
    }
    return {};
}
}

QString ConfigManager::settingsKey(const QString &key)
{
    return key.trimmed();
}

QString ConfigManager::get(const QString &key)
{
    const QString normalizedKey = settingsKey(key);
    if (normalizedKey.isEmpty()) {
        return {};
    }

    // 1) Environment variables have priority so deployments can override local values.
    const QProcessEnvironment environment = QProcessEnvironment::systemEnvironment();
    const QString envValue = environment.value(normalizedKey).trimmed();
    if (!envValue.isEmpty()) {
        return envValue;
    }

    // 2) Fallback to persistent local storage.
    QSettings settings(QSettings::IniFormat, QSettings::UserScope, kOrganization, kApplication);
    const QString settingsValue = settings.value(normalizedKey).toString().trimmed();
    if (!settingsValue.isEmpty()) {
        return settingsValue;
    }

    // 3) Last-resort defaults embedded in code.
    return defaultValueForKey(normalizedKey);
}

void ConfigManager::set(const QString &key, const QString &value)
{
    const QString normalizedKey = settingsKey(key);
    if (normalizedKey.isEmpty()) {
        return;
    }

    QSettings settings(QSettings::IniFormat, QSettings::UserScope, kOrganization, kApplication);
    settings.setValue(normalizedKey, value.trimmed());
    settings.sync();
}