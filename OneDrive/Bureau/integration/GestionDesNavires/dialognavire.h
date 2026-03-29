#ifndef DIALOGNAVIRE_H
#define DIALOGNAVIRE_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include "navire.h"

class DialogNavire : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNavire(QWidget *parent = nullptr);
    explicit DialogNavire(const Navire &navire, QWidget *parent = nullptr);
    ~DialogNavire();

    Navire getNavire() const;

private:
    QLabel      *lblDialogTitle;
    QLineEdit   *editNom;
    QLineEdit   *editMatricule;
    QComboBox   *comboType;
    QSpinBox    *spinCapacite;
    QComboBox   *comboStatut;
    QLineEdit   *editPosition;
    QPushButton *btnValider;
    QPushButton *btnAnnuler;

    bool editMode;

    void buildUi();
    void setupConnections();
    bool validateInputs(QString *errorMessage) const;
};

#endif // DIALOGNAVIRE_H
