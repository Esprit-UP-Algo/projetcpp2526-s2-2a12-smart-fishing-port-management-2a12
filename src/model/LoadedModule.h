#pragma once

#include <QtCore/QString>

class QPluginLoader;
class QWidget;

struct LoadedModule
{
    QString key;
    QString displayName;
    QWidget *widget = nullptr;
    QPluginLoader *loader = nullptr;
};
