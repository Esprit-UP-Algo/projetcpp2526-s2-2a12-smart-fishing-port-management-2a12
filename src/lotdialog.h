#ifndef LOTDIALOG_H
#define LOTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDateTimeEdit>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class LotDialog : public QDialog {
    Q_OBJECT
public:
    enum Mode { Ajout, Modif };
    explicit LotDialog(Mode mode, QWidget *parent = nullptr);

    QString idLot() const;
    QString espece() const;
    int poids() const;
    QString zonePeche() const;
    QDateTime dateCapture() const;
    QString navire() const;
    QString qualite() const;
    QDateTime dateEntreeStock() const;
    QString etat() const;

    void setIdLot(const QString &);
    void setEspece(const QString &);
    void setPoids(int);
    void setZonePeche(const QString &);
    void setDateCapture(const QDateTime &);
    void setNavire(const QString &);
    void setQualite(const QString &);
    void setDateEntreeStock(const QDateTime &);
    void setEtat(const QString &);

private:
    QLabel *idLotLabel;
    QLineEdit *idLotEdit;
    QLabel *idLotSpacer;
    QLabel *idLotError;
    QLineEdit *especeEdit;
    QLabel *especeError;
    QSpinBox *poidsEdit;
    QLabel *poidsError;
    QLineEdit *zonePecheEdit;
    QLabel *zonePecheError;
    QDateTimeEdit *dateCaptureEdit;
    QLabel *dateCaptureError;
    QLineEdit *navireEdit;
    QLabel *navireError;
    QComboBox *qualiteEdit;
    QLabel *qualiteError;
    QDateTimeEdit *dateEntreeStockEdit;
    QLabel *dateEntreeStockError;
    QComboBox *etatEdit;
    QLabel *etatError;
    QPushButton *actionBtn;
    QPushButton *saveBtn;
    Mode m_mode;
};

#endif // LOTDIALOG_H
