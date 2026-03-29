#pragma once

#include <QDateTime>
#include <QDialog>
#include <QString>

class QShowEvent;

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

protected:
    void accept() override;
    void showEvent(QShowEvent *event) override;

private:
    void buildUi();
    void applyMode();

    bool validateInputs(QString *error) const;
    void updateInlineErrors();
    void refreshOkEnabled();
    void refreshDateTimeEnabled();
    void refreshDateTimeConstraints();

    Mode m_mode;

    QString m_currentIdQuai;

    QDateTime m_originalArrivee;
    QDateTime m_originalDepart;

    QLineEdit *m_matricule = nullptr;
    QLabel *m_matriculeError = nullptr;
    QDoubleSpinBox *m_taille = nullptr;
    QLabel *m_tailleError = nullptr;
    QDoubleSpinBox *m_posX = nullptr;
    QLabel *m_posXError = nullptr;
    QDoubleSpinBox *m_posY = nullptr;
    QLabel *m_posYError = nullptr;
    QDateTimeEdit *m_arrivee = nullptr;
    QLabel *m_arriveeError = nullptr;
    QDateTimeEdit *m_depart = nullptr;
    QLabel *m_departError = nullptr;
    QComboBox *m_etat = nullptr;
    QLabel *m_etatError = nullptr;
    QDoubleSpinBox *m_prix = nullptr;
    QLabel *m_prixError = nullptr;

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
