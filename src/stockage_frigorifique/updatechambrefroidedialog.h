#pragma once

#include <QDialog>

class QLineEdit;
class QSpinBox;
class QDoubleSpinBox;
class QPushButton;

namespace stockage { struct ChambreFroide; }

namespace stockage {

class UpdateChambreFroideDialog final : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateChambreFroideDialog(const ChambreFroide &chambre, QWidget *parent = nullptr);

    ChambreFroide getChambreFroide() const;

private:
    void buildUi();
    void applyTheme();
    void wireSignals();

    void onModifier();

    QLineEdit *m_idChambre = nullptr;
    QSpinBox *m_capacite = nullptr;
    QDoubleSpinBox *m_tempConsigne = nullptr;
    QLineEdit *m_emplacement = nullptr;
    QDoubleSpinBox *m_temperature = nullptr;

    QPushButton *m_btnModifier = nullptr;
    QPushButton *m_btnEnregistrer = nullptr;
};

} // namespace stockage
