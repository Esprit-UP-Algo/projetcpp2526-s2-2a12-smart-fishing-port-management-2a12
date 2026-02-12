#pragma once

#include <QDialog>

class QLineEdit;
class QSpinBox;
class QDateTimeEdit;
class QCheckBox;
class QComboBox;
class QPushButton;

namespace stockage {
struct StockageLot;
class ChambreFroideModel;
}

namespace stockage {

class UpdateStockageLotDialog final : public QDialog
{
    Q_OBJECT

public:
    explicit UpdateStockageLotDialog(const StockageLot &lot, ChambreFroideModel *chambreModel, QWidget *parent = nullptr);

    StockageLot getStockageLot() const;

private slots:
    void onModifier();
    void onEnregistrer();
    void onSortieCheckChanged(bool checked);

private:
    void buildUi();
    void applyTheme();
    void wireSignals();

    QLineEdit *m_idLot = nullptr;
    QLineEdit *m_espece = nullptr;
    QSpinBox *m_quantite = nullptr;
    QDateTimeEdit *m_entree = nullptr;
    QDateTimeEdit *m_sortie = nullptr;
    QCheckBox *m_sortieRenseignee = nullptr;
    QComboBox *m_chambreLot = nullptr;

    QPushButton *m_btnModifier = nullptr;
    QPushButton *m_btnEnregistrer = nullptr;

    ChambreFroideModel *m_chambreModel = nullptr;
};

} // namespace stockage
