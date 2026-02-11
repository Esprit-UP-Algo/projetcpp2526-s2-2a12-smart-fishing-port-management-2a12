#include "lotdialog.h"
#include <QFormLayout>
#include <QHBoxLayout>

LotDialog::LotDialog(Mode mode, QWidget *parent)
    : QDialog(parent), m_mode(mode)
{
    setWindowTitle(mode == Ajout ? tr("Créer un lot de poissons") : tr("Modifier un lot de poissons"));
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
        QLineEdit, QSpinBox, QDoubleSpinBox, QDateTimeEdit, QComboBox {
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
    QLabel *title = new QLabel(mode == Ajout ? tr("Créer un lot de poissons") : tr("Modifier un lot de poissons"), this);
    mainLayout->addWidget(title);
    mainLayout->addSpacing(6);

    idLotEdit = new QLineEdit(this);
    idLotEdit->setPlaceholderText(tr("Ex: LOT-001"));
    idLotError = new QLabel(this);
    idLotError->setStyleSheet("color: #ff4d4d; font-size: 13px;");
    idLotError->setVisible(false);
    idLotError->setWordWrap(true);

    especeEdit = new QLineEdit(this);
    especeEdit->setPlaceholderText(tr("Ex: Sardine"));
    especeError = new QLabel(this);
    especeError->setStyleSheet("color: #ff4d4d; font-size: 13px;");
    especeError->setVisible(false);
    especeError->setWordWrap(true);

    poidsEdit = new QSpinBox(this);
    poidsEdit->setMaximum(100000);
    poidsEdit->setPrefix(tr("kg "));
    poidsError = new QLabel(this);
    poidsError->setStyleSheet("color: #ff4d4d; font-size: 13px;");
    poidsError->setVisible(false);
    poidsError->setWordWrap(true);

    zonePecheEdit = new QLineEdit(this);
    zonePecheEdit->setPlaceholderText(tr("Ex: Atlantique"));
    zonePecheError = new QLabel(this);
    zonePecheError->setStyleSheet("color: #ff4d4d; font-size: 13px;");
    zonePecheError->setVisible(false);
    zonePecheError->setWordWrap(true);

    dateCaptureEdit = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    dateCaptureError = new QLabel(this);
    dateCaptureError->setStyleSheet("color: #ff4d4d; font-size: 13px;");
    dateCaptureError->setVisible(false);
    dateCaptureError->setWordWrap(true);

    navireEdit = new QLineEdit(this);
    navireEdit->setPlaceholderText(tr("Ex: Mohsen"));
    navireError = new QLabel(this);
    navireError->setStyleSheet("color: #ff4d4d; font-size: 13px;");
    navireError->setVisible(false);
    navireError->setWordWrap(true);

    qualiteEdit = new QComboBox(this);
    qualiteEdit->addItems({tr("Auto"), tr("Premium"), tr("Standard"), tr("Économique"), tr("Déclassé")});
    qualiteError = new QLabel(this);
    qualiteError->setStyleSheet("color: #ff4d4d; font-size: 13px;");
    qualiteError->setVisible(false);
    qualiteError->setWordWrap(true);

    etatEdit = new QComboBox(this);
    etatEdit->addItems({tr("En stock"), tr("Vendu"), tr("En transit"), tr("Déclassé")});
    etatError = new QLabel(this);
    etatError->setStyleSheet("color: #ff4d4d; font-size: 13px;");
    etatError->setVisible(false);
    etatError->setWordWrap(true);

    dateEntreeStockEdit = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    dateEntreeStockError = new QLabel(this);
    dateEntreeStockError->setStyleSheet("color: #ff4d4d; font-size: 13px;");
    dateEntreeStockError->setVisible(false);
    dateEntreeStockError->setWordWrap(true);

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

    addRowWithError(tr("ID Lot"), idLotEdit, idLotError);
    addRowWithError(tr("Espèce"), especeEdit, especeError);
    addRowWithError(tr("Poids"), poidsEdit, poidsError);
    addRowWithError(tr("Zone Pêche"), zonePecheEdit, zonePecheError);
    addRowWithError(tr("Date Capture"), dateCaptureEdit, dateCaptureError);
    addRowWithError(tr("Etat"), etatEdit, etatError);
    addRowWithError(tr("Navire"), navireEdit, navireError);
    addRowWithError(tr("Qualité"), qualiteEdit, qualiteError);
    addRowWithError(tr("Date Entrée Stock"), dateEntreeStockEdit, dateEntreeStockError);

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

    auto validateAndAccept = [this]() {
        bool valid = true;
        idLotError->setVisible(false);
        especeError->setVisible(false);
        poidsError->setVisible(false);
        zonePecheError->setVisible(false);
        dateCaptureError->setVisible(false);
        etatError->setVisible(false);
        navireError->setVisible(false);
        qualiteError->setVisible(false);
        dateEntreeStockError->setVisible(false);

        if (idLotEdit->text().trimmed().isEmpty()) {
            idLotError->setText(tr("ID Lot obligatoire"));
            idLotError->setVisible(true);
            valid = false;
        }
        if (especeEdit->text().trimmed().isEmpty()) {
            especeError->setText(tr("Espèce obligatoire"));
            especeError->setVisible(true);
            valid = false;
        }
        if (poidsEdit->value() <= 0) {
            poidsError->setText(tr("Poids doit être > 0"));
            poidsError->setVisible(true);
            valid = false;
        }
        if (zonePecheEdit->text().trimmed().isEmpty()) {
            zonePecheError->setText(tr("Zone de pêche obligatoire"));
            zonePecheError->setVisible(true);
            valid = false;
        }
        if (!dateCaptureEdit->dateTime().isValid()) {
            dateCaptureError->setText(tr("Date de capture invalide"));
            dateCaptureError->setVisible(true);
            valid = false;
        }
        if (etatEdit->currentText().isEmpty()) {
            etatError->setText(tr("Etat obligatoire"));
            etatError->setVisible(true);
            valid = false;
        }
        if (navireEdit->text().trimmed().isEmpty()) {
            navireError->setText(tr("Navire obligatoire"));
            navireError->setVisible(true);
            valid = false;
        }
        if (qualiteEdit->currentText().isEmpty()) {
            qualiteError->setText(tr("Qualité obligatoire"));
            qualiteError->setVisible(true);
            valid = false;
        }
        if (!dateEntreeStockEdit->dateTime().isValid()) {
            dateEntreeStockError->setText(tr("Date entrée stock invalide"));
            dateEntreeStockError->setVisible(true);
            valid = false;
        }

        if (valid)
            accept();
    };

    connect(actionBtn, &QPushButton::clicked, this, validateAndAccept);
    connect(saveBtn, &QPushButton::clicked, this, validateAndAccept);

    setFixedSize(500, 660);
}

QString LotDialog::idLot() const { return idLotEdit->text(); }
QString LotDialog::espece() const { return especeEdit->text(); }
int LotDialog::poids() const { return poidsEdit->value(); }
QString LotDialog::zonePeche() const { return zonePecheEdit->text(); }
QDateTime LotDialog::dateCapture() const { return dateCaptureEdit->dateTime(); }
QString LotDialog::navire() const { return navireEdit->text(); }
QString LotDialog::qualite() const { return qualiteEdit->currentText(); }
QDateTime LotDialog::dateEntreeStock() const { return dateEntreeStockEdit->dateTime(); }

void LotDialog::setIdLot(const QString &v) { idLotEdit->setText(v); }
void LotDialog::setEspece(const QString &v) { especeEdit->setText(v); }
void LotDialog::setPoids(int v) { poidsEdit->setValue(v); }
void LotDialog::setZonePeche(const QString &v) { zonePecheEdit->setText(v); }
void LotDialog::setDateCapture(const QDateTime &v) { dateCaptureEdit->setDateTime(v); }
void LotDialog::setNavire(const QString &v) { navireEdit->setText(v); }
void LotDialog::setQualite(const QString &v) { qualiteEdit->setCurrentText(v); }
void LotDialog::setDateEntreeStock(const QDateTime &v) { dateEntreeStockEdit->setDateTime(v); }

QString LotDialog::etat() const { return etatEdit->currentText(); }
void LotDialog::setEtat(const QString &v) { etatEdit->setCurrentText(v); }
