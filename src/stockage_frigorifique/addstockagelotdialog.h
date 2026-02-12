#pragma once

#include <QDialog>
#include <functional>

class QLineEdit;
class QSpinBox;
class QDateTimeEdit;
class QCheckBox;
class QComboBox;
class QPushButton;

namespace stockage {
struct StockageLot;
struct ChambreFroide;
class ChambreFroideModel;
}

namespace stockage {

class AddStockageLotDialog final : public QDialog
{
    Q_OBJECT

public:
    explicit AddStockageLotDialog(ChambreFroideModel *chambreModel, QWidget *parent = nullptr);

    StockageLot getStockageLot() const;

    void setLotAddedCallback(std::function<bool(const StockageLot &)> callback);

private slots:
    void onAjouter();
    void onEnregistrer();
    void onSortieCheckChanged(bool checked);

private:
    void buildUi();
    void applyTheme();
    void wireSignals();
    void clearForm();

    QLineEdit *m_idLot = nullptr;
    QLineEdit *m_espece = nullptr;
    QSpinBox *m_quantite = nullptr;
    QDateTimeEdit *m_entree = nullptr;
    QDateTimeEdit *m_sortie = nullptr;
    QCheckBox *m_sortieRenseignee = nullptr;
    QComboBox *m_chambreLot = nullptr;

    QPushButton *m_btnAjouter = nullptr;
    QPushButton *m_btnEnregistrer = nullptr;

    ChambreFroideModel *m_chambreModel = nullptr;
    std::function<bool(const StockageLot &)> m_callback = nullptr;
};

} // namespace stockage
