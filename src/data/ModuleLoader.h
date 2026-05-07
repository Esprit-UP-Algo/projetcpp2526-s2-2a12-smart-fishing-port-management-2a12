#pragma once

#include <QtCore/QObject>
#include <QtCore/QVector>

#include "model/LoadedModule.h"

class QWidget;

class ModuleLoader final : public QObject
{
    Q_OBJECT

public:
    explicit ModuleLoader(QObject *parent = nullptr);

    QVector<LoadedModule> loadDefaultModules(QWidget *pageParent, QString *error = nullptr);

    // Load a single plugin module by its base target name (e.g. "module_navires").
    // Returns an empty LoadedModule (widget == nullptr) on failure.
    LoadedModule loadByBaseName(const QString &baseName, QWidget *pageParent, QString *error = nullptr);

private:
    QString findPluginFile(const QString &baseName) const;
    LoadedModule loadOne(const QString &baseName, QWidget *pageParent, QString *error = nullptr);
};
