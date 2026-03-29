#include "dialognavire.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpacerItem>
#include <QFrame>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

static QFrame* makeSep(QWidget *p)
{
    auto *s = new QFrame(p);
    s->setObjectName("separator");
    s->setFrameShape(QFrame::HLine);
    s->setFixedHeight(1);
    return s;
}

static const char *DIALOG_STYLE = R"(
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
QLineEdit, QComboBox, QSpinBox {
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
QLineEdit:focus, QComboBox:focus, QSpinBox:focus {
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
QSpinBox::up-button, QSpinBox::down-button {
    background: transparent;
    border: none;
    width: 22px;
}
QSpinBox::up-arrow {
    image: none;
    border-left: 5px solid transparent;
    border-right: 5px solid transparent;
    border-bottom: 5px solid #8ba3bb;
    width: 0; height: 0;
}
QSpinBox::down-arrow {
    image: none;
    border-left: 5px solid transparent;
    border-right: 5px solid transparent;
    border-top: 5px solid #8ba3bb;
    width: 0; height: 0;
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

void DialogNavire::buildUi()
{
    setMinimumSize(540, 580);
    resize(580, 620);
    setStyleSheet(DIALOG_STYLE);

    auto *main = new QVBoxLayout(this);
    main->setSpacing(0);
    main->setContentsMargins(36, 28, 36, 28);

    lblDialogTitle = new QLabel(this);
    lblDialogTitle->setObjectName("lblDialogTitle");
    main->addWidget(lblDialogTitle);
    main->addSpacing(4);

    auto *lblSub = new QLabel("Veuillez remplir les informations du navire.", this);
    lblSub->setObjectName("lblSubtitle");
    main->addWidget(lblSub);
    main->addSpacing(18);

    auto addRow = [&](const QString &text, QWidget *w) {
        auto *r = new QHBoxLayout;
        r->setSpacing(18);
        auto *l = new QLabel(text, this);
        l->setFixedWidth(140);
        l->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        r->addWidget(l);
        r->addWidget(w, 1);
        main->addLayout(r);
    };

    auto addSep = [&]() {
        main->addSpacing(10);
        main->addWidget(makeSep(this));
        main->addSpacing(10);
    };

    editNom = new QLineEdit(this);
    editNom->setPlaceholderText("Nom du navire");
    editNom->setMaxLength(80);
    addRow("Nom", editNom);
    addSep();

    editMatricule = new QLineEdit(this);
    editMatricule->setPlaceholderText("Ex: M-1237");
    editMatricule->setMaxLength(8);
    {
        // Lettre - 3 à 6 chiffres : M-1234
        auto *v = new QRegularExpressionValidator(QRegularExpression(QStringLiteral("^[A-Za-z]\\-\\d{3,6}$")), editMatricule);
        editMatricule->setValidator(v);
    }
    addRow("Matricule", editMatricule);
    addSep();

    comboType = new QComboBox(this);
    comboType->addItems({QStringLiteral("P\u00eache"), "Transport", "Cargo", "Recherche", "Plaisance"});
    addRow("Type", comboType);
    addSep();

    spinCapacite = new QSpinBox(this);
    spinCapacite->setSuffix(" tonnes");
    spinCapacite->setMinimum(1);
    spinCapacite->setMaximum(100000);
    spinCapacite->setValue(100);
    addRow(QStringLiteral("Capacit\u00e9 (t)"), spinCapacite);
    addSep();

    comboStatut = new QComboBox(this);
    comboStatut->addItems({"Actif", "En maintenance", "Inactif"});
    addRow("Statut", comboStatut);
    addSep();

    editPosition = new QLineEdit(this);
    editPosition->setPlaceholderText("Ex: Port A");
    editPosition->setMaxLength(60);
    addRow("Position", editPosition);

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

// ─── Mode création ──────────────────────────────────────────────────────────
DialogNavire::DialogNavire(QWidget *parent)
    : QDialog(parent)
    , editMode(false)
{
    buildUi();
    setWindowTitle(QStringLiteral("Cr\u00e9er un navire"));
    lblDialogTitle->setText(QStringLiteral("Cr\u00e9er un navire"));
    btnValider->setText("Enregistrer");
    setupConnections();
}

// ─── Mode modification ──────────────────────────────────────────────────────
DialogNavire::DialogNavire(const Navire &navire, QWidget *parent)
    : QDialog(parent)
    , editMode(true)
{
    buildUi();
    setWindowTitle("Modifier le navire");
    lblDialogTitle->setText("Modifier le navire");
    btnValider->setText("Enregistrer");

    editNom->setText(navire.nom);
    editMatricule->setText(navire.matricule);
    comboType->setCurrentText(navire.type);
    spinCapacite->setValue(navire.capacite);
    comboStatut->setCurrentText(navire.statut);
    editPosition->setText(navire.position);

    setupConnections();
}

DialogNavire::~DialogNavire()
{
}

void DialogNavire::setupConnections()
{
    connect(btnValider,  &QPushButton::clicked, this, [this]() {
        QString err;
        if (!validateInputs(&err)) {
            QMessageBox::warning(this, QStringLiteral("Contrôle de saisie"), err);
            return;
        }
        accept();
    });
    connect(btnAnnuler,  &QPushButton::clicked, this, &QDialog::reject);
}

bool DialogNavire::validateInputs(QString *errorMessage) const
{
    const QString nom = editNom->text().trimmed();
    const QString mat = editMatricule->text().trimmed();
    const QString pos = editPosition->text().trimmed();

    if (nom.isEmpty()) {
        if (errorMessage) *errorMessage = QStringLiteral("Le nom est obligatoire.");
        return false;
    }
    if (mat.isEmpty()) {
        if (errorMessage) *errorMessage = QStringLiteral("Le matricule est obligatoire.");
        return false;
    }
    if (editMatricule->hasAcceptableInput() == false) {
        if (errorMessage) *errorMessage = QStringLiteral("Matricule invalide. Format attendu: M-1234.");
        return false;
    }
    if (pos.isEmpty()) {
        if (errorMessage) *errorMessage = QStringLiteral("La position est obligatoire.");
        return false;
    }
    if (spinCapacite->value() <= 0) {
        if (errorMessage) *errorMessage = QStringLiteral("La capacité doit être > 0.");
        return false;
    }
    return true;
}

Navire DialogNavire::getNavire() const
{
    Navire n;
    n.nom       = editNom->text().trimmed();
    n.matricule = editMatricule->text().trimmed();
    n.type      = comboType->currentText();
    n.capacite  = spinCapacite->value();
    n.statut    = comboStatut->currentText();
    n.position  = editPosition->text().trimmed();
    return n;
}
