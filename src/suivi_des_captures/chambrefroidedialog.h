#ifndef CHAMBREFROIDEDIALOG_H
#define CHAMBREFROIDEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class ChambreFroideDialog : public QDialog {
    Q_OBJECT
public:
    enum Mode { Ajout, Modif };
    explicit ChambreFroideDialog(Mode mode, QWidget *parent = nullptr);

    QString chambre() const;
    int capacite() const;
    double tempConsigne() const;
    QString emplacement() const;
    double temperature() const;

    void setChambre(const QString &);
    void setCapacite(int);
    void setTempConsigne(double);
    void setEmplacement(const QString &);
    void setTemperature(double);

private:
    QLineEdit *chambreEdit;
    QLabel *chambreError;
    QSpinBox *capaciteEdit;
    QLabel *capaciteError;
    QDoubleSpinBox *tempConsigneEdit;
    QLabel *tempConsigneError;
    QLineEdit *emplacementEdit;
    QLabel *emplacementError;
    QDoubleSpinBox *temperatureEdit;
    QLabel *temperatureError;
    QPushButton *actionBtn;
    QPushButton *saveBtn;
    Mode m_mode;
};

#endif // CHAMBREFROIDEDIALOG_H
