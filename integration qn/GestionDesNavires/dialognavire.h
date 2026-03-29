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
    QLabel      *lblHint;
    QLineEdit   *editNom;
    QLabel      *errNom;
    QLineEdit   *editMatricule;
    QLabel      *errMatricule;
    QComboBox   *comboType;
    QSpinBox    *spinCapacite;
    QLabel      *errCapacite;
    QComboBox   *comboStatut;
    QLineEdit   *editPosition;
    QLabel      *errPosition;
    QPushButton *btnValider;
    QPushButton *btnAnnuler;

    bool editMode;

    void buildUi();
    void setupConnections();
    bool validateInputs(QString *errorMessage) const;
    void updateInlineErrors();
    void refreshOkEnabled();
};

#endif // DIALOGNAVIRE_H
