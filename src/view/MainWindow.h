#pragma once

#include <QtWidgets/QMainWindow>

#include <QtCore/QHash>
#include <QtCore/QPointer>
#include <QtCore/QString>

class QButtonGroup;
class QPushButton;
class QStackedWidget;
class QLabel;

class MainWindow final : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void switchToModule(const QString &moduleKey);
    void onEmbeddedNavigateRequested(const QString &moduleKey);

private:
    void buildUi();
    void registerDefaultModules();
    bool ensureModuleLoaded(const QString &moduleKey);
    void setSidebarChecked(const QString &moduleKey);

    QButtonGroup *m_sidebarGroup = nullptr;
    QStackedWidget *m_stack = nullptr;

    QLabel *m_placeholder = nullptr;

    // ModuleKey -> plugin target base name (DLL name without suffix)
    QHash<QString, QString> m_moduleBaseNames;

    QHash<QString, QPushButton *> m_sidebarButtons;
    QHash<QString, QWidget *> m_moduleWidgets;
};
