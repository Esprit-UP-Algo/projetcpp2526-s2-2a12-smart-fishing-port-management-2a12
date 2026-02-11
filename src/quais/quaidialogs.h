#pragma once

#include <QDialog>

class QComboBox;
class QDateTimeEdit;
class QDoubleSpinBox;
class QLabel;
class QLineEdit;
class QPushButton;

struct Quai;

class QuaiFormDialog final : public QDialog
{
    Q_OBJECT

public:
    enum class Mode
    {
        Create = 0,
        Update
    };

    explicit QuaiFormDialog(Mode mode, QWidget *parent = nullptr);

    void setQuai(const Quai &q);
    Quai quai() const;

private:
    void buildUi();
    void applyMode();

    Mode m_mode;

    QString m_currentIdQuai;

    QLineEdit *m_matricule = nullptr;
    QDoubleSpinBox *m_taille = nullptr;
    QDoubleSpinBox *m_posX = nullptr;
    QDoubleSpinBox *m_posY = nullptr;
    QDateTimeEdit *m_arrivee = nullptr;
    QDateTimeEdit *m_depart = nullptr;
    QComboBox *m_etat = nullptr;
    QDoubleSpinBox *m_prix = nullptr;

    QPushButton *m_btnOk = nullptr;
    QPushButton *m_btnCancel = nullptr;
};

class QuaiDeleteDialog final : public QDialog
{
    Q_OBJECT

public:
    explicit QuaiDeleteDialog(QWidget *parent = nullptr);

    void setQuaiSummary(const QString &idQuai, const QString &matricule);

private:
    QLabel *m_title = nullptr;
    QLabel *m_sub = nullptr;
    QPushButton *m_btnDelete = nullptr;
    QPushButton *m_btnCancel = nullptr;
};
