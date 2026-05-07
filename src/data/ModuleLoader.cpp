#include "data/ModuleLoader.h"

#include "model/ModulePageInterface.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QElapsedTimer>
#include <QtCore/QDir>
#include <QtCore/QFileInfo>
#include <QtCore/QLibrary>
#include <QtCore/QPluginLoader>
#include <QtCore/QDebug>
#include <QtWidgets/QWidget>

namespace {
QStringList candidatePluginFileNames(const QString &baseName)
{
    // QLibrary::suffixes() returns things like ["dll"] on Windows.
    // Keep it simple and platform-specific.
#if defined(Q_OS_WIN)
    const QStringList suffixes = {QStringLiteral("dll")};
#elif defined(Q_OS_MAC)
    const QStringList suffixes = {QStringLiteral("dylib"), QStringLiteral("so")};
#else
    const QStringList suffixes = {QStringLiteral("so")};
#endif
    QStringList candidates;

    for (const QString &suffix : suffixes)
        candidates.push_back(baseName + QStringLiteral(".") + suffix);

    // Also try with a lib prefix for non-Windows toolchains.
    for (const QString &suffix : suffixes)
        candidates.push_back(QStringLiteral("lib") + baseName + QStringLiteral(".") + suffix);

    return candidates;
}

QStringList pluginSearchDirs()
{
    const QString appDir = QCoreApplication::applicationDirPath();
    return {
        appDir,
        QDir(appDir).absoluteFilePath(QStringLiteral("plugins")),
    };
}
}

ModuleLoader::ModuleLoader(QObject *parent) : QObject(parent)
{
}

LoadedModule ModuleLoader::loadByBaseName(const QString &baseName, QWidget *pageParent, QString *error)
{
    return loadOne(baseName, pageParent, error);
}

QString ModuleLoader::findPluginFile(const QString &baseName) const
{
    const QStringList searchDirs = pluginSearchDirs();
    const QStringList candidates = candidatePluginFileNames(baseName);

    for (const QString &dirPath : searchDirs)
    {
        const QDir dir(dirPath);
        for (const QString &candidate : candidates)
        {
            const QString fullPath = dir.absoluteFilePath(candidate);
            if (QFileInfo::exists(fullPath))
                return fullPath;
        }

        // Fallback: if naming differs, try to locate any library that contains the baseName.
        const QFileInfoList matches = dir.entryInfoList({QStringLiteral("*") + baseName + QStringLiteral("*")}, QDir::Files);
        for (const QFileInfo &fi : matches)
        {
            if (QLibrary::isLibrary(fi.absoluteFilePath()))
                return fi.absoluteFilePath();
        }
    }

    return {};
}

LoadedModule ModuleLoader::loadOne(const QString &baseName, QWidget *pageParent, QString *error)
{
    LoadedModule out;

    QElapsedTimer timer;
    timer.start();

    const QString pluginPath = findPluginFile(baseName);
    if (pluginPath.isEmpty())
    {
        if (error)
            *error += QStringLiteral("Plugin not found: %1\n").arg(baseName);
        return out;
    }

    const qint64 findMs = timer.elapsed();

    // Parent to the page container so the loader (and thus the plugin) stays alive
    // for the lifetime of the UI.
    auto *loader = new QPluginLoader(pluginPath, pageParent);
    QObject *instance = loader->instance();
    if (!instance)
    {
        if (error)
            *error += QStringLiteral("Failed to load %1: %2\n").arg(pluginPath, loader->errorString());
        delete loader;
        return out;
    }

    const qint64 instanceMs = timer.elapsed();

    auto *module = qobject_cast<ModulePageInterface *>(instance);
    if (!module)
    {
        if (error)
            *error += QStringLiteral("Invalid plugin (does not implement ModulePageInterface): %1\n").arg(pluginPath);
        loader->unload();
        delete loader;
        return out;
    }

    QWidget *w = module->createWidget(pageParent);
    if (!w)
    {
        if (error)
            *error += QStringLiteral("Plugin returned null widget: %1\n").arg(pluginPath);
        loader->unload();
        delete loader;
        return out;
    }

    const qint64 widgetMs = timer.elapsed();

    out.key = module->moduleKey();
    out.displayName = module->moduleDisplayName();
    out.widget = w;
    out.loader = loader;

    qInfo().noquote() << QStringLiteral("[integration_finale] Loaded %1 (%2) in %3ms (find:%4ms instance:%5ms widget:%6ms) from %7")
                             .arg(baseName, out.key)
                             .arg(timer.elapsed())
                             .arg(findMs)
                             .arg(instanceMs)
                             .arg(widgetMs)
                             .arg(pluginPath);
    return out;
}

QVector<LoadedModule> ModuleLoader::loadDefaultModules(QWidget *pageParent, QString *error)
{
    QVector<LoadedModule> out;
    out.reserve(6);

    // Keep these base names stable: the CMake files set target names to match.
    for (const QString &baseName : {
             QStringLiteral("module_employees"),
             QStringLiteral("module_navires"),
             QStringLiteral("module_quais"),
             QStringLiteral("module_ventes"),
             QStringLiteral("module_captures"),
             QStringLiteral("module_stockage"),
         })
    {
        const LoadedModule m = loadOne(baseName, pageParent, error);
        if (m.widget)
            out.push_back(m);
    }

    return out;
}
