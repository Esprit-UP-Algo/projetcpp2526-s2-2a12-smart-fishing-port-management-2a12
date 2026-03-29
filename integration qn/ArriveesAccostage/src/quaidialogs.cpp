#include "quaidialogs.h"

#include <QComboBox>
#include <QDateTime>
#include <QDateTimeEdit>
#include <QCalendarWidget>
#include <QDate>
#include <QTime>
#include <QLocale>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QShowEvent>
#include <QPushButton>
#include <QPropertyAnimation>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QSequentialAnimationGroup>
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

static QLabel *makeInlineErrorLabel(QWidget *parent)
{
    auto *l = new QLabel(parent);
    l->setObjectName("ErrorText");
    l->setWordWrap(true);
    l->setVisible(false);
    l->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Minimum);
    return l;
}

static void shakeWidget(QWidget *w)
{
    if (!w)
        return;

    // Quick horizontal shake.
    const QPoint startPos = w->pos();
    const int amplitude = 6;
    const int stepMs = 28;

    auto *group = new QSequentialAnimationGroup(w);
    const auto addStep = [group, w, stepMs](const QPoint &pos) {
        auto *anim = new QPropertyAnimation(w, "pos", group);
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

static QString foldAccentsLowerTrim(const QString &s)
{
    QString out = s.trimmed().toLower().normalized(QString::NormalizationForm_D);
    // Remove diacritic marks (e.g., "é" -> "e") to make matching more robust.
    out.remove(QRegularExpression(QStringLiteral("[\\u0300-\\u036f]")));
    return out;
}

static bool isOccupeText(const QString &etat)
{
    const QString t = foldAccentsLowerTrim(etat);
    return t == QStringLiteral("occupe") || t.contains(QStringLiteral("occup"));
}

static bool isLibreText(const QString &etat)
{
    const QString t = foldAccentsLowerTrim(etat);
    return t == QStringLiteral("libre");
}

static bool isMaintenanceText(const QString &etat)
{
    const QString t = foldAccentsLowerTrim(etat);
    return t.contains(QStringLiteral("maintenance"));
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
    m_matricule->setMaxLength(24);

    // Basic input control: enforce a simple, consistent matricule format.
    // Example requested: "MQ-" + number (case-insensitive). You can widen the regex later if needed.
    {
        const QRegularExpression re(QStringLiteral(R"(^\s*MQ-\d+\s*$)"), QRegularExpression::CaseInsensitiveOption);
        m_matricule->setValidator(new QRegularExpressionValidator(re, m_matricule));
    }

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

    {
        auto *cal = new QCalendarWidget(m_arrivee);
        cal->setGridVisible(true);
        cal->setHorizontalHeaderFormat(QCalendarWidget::ShortDayNames);
        cal->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
        cal->setNavigationBarVisible(true);
        cal->setLocale(QLocale::system());
        const QSize minSize(380, 300);
        cal->setMinimumSize(minSize);
        cal->resize(minSize);
        m_arrivee->setCalendarWidget(cal);
    }

    m_depart = new QDateTimeEdit(QDateTime::currentDateTime().addSecs(2 * 3600), this);
    m_depart->setObjectName("Input");
    m_depart->setDisplayFormat("yyyy-MM-dd HH:mm");
    m_depart->setCalendarPopup(true);

    {
        auto *cal = new QCalendarWidget(m_depart);
        cal->setGridVisible(true);
        cal->setHorizontalHeaderFormat(QCalendarWidget::ShortDayNames);
        cal->setVerticalHeaderFormat(QCalendarWidget::NoVerticalHeader);
        cal->setNavigationBarVisible(true);
        cal->setLocale(QLocale::system());
        const QSize minSize(380, 300);
        cal->setMinimumSize(minSize);
        cal->resize(minSize);
        m_depart->setCalendarWidget(cal);
    }

    m_etat = new QComboBox(this);
    m_etat->setObjectName("Input");
    m_etat->addItems({tr("Libre"), tr("Occupé"), tr("En maintenance")});

    m_prix = new QDoubleSpinBox(this);
    m_prix->setObjectName("Input");
    m_prix->setRange(0.0, 1e9);
    m_prix->setDecimals(2);
    m_prix->setSingleStep(5.0);

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

    const QList<QLabel *> labels = {labEtat, labTaille, labPrix, labMat, labArr, labDep, labPosX, labPosY};
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

    // Order aligned with Oracle table QUAI (excluding auto-increment IDQUAI).
    form->addRow(wrapCenteredLabel(labEtat), wrapFieldWithError(m_etat, &m_etatError));
    form->addRow(wrapCenteredLabel(labTaille), wrapFieldWithError(m_taille, &m_tailleError));
    form->addRow(wrapCenteredLabel(labPrix), wrapFieldWithError(m_prix, &m_prixError));
    form->addRow(wrapCenteredLabel(labMat), wrapFieldWithError(m_matricule, &m_matriculeError));
    form->addRow(wrapCenteredLabel(labArr), wrapFieldWithError(m_arrivee, &m_arriveeError));
    form->addRow(wrapCenteredLabel(labDep), wrapFieldWithError(m_depart, &m_departError));
    form->addRow(wrapCenteredLabel(labPosX), wrapFieldWithError(m_posX, &m_posXError));
    form->addRow(wrapCenteredLabel(labPosY), wrapFieldWithError(m_posY, &m_posYError));

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
    // Important: connect to our override so invalid submissions trigger inline errors + shake.
    connect(m_btnOk, &QPushButton::clicked, this, &QuaiFormDialog::accept);

    // Live validation: keep the OK button disabled until the form is valid.
    // This complements (does not replace) the model/database validation.
    const auto hook = [this]() {
        refreshDateTimeConstraints();
        refreshOkEnabled();
    };
    connect(m_matricule, &QLineEdit::textChanged, this, hook);
    connect(m_taille, qOverload<double>(&QDoubleSpinBox::valueChanged), this, hook);
    connect(m_posX, qOverload<double>(&QDoubleSpinBox::valueChanged), this, hook);
    connect(m_posY, qOverload<double>(&QDoubleSpinBox::valueChanged), this, hook);
    connect(m_arrivee, &QDateTimeEdit::dateTimeChanged, this, hook);
    connect(m_depart, &QDateTimeEdit::dateTimeChanged, this, hook);
    connect(m_etat, &QComboBox::currentTextChanged, this, [this, hook](const QString &) {
        refreshDateTimeEnabled();
        hook();
    });
    connect(m_prix, qOverload<double>(&QDoubleSpinBox::valueChanged), this, hook);

    refreshDateTimeEnabled();
    refreshOkEnabled();
}

void QuaiFormDialog::updateInlineErrors()
{
    const auto setErr = [](QLabel *l, const QString &msg) {
        if (!l)
            return;
        const QString m = msg.trimmed();
        l->setText(m);
        l->setVisible(!m.isEmpty());
    };

    // Default: no errors.
    setErr(m_matriculeError, QString());
    setErr(m_tailleError, QString());
    setErr(m_prixError, QString());
    setErr(m_posXError, QString());
    setErr(m_posYError, QString());
    setErr(m_etatError, QString());
    setErr(m_arriveeError, QString());
    setErr(m_departError, QString());

    const QString mat = m_matricule ? m_matricule->text().trimmed() : QString();
    if (mat.isEmpty())
    {
        setErr(m_matriculeError, tr("Champ obligatoire."));
    }
    else if (mat.contains(' '))
    {
        setErr(m_matriculeError, tr("Ne doit pas contenir d'espaces."));
    }
    else
    {
        const QRegularExpression re(QStringLiteral(R"(^MQ-\d+$)"), QRegularExpression::CaseInsensitiveOption);
        if (!re.match(mat).hasMatch())
            setErr(m_matriculeError, tr("Format attendu: MQ- suivi d'un numéro (ex: MQ-12)."));
    }

    const QString etatText = m_etat ? m_etat->currentText().trimmed() : QString();
    if (etatText.isEmpty())
        setErr(m_etatError, tr("Champ obligatoire."));

    const bool requireInterval = isOccupeText(etatText);
    if (requireInterval)
    {
        const QDateTime a = m_arrivee ? m_arrivee->dateTime() : QDateTime();
        const QDateTime d = m_depart ? m_depart->dateTime() : QDateTime();

        if (!a.isValid())
            setErr(m_arriveeError, tr("Champ obligatoire."));
        if (!d.isValid())
            setErr(m_departError, tr("Champ obligatoire."));

        if (a.isValid() && d.isValid())
        {
            if (d <= a)
            {
                // Prioritize the core coherence rule.
                setErr(m_departError, tr("La date/heure départ doit être après l'arrivée."));
            }
            else if (m_mode == Mode::Update && m_originalArrivee.isValid() && m_originalDepart.isValid())
            {
                if (a < m_originalArrivee)
                    setErr(m_arriveeError, tr("Ne peut pas être avant la période initiale."));
                if (d < m_originalDepart)
                    setErr(m_departError, tr("Ne peut pas être avant la période initiale."));
            }
        }
    }
    else
    {
        // When not Occupé, dates are disabled/ignored: keep error labels hidden.
        setErr(m_arriveeError, QString());
        setErr(m_departError, QString());
    }
}

void QuaiFormDialog::applyMode()
{
    // IDQuai is auto-incremented and not editable in the form.

    if (m_matricule)
    {
        const bool canEditMatricule = (m_mode == Mode::Create);
        m_matricule->setReadOnly(!canEditMatricule);
        m_matricule->setToolTip(canEditMatricule
            ? QString()
            : tr("Le matricule ne peut pas être modifié.")
        );
    }
}

void QuaiFormDialog::showEvent(QShowEvent *event)
{
    QDialog::showEvent(event);

    if (m_mode != Mode::Create)
        return;
    if (!m_arrivee || !m_depart)
        return;

    const QDateTime now = QDateTime::currentDateTime();
    {
        QSignalBlocker ba(m_arrivee);
        QSignalBlocker bd(m_depart);
        m_arrivee->setDateTime(now);
        m_depart->setDateTime(now.addSecs(2 * 3600));
    }
    refreshOkEnabled();
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

    m_originalArrivee = q.dateHeureArrivee;
    m_originalDepart = q.dateHeureDepart;

    const int idx = m_etat->findText(q.etatQuai);
    m_etat->setCurrentIndex(idx >= 0 ? idx : 0);

    m_prix->setValue(q.prixLocation);

    refreshDateTimeEnabled();
    refreshDateTimeConstraints();
    refreshOkEnabled();
}

void QuaiFormDialog::refreshDateTimeEnabled()
{
    if (!m_etat || !m_arrivee || !m_depart)
        return;

    const QString etat = m_etat->currentText().trimmed();
    const bool enableDates = isOccupeText(etat);

    m_arrivee->setEnabled(enableDates);
    m_depart->setEnabled(enableDates);

    const QString hint = enableDates
        ? QString()
        : tr("Dates/heures non modifiables quand l'état est Libre ou En maintenance.");
    m_arrivee->setToolTip(hint);
    m_depart->setToolTip(hint);

    refreshDateTimeConstraints();
}

void QuaiFormDialog::refreshDateTimeConstraints()
{
    if (!m_etat || !m_arrivee || !m_depart)
        return;

    const QDateTime globalMin(QDate(1900, 1, 1), QTime(0, 0));
    const QString etat = m_etat->currentText().trimmed();
    const bool enableDates = isOccupeText(etat);

    if (!enableDates)
    {
        m_arrivee->setMinimumDateTime(globalMin);
        m_depart->setMinimumDateTime(globalMin);
        return;
    }

    // Do not clamp user input here (especially in update mode), otherwise Qt will auto-adjust the
    // date fields and the user won't see inline validation messages. Validation is handled by
    // validateInputs()/updateInlineErrors() + shake on submit.
    m_arrivee->setMinimumDateTime(globalMin);
    m_depart->setMinimumDateTime(globalMin);
}

bool QuaiFormDialog::validateInputs(QString *error) const
{
    const QString mat = m_matricule ? m_matricule->text().trimmed() : QString();
    if (mat.isEmpty())
    {
        if (error)
            *error = tr("Matricule est obligatoire.");
        return false;
    }
    if (mat.contains(' '))
    {
        if (error)
            *error = tr("Matricule ne doit pas contenir d'espaces.");
        return false;
    }

    // Same pattern as the validator: MQ-<number>
    {
        const QRegularExpression re(QStringLiteral(R"(^MQ-\d+$)"), QRegularExpression::CaseInsensitiveOption);
        if (!re.match(mat).hasMatch())
        {
            if (error)
                *error = tr("Matricule invalide. Format attendu: MQ- suivi d'un numéro (ex: MQ-12).");
            return false;
        }
    }

    const QDateTime a = m_arrivee ? m_arrivee->dateTime() : QDateTime();
    const QDateTime d = m_depart ? m_depart->dateTime() : QDateTime();
    if (!a.isValid() || !d.isValid())
    {
        if (error)
            *error = tr("Les dates/heures doivent être valides.");
        return false;
    }

    const QString etatText = m_etat ? m_etat->currentText().trimmed() : QString();
    const bool requireInterval = isOccupeText(etatText);
    if (requireInterval)
    {
        if (d <= a)
        {
            if (error)
                *error = tr("Pour l'état Occupé, la date/heure départ doit être après l'arrivée.");
            return false;
        }

        if (m_mode == Mode::Update && m_originalArrivee.isValid() && m_originalDepart.isValid())
        {
            if (a < m_originalArrivee || d < m_originalDepart)
            {
                if (error)
                    *error = tr("En modification, vous ne pouvez pas mettre une période avant la période initiale (Ajout).");
                return false;
            }
        }
    }

    if (m_taille && m_taille->value() < 0.0)
    {
        if (error)
            *error = tr("La taille du quai doit être >= 0.");
        return false;
    }
    if (m_prix && m_prix->value() < 0.0)
    {
        if (error)
            *error = tr("Le prix location doit être >= 0.");
        return false;
    }
    if (m_etat && m_etat->currentText().trimmed().isEmpty())
    {
        if (error)
            *error = tr("État quai est obligatoire.");
        return false;
    }

    // Additional coherence: if state is Libre/Maintenance, keep dates read-only (UI responsibility).
    if (m_etat && (isLibreText(etatText) || isMaintenanceText(etatText)))
    {
        // Nothing to enforce here besides validity; those dates are ignored by filtering/stats.
    }

    // Positions are optional but must be finite when present (spinboxes already constrain range).
    if (m_posX && !qIsFinite(m_posX->value()))
    {
        if (error)
            *error = tr("Position X invalide.");
        return false;
    }
    if (m_posY && !qIsFinite(m_posY->value()))
    {
        if (error)
            *error = tr("Position Y invalide.");
        return false;
    }

    return true;
}

void QuaiFormDialog::refreshOkEnabled()
{
    if (!m_btnOk)
        return;
    QString err;
    const bool ok = validateInputs(&err);
    // Keep the button clickable to allow a "shake" feedback on invalid submit.
    m_btnOk->setEnabled(true);
    // Keep a lightweight hint.
    m_btnOk->setToolTip(ok ? QString() : err);

    updateInlineErrors();
}

void QuaiFormDialog::accept()
{
    QString err;
    if (!validateInputs(&err))
    {
        updateInlineErrors();
        shakeWidget(this);
        return;
    }
    QDialog::accept();
}

Quai QuaiFormDialog::quai() const
{
    Quai q;
    // If we're editing an existing quai, keep its ID. For create, leave empty (auto-generated by the model).
    q.idQuai = (m_mode == Mode::Update) ? m_currentIdQuai : QString();
    q.matricule = m_matricule->text();
    q.typeQuai = QString();
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
