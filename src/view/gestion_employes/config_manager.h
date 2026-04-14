#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <QString>

class ConfigManager
{
public:
    static QString get(const QString &key);
    static void set(const QString &key, const QString &value);

private:
    static QString settingsKey(const QString &key);
};

#endif // CONFIG_MANAGER_H