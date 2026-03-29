#include "dialognavire.h"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QList>
#include <QLineEdit>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSequentialAnimationGroup>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QWidget>

// Match the CRUD dialog styling used in the Quais module.
static const char *NAVIRE_DIALOG_STYLE = R"(
QDialog {
  background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #ffffff, stop:1 #ADD8E6);
}
QLabel#HeaderH1 {
  color: #132437;
  font-size: 28px;
  font-weight: bold;
  font-family: 'Segoe UI', Arial;
  letter-spacing: 0px;
  margin: 0px;
  padding: 0px;
}
QDialog QLabel#Muted { color: #5c6e80; font-size: 13px; }
QDialog QLabel#FormLabel { color: #132437; font-weight: 700; font-size: 13px; }
QDialog QLabel#ErrorText { color: #e74c3c; font-size: 12px; }

QDialog QLineEdit#Input,
QDialog QSpinBox#Input,
QDialog QComboBox#Input {
  background: #132437;
  border: 1px solid #23374e;
  border-radius: 8px;
  min-height: 36px;
  padding: 4px 10px;
  font-size: 13px;
  color: #e6eef6;
  selection-background-color: #1e8de0;
}
QDialog QLineEdit#Input:focus,
QDialog QSpinBox#Input:focus,
QDialog QComboBox#Input:focus {
  border-color: #1e8de0;
}
QComboBox::drop-down { border: 0px; width: 20px; }

QPushButton#AltPrimaryButton {
  background: transparent;
  color: #1e8de0;
  border: 2px solid #1e8de0;
  border-radius: 10px;
  padding: 9px 14px;
  font-weight: 700;
  font-size: 14px;
}
QPushButton#AltPrimaryButton:hover { background: rgba(30,141,224,30); }
QPushButton#AltPrimaryButton:pressed { background: rgba(30,141,224,60); }

QPushButton#PrimaryButton {
  background: #1e8de0;
  color: #ffffff;
  border: 0px;
  border-radius: 10px;
  padding: 9px 14px;
  font-weight: 700;
  font-size: 14px;
}
QPushButton#PrimaryButton:hover { background: #2a99ec; }
QPushButton#PrimaryButton:pressed { background: #1a7cc5; }
)";

static QLabel *makeDialogTitle(const QString &text, QWidget *parent)
{
    auto *label = new QLabel(text, parent);
    label->setObjectName("HeaderH1");
    QFont font = label->font();
    font.setPixelSize(18);
    font.setBold(true);
    label->setFont(font);
    return label;
}

static QLabel *makeInlineErrorLabel(QWidget *parent)
{
    auto *label = new QLabel(parent);
    label->setObjectName("ErrorText");
    label->setWordWrap(true);
    label->setVisible(false);
    label->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    return label;
}

static void shakeWidget(QWidget *widget)
{
    if (!widget)
        return;

    const QPoint startPos = widget->pos();
    const int amplitude = 6;
    const int stepMs = 28;

    auto *group = new QSequentialAnimationGroup(widget);
    const auto addStep = [group, widget, stepMs](const QPoint &pos) {
        auto *anim = new QPropertyAnimation(widget, "pos", group);
        anim->setDuration(stepMs);
        anim->setEasingCurve(QEasingCurve::InOutSine);
        anim->setEndValue(pos);
        group->addAnimation(anim);
    };

    addStep(startPos + QPoint(-amplitude, 0));
    addStep(startPos + QPoint(amplitude, 0));
    addStep(startPos + QPoint(-amplitude, 0));
    addStep(startPos + QPoint(amplitude, 0));
    addStep(startPos);

    group->start(QAbstractAnimation::DeleteWhenStopped);
}

void DialogNavire::buildUi()
{
    setModal(true);
    setMinimumSize(520, 520);
    resize(520, 520);
    setStyleSheet(NAVIRE_DIALOG_STYLE);

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(18, 16, 18, 16);
    root->setSpacing(12);

    lblDialogTitle = makeDialogTitle(windowTitle(), this);
    root->addWidget(lblDialogTitle);

    lblHint = new QLabel(tr("Veuillez remplir les informations du navire."), this);
    lblHint->setObjectName("Muted");
    root->addWidget(lblHint);

    auto *form = new QFormLayout();
    form->setHorizontalSpacing(14);
    form->setVerticalSpacing(8);
    form->setRowWrapPolicy(QFormLayout::DontWrapRows);
    form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    form->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    editNom = new QLineEdit(this);
    editNom->setObjectName("Input");
    editNom->setPlaceholderText(tr("Nom du navire"));
    editNom->setMaxLength(80);

    editMatricule = new QLineEdit(this);
    editMatricule->setObjectName("Input");
    editMatricule->setPlaceholderText(tr("Ex: M-1237"));
    editMatricule->setMaxLength(16);
    {
        const QRegularExpression re(QStringLiteral("^[A-Za-z]\\-\\d{3,6}$"));
        editMatricule->setValidator(new QRegularExpressionValidator(re, editMatricule));
    }

    comboType = new QComboBox(this);
    comboType->setObjectName("Input");
    comboType->addItems({QStringLiteral("P\u00eache"), tr("Transport"), tr("Cargo"), tr("Recherche"), tr("Plaisance")});

    spinCapacite = new QSpinBox(this);
    spinCapacite->setObjectName("Input");
    spinCapacite->setSuffix(tr(" tonnes"));
    spinCapacite->setMinimum(1);
    spinCapacite->setMaximum(100000);
    spinCapacite->setValue(100);

    comboStatut = new QComboBox(this);
    comboStatut->setObjectName("Input");
    comboStatut->addItems({tr("Actif"), tr("En maintenance"), tr("Inactif")});

    editPosition = new QLineEdit(this);
    editPosition->setObjectName("Input");
    editPosition->setPlaceholderText(tr("Ex: Port A"));
    editPosition->setMaxLength(60);

    const auto wrapFieldWithError = [this](QWidget *field, QLabel **outError) -> QWidget * {
        auto *box = new QWidget(this);
        auto *vl = new QVBoxLayout(box);
        vl->setContentsMargins(0, 0, 0, 0);
        vl->setSpacing(4);
        vl->addWidget(field);
        QLabel *err = makeInlineErrorLabel(box);
        vl->addWidget(err);
        if (outError)
            *outError = err;
        return box;
    };

    const auto wrapCenteredLabel = [this](QLabel *label) -> QWidget * {
        auto *box = new QWidget(this);
        auto *vl = new QVBoxLayout(box);
        vl->setContentsMargins(0, 0, 0, 0);
        vl->setSpacing(0);
        vl->addStretch(1);
        vl->addWidget(label);
        vl->addStretch(1);
        return box;
    };

    auto *labNom = new QLabel(tr("Nom"), this);
    labNom->setObjectName("FormLabel");
    auto *labMat = new QLabel(tr("Matricule"), this);
    labMat->setObjectName("FormLabel");
    auto *labType = new QLabel(tr("Type"), this);
    labType->setObjectName("FormLabel");
    auto *labCap = new QLabel(tr("Capacité (t)"), this);
    labCap->setObjectName("FormLabel");
    auto *labStatut = new QLabel(tr("Statut"), this);
    labStatut->setObjectName("FormLabel");
    auto *labPos = new QLabel(tr("Position"), this);
    labPos->setObjectName("FormLabel");

    const QList<QLabel *> labels = {labNom, labMat, labType, labCap, labStatut, labPos};
    for (QLabel *label : labels)
    {
        QFont font = label->font();
        font.setBold(true);
        label->setFont(font);
        label->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }

    form->addRow(wrapCenteredLabel(labNom), wrapFieldWithError(editNom, &errNom));
    form->addRow(wrapCenteredLabel(labMat), wrapFieldWithError(editMatricule, &errMatricule));
    form->addRow(wrapCenteredLabel(labType), wrapFieldWithError(comboType, nullptr));
    form->addRow(wrapCenteredLabel(labCap), wrapFieldWithError(spinCapacite, &errCapacite));
    form->addRow(wrapCenteredLabel(labStatut), wrapFieldWithError(comboStatut, nullptr));
    form->addRow(wrapCenteredLabel(labPos), wrapFieldWithError(editPosition, &errPosition));

    root->addLayout(form);

    auto *btnRow = new QHBoxLayout();
    btnRow->addStretch();

    btnAnnuler = new QPushButton(tr("Annuler"), this);
    btnAnnuler->setObjectName("AltPrimaryButton");
    btnAnnuler->setCursor(Qt::PointingHandCursor);
    btnAnnuler->setMinimumSize(130, 40);

    btnValider = new QPushButton(tr("Enregistrer"), this);
    btnValider->setObjectName("PrimaryButton");
    btnValider->setCursor(Qt::PointingHandCursor);
    btnValider->setMinimumSize(130, 40);

    btnRow->addWidget(btnAnnuler);
    btnRow->addWidget(btnValider);

    root->addLayout(btnRow);
}

DialogNavire::DialogNavire(QWidget *parent)
    : QDialog(parent)
    , editMode(false)
{
    setWindowTitle(QStringLiteral("Cr\u00e9er un navire"));
    buildUi();
    lblDialogTitle->setText(windowTitle());
    setupConnections();
    updateInlineErrors();
    refreshOkEnabled();
}

DialogNavire::DialogNavire(const Navire &navire, QWidget *parent)
    : QDialog(parent)
    , editMode(true)
{
    setWindowTitle(tr("Modifier le navire"));
    buildUi();
    lblDialogTitle->setText(windowTitle());

    editNom->setText(navire.nom);
    editMatricule->setText(navire.matricule);
    comboType->setCurrentText(navire.type);
    spinCapacite->setValue(navire.capacite);
    comboStatut->setCurrentText(navire.statut);
    editPosition->setText(navire.position);

    setupConnections();
    updateInlineErrors();
    refreshOkEnabled();
}

DialogNavire::~DialogNavire() = default;

void DialogNavire::setupConnections()
{
    connect(btnAnnuler, &QPushButton::clicked, this, &QDialog::reject);

    const auto hook = [this]() {
        updateInlineErrors();
        refreshOkEnabled();
    };
    connect(editNom, &QLineEdit::textChanged, this, hook);
    connect(editMatricule, &QLineEdit::textChanged, this, hook);
    connect(editPosition, &QLineEdit::textChanged, this, hook);
    connect(spinCapacite, qOverload<int>(&QSpinBox::valueChanged), this, hook);

    connect(btnValider, &QPushButton::clicked, this, [this]() {
        updateInlineErrors();
        refreshOkEnabled();
        if (!validateInputs(nullptr))
        {
            shakeWidget(this);
            return;
        }
        accept();
    });
}

bool DialogNavire::validateInputs(QString *errorMessage) const
{
    const QString nom = editNom ? editNom->text().trimmed() : QString();
    const QString mat = editMatricule ? editMatricule->text().trimmed() : QString();
    const QString pos = editPosition ? editPosition->text().trimmed() : QString();

    if (nom.isEmpty())
    {
        if (errorMessage)
            *errorMessage = tr("Le nom est obligatoire.");
        return false;
    }
    if (mat.isEmpty())
    {
        if (errorMessage)
            *errorMessage = tr("Le matricule est obligatoire.");
        return false;
    }
    if (editMatricule && !editMatricule->hasAcceptableInput())
    {
        if (errorMessage)
            *errorMessage = tr("Matricule invalide. Format attendu: M-1234.");
        return false;
    }
    if (pos.isEmpty())
    {
        if (errorMessage)
            *errorMessage = tr("La position est obligatoire.");
        return false;
    }
    if (spinCapacite && spinCapacite->value() <= 0)
    {
        if (errorMessage)
            *errorMessage = tr("La capacité doit être > 0.");
        return false;
    }
    return true;
}

void DialogNavire::updateInlineErrors()
{
    const auto setErr = [](QLabel *label, const QString &msg) {
        if (!label)
            return;
        const QString m = msg.trimmed();
        label->setText(m);
        label->setVisible(!m.isEmpty());
    };

    setErr(errNom, QString());
    setErr(errMatricule, QString());
    setErr(errCapacite, QString());
    setErr(errPosition, QString());

    const QString nom = editNom ? editNom->text().trimmed() : QString();
    if (nom.isEmpty())
        setErr(errNom, tr("Champ obligatoire."));

    const QString mat = editMatricule ? editMatricule->text().trimmed() : QString();
    if (mat.isEmpty())
    {
        setErr(errMatricule, tr("Champ obligatoire."));
    }
    else if (editMatricule && !editMatricule->hasAcceptableInput())
    {
        setErr(errMatricule, tr("Format attendu: M-1234."));
    }

    if (spinCapacite && spinCapacite->value() <= 0)
        setErr(errCapacite, tr("La capacité doit être > 0."));

    const QString pos = editPosition ? editPosition->text().trimmed() : QString();
    if (pos.isEmpty())
        setErr(errPosition, tr("Champ obligatoire."));
}

void DialogNavire::refreshOkEnabled()
{
    if (!btnValider)
        return;

    QString err;
    const bool ok = validateInputs(&err);
    // Keep the button clickable to allow a "shake" feedback on invalid submit.
    btnValider->setEnabled(true);
    btnValider->setToolTip(ok ? QString() : err);
}

Navire DialogNavire::getNavire() const
{
    Navire n;
    n.nom = editNom->text().trimmed();
    n.matricule = editMatricule->text().trimmed();
    n.type = comboType->currentText();
    n.capacite = spinCapacite->value();
    n.statut = comboStatut->currentText();
    n.position = editPosition->text().trimmed();
    return n;
}
