#ifndef DIALOGMOUVEMENT_H
#define DIALOGMOUVEMENT_H

#include <QDialog>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QStringList>
#include "navire.h"

class DialogMouvement : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMouvement(const QStringList &navireNames, QWidget *parent = nullptr);
    ~DialogMouvement();

    Mouvement getMouvement() const;

private:
    QLabel          *lblDialogTitle;
    QComboBox       *comboNavire;
    QDateTimeEdit   *dtDepart;
    QDateTimeEdit   *dtRetour;
    QLineEdit       *editCargaison;
    QDoubleSpinBox  *spinDistance;
    QPushButton     *btnValider;
    QPushButton     *btnAnnuler;

    void buildUi();
    void setupConnections();
};

#endif // DIALOGMOUVEMENT_H
