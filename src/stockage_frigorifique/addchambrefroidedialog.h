#pragma once

#include <QDialog>
#include <functional>

class QLineEdit;
class QSpinBox;
class QDoubleSpinBox;
class QPushButton;

namespace stockage { struct ChambreFroide; }

namespace stockage {

class AddChambreFroideDialog final : public QDialog
{
    Q_OBJECT

public:
    explicit AddChambreFroideDialog(QWidget *parent = nullptr);

    ChambreFroide getChambreFroide() const;

    void setChambreAddedCallback(std::function<void(const ChambreFroide &)> callback);

private slots:

    void onAjouter();
    void onEnregistrer();

private:
    void buildUi();
    void applyTheme();
    void wireSignals();
    void clearForm();

    QLineEdit *m_idChambre = nullptr;
    QSpinBox *m_capacite = nullptr;
    QDoubleSpinBox *m_tempConsigne = nullptr;
    QLineEdit *m_emplacement = nullptr;
    QDoubleSpinBox *m_temperature = nullptr;

    QPushButton *m_btnAjouter = nullptr;
    QPushButton *m_btnEnregistrer = nullptr;

    std::function<void(const ChambreFroide &)> m_callback = nullptr;
};

} // namespace stockage
