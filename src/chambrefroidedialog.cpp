#include "chambrefroidedialog.h"
#include <QFormLayout>

ChambreFroideDialog::ChambreFroideDialog(Mode mode, QWidget *parent)
    : QDialog(parent), m_mode(mode)
{
    setWindowTitle(mode == Ajout ? tr("Créer une chambre froide") : tr("Modifier une chambre froide"));
    setStyleSheet(R"(
        QDialog {
            background: #101c2c;
            border-radius: 16px;
        }
        QLabel {
            color: #e6eef6;
            font-size: 14px;
            font-weight: bold;
        }
        QLineEdit, QSpinBox, QDoubleSpinBox {
            background: #18243A;
            color: #e6eef6;
            border: 1px solid #23374e;
            border-radius: 8px;
            font-size: 15px;
            min-height: 32px;
            padding: 6px 10px;
        }
        QPushButton {
            min-width: 120px;
            min-height: 40px;
            border-radius: 10px;
            font-size: 15px;
            font-weight: bold;
        }
        QPushButton#ActionBtn {
            background: #36b6ff;
            color: #101c2c;
        }
        QPushButton#SaveBtn {
            background: #23374e;
            color: #e6eef6;
        }
    )");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(18, 16, 18, 16);
    mainLayout->setSpacing(10);
    QLabel *title = new QLabel(mode == Ajout ? tr("Créer une chambre froide") : tr("Modifier une chambre froide"), this);
    mainLayout->addWidget(title);
    mainLayout->addSpacing(6);

    chambreEdit = new QLineEdit(this);
    chambreEdit->setPlaceholderText(tr("Ex: CF-01"));
    chambreError = new QLabel(this);
    chambreError->setStyleSheet("color: #ff4d4d; font-size: 13px;");
    chambreError->setVisible(false);
    chambreError->setWordWrap(true);

    capaciteEdit = new QSpinBox(this);
    capaciteEdit->setMaximum(100000);
    capaciteEdit->setPrefix(tr("m³ "));
    capaciteError = new QLabel(this);
    capaciteError->setStyleSheet("color: #ff4d4d; font-size: 13px;");
    capaciteError->setVisible(false);
    capaciteError->setWordWrap(true);

    tempConsigneEdit = new QDoubleSpinBox(this);
    tempConsigneEdit->setDecimals(2);
    tempConsigneEdit->setMinimum(-100);
    tempConsigneEdit->setMaximum(100);
    tempConsigneEdit->setSuffix(tr(" °C"));
    tempConsigneError = new QLabel(this);
    tempConsigneError->setStyleSheet("color: #ff4d4d; font-size: 13px;");
    tempConsigneError->setVisible(false);
    tempConsigneError->setWordWrap(true);

    emplacementEdit = new QLineEdit(this);
    emplacementEdit->setPlaceholderText(tr("Ex: Zone A"));
    emplacementError = new QLabel(this);
    emplacementError->setStyleSheet("color: #ff4d4d; font-size: 13px;");
    emplacementError->setVisible(false);
    emplacementError->setWordWrap(true);

    temperatureEdit = new QDoubleSpinBox(this);
    temperatureEdit->setDecimals(2);
    temperatureEdit->setMinimum(-100);
    temperatureEdit->setMaximum(100);
    temperatureEdit->setSuffix(tr(" °C"));
    temperatureError = new QLabel(this);
    temperatureError->setStyleSheet("color: #ff4d4d; font-size: 13px;");
    temperatureError->setVisible(false);
    temperatureError->setWordWrap(true);

    QFormLayout *form = new QFormLayout();
    form->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    form->setFormAlignment(Qt::AlignTop);
    form->setHorizontalSpacing(12);
    form->setVerticalSpacing(6);

    auto addRowWithError = [&](const QString &label, QWidget *field, QLabel *error) {
        auto *lab = new QLabel(label, this);
        form->addRow(lab, field);
        auto *spacer = new QLabel("", this);
        spacer->setMinimumWidth(80);
        form->addRow(spacer, error);
    };

    addRowWithError(tr("Chambre"), chambreEdit, chambreError);
    addRowWithError(tr("Capacité"), capaciteEdit, capaciteError);
    addRowWithError(tr("Temp Consigne"), tempConsigneEdit, tempConsigneError);
    addRowWithError(tr("Emplacement"), emplacementEdit, emplacementError);
    addRowWithError(tr("Température"), temperatureEdit, temperatureError);

    mainLayout->addLayout(form);

    QHBoxLayout *btnLayout = new QHBoxLayout;
    actionBtn = new QPushButton(mode == Ajout ? tr("Ajouter") : tr("Modifier"), this);
    actionBtn->setObjectName("ActionBtn");
    saveBtn = new QPushButton(tr("Enregistrer"), this);
    saveBtn->setObjectName("SaveBtn");
    btnLayout->addWidget(actionBtn);
    btnLayout->addWidget(saveBtn);
    mainLayout->addSpacing(16);
    mainLayout->addLayout(btnLayout);

    setFixedSize(500, 460);
}

QString ChambreFroideDialog::chambre() const { return chambreEdit->text(); }
int ChambreFroideDialog::capacite() const { return capaciteEdit->value(); }
double ChambreFroideDialog::tempConsigne() const { return tempConsigneEdit->value(); }
QString ChambreFroideDialog::emplacement() const { return emplacementEdit->text(); }
double ChambreFroideDialog::temperature() const { return temperatureEdit->value(); }

void ChambreFroideDialog::setChambre(const QString &v) { chambreEdit->setText(v); }
void ChambreFroideDialog::setCapacite(int v) { capaciteEdit->setValue(v); }
void ChambreFroideDialog::setTempConsigne(double v) { tempConsigneEdit->setValue(v); }
void ChambreFroideDialog::setEmplacement(const QString &v) { emplacementEdit->setText(v); }
void ChambreFroideDialog::setTemperature(double v) { temperatureEdit->setValue(v); }
