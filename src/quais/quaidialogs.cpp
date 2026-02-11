#include "quaidialogs.h"

#include <QComboBox>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

#include "quai.h"

static QLabel *makeDialogTitle(const QString &text, QWidget *parent)
{
    auto *l = new QLabel(text, parent);
    l->setObjectName("HeaderH1");
    QFont f = l->font();
    f.setPixelSize(18);
    f.setBold(true);
    l->setFont(f);
    return l;
}

QuaiFormDialog::QuaiFormDialog(Mode mode, QWidget *parent)
    : QDialog(parent)
    , m_mode(mode)
{
    setModal(true);
    setWindowTitle(mode == Mode::Create ? tr("Créer un quai") : tr("Modifier un quai"));
    resize(520, 520);

    buildUi();
    applyMode();
}

void QuaiFormDialog::buildUi()
{
    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(18, 16, 18, 16);
    root->setSpacing(12);

    root->addWidget(makeDialogTitle(windowTitle(), this));

    auto *hint = new QLabel(tr("Veuillez remplir les informations du quai."), this);
    hint->setObjectName("Muted");
    root->addWidget(hint);

    auto *form = new QFormLayout();
    form->setHorizontalSpacing(14);
    form->setVerticalSpacing(8);
    form->setRowWrapPolicy(QFormLayout::DontWrapRows);
    form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
    form->setLabelAlignment(Qt::AlignLeft | Qt::AlignVCenter);

    m_matricule = new QLineEdit(this);
    m_matricule->setObjectName("Input");
    m_matricule->setPlaceholderText("MQ-");

    m_taille = new QDoubleSpinBox(this);
    m_taille->setObjectName("Input");
    m_taille->setRange(0.0, 1e6);
    m_taille->setDecimals(1);
    m_taille->setSingleStep(0.5);

    m_posX = new QDoubleSpinBox(this);
    m_posX->setObjectName("Input");
    m_posX->setRange(-1e9, 1e9);
    m_posX->setDecimals(2);
    m_posX->setSingleStep(1.0);

    m_posY = new QDoubleSpinBox(this);
    m_posY->setObjectName("Input");
    m_posY->setRange(-1e9, 1e9);
    m_posY->setDecimals(2);
    m_posY->setSingleStep(1.0);

    m_arrivee = new QDateTimeEdit(QDateTime::currentDateTime(), this);
    m_arrivee->setObjectName("Input");
    m_arrivee->setDisplayFormat("yyyy-MM-dd HH:mm");
    m_arrivee->setCalendarPopup(true);

    m_depart = new QDateTimeEdit(QDateTime::currentDateTime().addSecs(2 * 3600), this);
    m_depart->setObjectName("Input");
    m_depart->setDisplayFormat("yyyy-MM-dd HH:mm");
    m_depart->setCalendarPopup(true);

    m_etat = new QComboBox(this);
    m_etat->setObjectName("Input");
    m_etat->addItems({tr("Libre"), tr("Occupé"), tr("En maintenance")});

    m_prix = new QDoubleSpinBox(this);
    m_prix->setObjectName("Input");
    m_prix->setRange(0.0, 1e9);
    m_prix->setDecimals(2);
    m_prix->setSingleStep(5.0);

    auto *labMat = new QLabel(tr("Matricule quai"), this);
    labMat->setObjectName("FormLabel");
    auto *labTaille = new QLabel(tr("Taille (m)"), this);
    labTaille->setObjectName("FormLabel");
    auto *labArr = new QLabel(tr("Date/Heure arrivée"), this);
    labArr->setObjectName("FormLabel");
    auto *labDep = new QLabel(tr("Date/Heure départ"), this);
    labDep->setObjectName("FormLabel");
    auto *labPosX = new QLabel(tr("Position X"), this);
    labPosX->setObjectName("FormLabel");
    auto *labPosY = new QLabel(tr("Position Y"), this);
    labPosY->setObjectName("FormLabel");
    auto *labEtat = new QLabel(tr("État quai"), this);
    labEtat->setObjectName("FormLabel");
    auto *labPrix = new QLabel(tr("Prix location (DT/h)"), this);
    labPrix->setObjectName("FormLabel");

    const QList<QLabel *> labels = {labMat, labTaille, labPosX, labPosY, labArr, labDep, labEtat, labPrix};
    for (QLabel *l : labels)
    {
        QFont f = l->font();
        f.setBold(true);
        l->setFont(f);
        l->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    }

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

    form->addRow(wrapCenteredLabel(labMat), m_matricule);
    form->addRow(wrapCenteredLabel(labTaille), m_taille);
    form->addRow(wrapCenteredLabel(labPosX), m_posX);
    form->addRow(wrapCenteredLabel(labPosY), m_posY);
    form->addRow(wrapCenteredLabel(labArr), m_arrivee);
    form->addRow(wrapCenteredLabel(labDep), m_depart);
    form->addRow(wrapCenteredLabel(labEtat), m_etat);
    form->addRow(wrapCenteredLabel(labPrix), m_prix);

    root->addLayout(form);

    auto *btnRow = new QHBoxLayout();
    btnRow->addStretch();

    m_btnCancel = new QPushButton(tr("Annuler"), this);
    m_btnCancel->setObjectName("AltPrimaryButton");
    m_btnCancel->setCursor(Qt::PointingHandCursor);
    m_btnCancel->setMinimumSize(130, 40);

    m_btnOk = new QPushButton(tr("Enregistrer"), this);
    m_btnOk->setObjectName("PrimaryButton");
    m_btnOk->setCursor(Qt::PointingHandCursor);
    m_btnOk->setMinimumSize(130, 40);

    btnRow->addWidget(m_btnCancel);
    btnRow->addWidget(m_btnOk);

    root->addLayout(btnRow);

    connect(m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_btnOk, &QPushButton::clicked, this, &QDialog::accept);
}

void QuaiFormDialog::applyMode()
{
    // IDQuai is auto-incremented and not editable in the form.
}

void QuaiFormDialog::setQuai(const Quai &q)
{
    m_currentIdQuai = q.idQuai;
    m_matricule->setText(q.matricule);
    m_taille->setValue(q.taille);
    m_posX->setValue(q.posX);
    m_posY->setValue(q.posY);
    m_arrivee->setDateTime(q.dateHeureArrivee);
    m_depart->setDateTime(q.dateHeureDepart);

    const int idx = m_etat->findText(q.etatQuai);
    m_etat->setCurrentIndex(idx >= 0 ? idx : 0);

    m_prix->setValue(q.prixLocation);
}

Quai QuaiFormDialog::quai() const
{
    Quai q;
    // If we're editing an existing quai, keep its ID. For create, leave empty (auto-generated by the model).
    q.idQuai = (m_mode == Mode::Update) ? m_currentIdQuai : QString();
    q.matricule = m_matricule->text();
    q.taille = m_taille->value();
    q.posX = m_posX->value();
    q.posY = m_posY->value();
    q.dateHeureArrivee = m_arrivee->dateTime();
    q.dateHeureDepart = m_depart->dateTime();
    q.etatQuai = m_etat->currentText();
    q.prixLocation = m_prix->value();
    return q;
}

QuaiDeleteDialog::QuaiDeleteDialog(QWidget *parent)
    : QDialog(parent)
{
    setModal(true);
    setWindowTitle(tr("Supprimer un quai"));
    resize(460, 200);

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(18, 16, 18, 16);
    root->setSpacing(12);

    m_title = makeDialogTitle(tr("Confirmation de suppression"), this);
    root->addWidget(m_title);

    m_sub = new QLabel(this);
    m_sub->setObjectName("Muted");
    m_sub->setWordWrap(true);
    root->addWidget(m_sub);

    root->addStretch();

    auto *btnRow = new QHBoxLayout();
    btnRow->addStretch();

    m_btnCancel = new QPushButton(tr("Annuler"), this);
    m_btnCancel->setObjectName("AltPrimaryButton");
    m_btnCancel->setCursor(Qt::PointingHandCursor);
    m_btnCancel->setMinimumSize(130, 40);

    m_btnDelete = new QPushButton(tr("Supprimer"), this);
    m_btnDelete->setObjectName("DangerButton");
    m_btnDelete->setCursor(Qt::PointingHandCursor);
    m_btnDelete->setMinimumSize(130, 40);

    btnRow->addWidget(m_btnCancel);
    btnRow->addWidget(m_btnDelete);
    root->addLayout(btnRow);

    connect(m_btnCancel, &QPushButton::clicked, this, &QDialog::reject);
    connect(m_btnDelete, &QPushButton::clicked, this, &QDialog::accept);
}

void QuaiDeleteDialog::setQuaiSummary(const QString &idQuai, const QString &matricule)
{
    m_sub->setText(tr("Vous êtes sur le point de supprimer le quai <b>%1</b> (matricule: %2).<br>Cette action est irréversible.")
                     .arg(idQuai.toHtmlEscaped(), matricule.toHtmlEscaped()));
}
