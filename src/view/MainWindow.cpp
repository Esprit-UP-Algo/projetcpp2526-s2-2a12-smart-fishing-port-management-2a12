#include "view/MainWindow.h"

#include "data/ModuleLoader.h"
#include "model/ModuleKey.h"

#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    buildUi();
    registerDefaultModules();

    // Defer loading to let the window paint quickly.
    QTimer::singleShot(0, this, [this]() {
        // Prefer Navires as the initial page, but only load that module at startup.
        if (!ensureModuleLoaded(ModuleKey::Navires))
        {
            // Fall back to first module that can be loaded.
            for (auto it = m_moduleBaseNames.constBegin(); it != m_moduleBaseNames.constEnd(); ++it)
            {
                if (ensureModuleLoaded(it.key()))
                    break;
            }
        }

        if (m_moduleWidgets.contains(ModuleKey::Navires))
            switchToModule(ModuleKey::Navires);
        else if (!m_moduleWidgets.isEmpty())
            switchToModule(m_moduleWidgets.constBegin().key());
    });
}

void MainWindow::buildUi()
{
    auto *root = new QWidget(this);
    auto *layout = new QHBoxLayout(root);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_stack = new QStackedWidget(root);
    layout->addWidget(m_stack, 1);

    m_placeholder = new QLabel(tr("Chargement…"), m_stack);
    m_placeholder->setAlignment(Qt::AlignCenter);
    m_placeholder->setWordWrap(true);
    m_stack->addWidget(m_placeholder);
    m_stack->setCurrentWidget(m_placeholder);

    setCentralWidget(root);
}

void MainWindow::registerDefaultModules()
{
    // Stable mapping between the in-app navigation key and the plugin DLL base name.
    // Loading is deferred; we only load a module when it is requested.
    m_moduleBaseNames.insert(ModuleKey::Employees, QStringLiteral("module_employees"));
    m_moduleBaseNames.insert(ModuleKey::Navires, QStringLiteral("module_navires"));
    m_moduleBaseNames.insert(ModuleKey::Quais, QStringLiteral("module_quais"));
    m_moduleBaseNames.insert(ModuleKey::Ventes, QStringLiteral("module_ventes"));
    m_moduleBaseNames.insert(ModuleKey::Captures, QStringLiteral("module_captures"));
    m_moduleBaseNames.insert(ModuleKey::Stockage, QStringLiteral("module_stockage"));
}

bool MainWindow::ensureModuleLoaded(const QString &moduleKey)
{
    if (moduleKey.isEmpty())
        return false;
    if (m_moduleWidgets.contains(moduleKey))
        return true;

    const QString baseName = m_moduleBaseNames.value(moduleKey);
    if (baseName.isEmpty())
    {
        qWarning().noquote() << "[integration_finale] Unknown module key:" << moduleKey;
        return false;
    }

    if (m_placeholder)
        m_placeholder->setText(tr("Chargement du module… (%1)").arg(moduleKey));

    ModuleLoader loader;
    QString errors;
    const LoadedModule m = loader.loadByBaseName(baseName, m_stack, &errors);

    if (!errors.trimmed().isEmpty())
        qWarning().noquote() << "[integration_finale] Module load warnings:\n" + errors.trimmed();

    if (!m.widget)
    {
        if (m_placeholder)
            m_placeholder->setText(tr("Impossible de charger le module: %1\n\n%2").arg(moduleKey, errors.trimmed()));
        return false;
    }

    m_stack->addWidget(m.widget);
    // Store by requested key (navigation uses ModuleKey::*), and also by plugin-reported key.
    m_moduleWidgets.insert(moduleKey, m.widget);
    if (!m.key.isEmpty() && m.key != moduleKey)
        m_moduleWidgets.insert(m.key, m.widget);

    // If the embedded widget exposes a signal "navigateRequested(QString)", hook it up.
    QObject::connect(m.widget, SIGNAL(navigateRequested(QString)), this, SLOT(onEmbeddedNavigateRequested(QString)));
    return true;
}

void MainWindow::setSidebarChecked(const QString &moduleKey)
{
    if (auto *btn = m_sidebarButtons.value(moduleKey, nullptr))
        btn->setChecked(true);
}

void MainWindow::switchToModule(const QString &moduleKey)
{
    // Load on demand.
    ensureModuleLoaded(moduleKey);

    QWidget *w = m_moduleWidgets.value(moduleKey, nullptr);
    if (!w)
    {
        qWarning().noquote() << "[integration_finale] switchToModule: module not loaded:" << moduleKey;
        return;
    }

    setSidebarChecked(moduleKey);

    // Prevent visual flicker: no widget recreation, just index swap.
    m_stack->setUpdatesEnabled(false);
    m_stack->setCurrentWidget(w);
    m_stack->setUpdatesEnabled(true);
    m_stack->update();

    w->setFocus(Qt::OtherFocusReason);
}

void MainWindow::onEmbeddedNavigateRequested(const QString &moduleKey)
{
    qWarning().noquote() << "[integration_finale] navigateRequested:" << moduleKey;
    switchToModule(moduleKey);
}
