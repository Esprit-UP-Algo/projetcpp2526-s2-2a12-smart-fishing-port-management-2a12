#include "dialogmouvement.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QDateTime>
#include <QFrame>

static QFrame* makeSepM(QWidget *p)
{
    auto *s = new QFrame(p);
    s->setObjectName("separator");
    s->setFrameShape(QFrame::HLine);
    s->setFixedHeight(1);
    return s;
}

static const char *MVT_DIALOG_STYLE = R"(
QDialog {
    background: qlineargradient(x1:0, y1:0, x2:0.5, y2:1, stop:0 #eaf2fb, stop:1 #d8e6f3);
}
QLabel {
    color: #0f2744;
    font-size: 14px;
    font-weight: 700;
    font-family: 'Segoe UI', Arial;
    background: transparent;
}
QLabel#lblDialogTitle {
    color: #0a1929;
    font-size: 22px;
    font-weight: 800;
    font-family: 'Segoe UI', Arial;
}
QLabel#lblSubtitle {
    color: #6b8299;
    font-size: 12px;
    font-weight: 400;
}
QLineEdit, QComboBox, QDoubleSpinBox, QDateTimeEdit {
    background: #0f1c2e;
    border: none;
    border-radius: 10px;
    min-height: 44px;
    font-size: 14px;
    padding: 4px 14px;
    color: #e6eef6;
    font-family: 'Segoe UI', Arial;
    selection-background-color: #1e8de0;
}
QLineEdit:focus, QComboBox:focus, QDoubleSpinBox:focus, QDateTimeEdit:focus {
    border: 1.5px solid #2196f3;
}
QLineEdit::placeholder {
    color: #5a7a94;
}
QComboBox::drop-down {
    background: transparent;
    border: none;
    width: 28px;
    subcontrol-position: center right;
    padding-right: 8px;
}
QComboBox::down-arrow {
    image: none;
    border-left: 5px solid transparent;
    border-right: 5px solid transparent;
    border-top: 6px solid #8ba3bb;
    width: 0; height: 0;
}
QComboBox QAbstractItemView {
    background: #0f1c2e;
    border: 1px solid #1a2d42;
    color: #e6eef6;
    selection-background-color: #1e8de0;
}
QDateTimeEdit::up-button, QDateTimeEdit::down-button {
    background: transparent;
    border: none;
    width: 22px;
}
QDateTimeEdit::up-arrow {
    image: none;
    border-left: 5px solid transparent;
    border-right: 5px solid transparent;
    border-bottom: 5px solid #8ba3bb;
    width: 0; height: 0;
}
QDateTimeEdit::down-arrow {
    image: none;
    border-left: 5px solid transparent;
    border-right: 5px solid transparent;
    border-top: 5px solid #8ba3bb;
    width: 0; height: 0;
}
QDoubleSpinBox::up-button, QDoubleSpinBox::down-button {
    background: transparent;
    border: none;
    width: 22px;
}
QDoubleSpinBox::up-arrow {
    image: none;
    border-left: 5px solid transparent;
    border-right: 5px solid transparent;
    border-bottom: 5px solid #8ba3bb;
    width: 0; height: 0;
}
QDoubleSpinBox::down-arrow {
    image: none;
    border-left: 5px solid transparent;
    border-right: 5px solid transparent;
    border-top: 5px solid #8ba3bb;
    width: 0; height: 0;
}
QCalendarWidget {
    background: #0f1c2e;
    color: #e6eef6;
}
QFrame#separator {
    background: #c0d0e0;
    border: none;
}
#btnValider {
    background: #2196f3;
    color: #ffffff;
    border: 0px;
    border-radius: 12px;
    padding: 12px 36px;
    font-weight: 700;
    font-size: 14px;
    font-family: 'Segoe UI', Arial;
    min-width: 140px;
}
#btnValider:hover {
    background: #42a5f5;
}
#btnAnnuler {
    background: #4a90c4;
    color: #ffffff;
    border: 0px;
    border-radius: 12px;
    padding: 12px 36px;
    font-weight: 700;
    font-size: 14px;
    font-family: 'Segoe UI', Arial;
    min-width: 140px;
}
#btnAnnuler:hover {
    background: #5a9fd0;
}
)";

void DialogMouvement::buildUi()
{
    setMinimumSize(560, 520);
    resize(600, 560);
    setStyleSheet(MVT_DIALOG_STYLE);

    auto *main = new QVBoxLayout(this);
    main->setSpacing(0);
    main->setContentsMargins(36, 28, 36, 28);

    lblDialogTitle = new QLabel("Ajouter un mouvement", this);
    lblDialogTitle->setObjectName("lblDialogTitle");
    main->addWidget(lblDialogTitle);
    main->addSpacing(4);

    auto *lblSub = new QLabel("Veuillez remplir les informations du mouvement.", this);
    lblSub->setObjectName("lblSubtitle");
    main->addWidget(lblSub);
    main->addSpacing(18);

    auto addRow = [&](const QString &text, QWidget *w) {
        auto *r = new QHBoxLayout;
        r->setSpacing(18);
        auto *l = new QLabel(text, this);
        l->setFixedWidth(155);
        l->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        r->addWidget(l);
        r->addWidget(w, 1);
        main->addLayout(r);
    };

    auto addSep = [&]() {
        main->addSpacing(10);
        main->addWidget(makeSepM(this));
        main->addSpacing(10);
    };

    comboNavire = new QComboBox(this);
    addRow("Navire", comboNavire);
    addSep();

    dtDepart = new QDateTimeEdit(this);
    dtDepart->setDisplayFormat("yyyy-MM-dd HH:mm");
    dtDepart->setCalendarPopup(true);
    addRow(QStringLiteral("Heure d\u00e9part"), dtDepart);
    addSep();

    dtRetour = new QDateTimeEdit(this);
    dtRetour->setDisplayFormat("yyyy-MM-dd HH:mm");
    dtRetour->setCalendarPopup(true);
    addRow("Heure retour", dtRetour);
    addSep();

    editCargaison = new QLineEdit(this);
    editCargaison->setPlaceholderText("Ex: 350 kg sardines, 120 kg thon");
    addRow("Cargaison retour", editCargaison);
    addSep();

    spinDistance = new QDoubleSpinBox(this);
    spinDistance->setSuffix(" km");
    spinDistance->setDecimals(1);
    spinDistance->setMaximum(99999.0);
    addRow("Distance (km)", spinDistance);

    main->addSpacerItem(new QSpacerItem(20, 14, QSizePolicy::Minimum, QSizePolicy::Expanding));

    auto *btnLay = new QHBoxLayout;
    btnLay->setSpacing(16);
    btnLay->addStretch();

    btnAnnuler = new QPushButton("Annuler", this);
    btnAnnuler->setObjectName("btnAnnuler");
    btnAnnuler->setCursor(Qt::PointingHandCursor);
    btnLay->addWidget(btnAnnuler);

    btnValider = new QPushButton("Enregistrer", this);
    btnValider->setObjectName("btnValider");
    btnValider->setCursor(Qt::PointingHandCursor);
    btnLay->addWidget(btnValider);

    btnLay->addStretch();
    main->addLayout(btnLay);
}

DialogMouvement::DialogMouvement(const QStringList &navireNames, QWidget *parent)
    : QDialog(parent)
{
    buildUi();
    setWindowTitle("Ajouter un Mouvement");

    comboNavire->clear();
    comboNavire->addItems(navireNames);

    QDateTime now = QDateTime::currentDateTime();
    dtDepart->setDateTime(now);
    dtRetour->setDateTime(now.addSecs(3600));

    setupConnections();
}

DialogMouvement::~DialogMouvement()
{
}

void DialogMouvement::setupConnections()
{
    connect(btnValider,  &QPushButton::clicked, this, &QDialog::accept);
    connect(btnAnnuler,  &QPushButton::clicked, this, &QDialog::reject);
}

Mouvement DialogMouvement::getMouvement() const
{
    Mouvement m;
    m.navireNom       = comboNavire->currentText();
    m.heureDepart     = dtDepart->dateTime();
    m.heureRetour     = dtRetour->dateTime();
    m.cargaisonRetour = editCargaison->text().trimmed();
    m.distanceKm      = spinDistance->value();
    return m;
}
