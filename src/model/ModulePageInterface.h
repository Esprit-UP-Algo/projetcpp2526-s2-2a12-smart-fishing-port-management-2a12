#pragma once

#include <QtCore/QtGlobal>
#include <QtCore/QString>

class QWidget;

class ModulePageInterface
{
public:
    virtual ~ModulePageInterface() = default;

    virtual QString moduleKey() const = 0;
    virtual QString moduleDisplayName() const = 0;

    // Must return a widget that can be embedded into another window.
    // Ownership: Qt parent/child (parent will own it).
    virtual QWidget *createWidget(QWidget *parent) = 0;
};

#define ModulePageInterface_iid "com.visionsight.IntegrationFinale.ModulePageInterface/1.0"
Q_DECLARE_INTERFACE(ModulePageInterface, ModulePageInterface_iid)
