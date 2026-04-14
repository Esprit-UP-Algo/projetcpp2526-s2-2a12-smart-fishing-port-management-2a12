#include "payslip_pdf_exporter.h"

#include "config_manager.h"
#include "../../modele/gestion_employes/employe_dao.h"

#include <QDate>
#include <QFileInfo>
#include <QPainter>
#include <QPdfWriter>
#include <QPixmap>
#include <QStringList>

#include <cmath>
#include <stdexcept>

namespace employes {

namespace {
constexpr int kPageMargin = 96;
constexpr int kHeaderHeight = 176;
constexpr int kSectionGap = 18;
constexpr int kSectionTitleH = 38;
constexpr int kTableHeaderH = 34;
constexpr int kTableRowH = 32;

const QColor kHeaderColor("#101c2c");
const QColor kCardColor("#132437");
const QColor kAccentColor("#39c0fa");
const QColor kTextLight("#e6eef6");
const QColor kTextDark("#0f172a");
const QColor kMuted("#64748b");
const QColor kTableHeaderBg("#e6eef6");
const QColor kTableRowBg1("#ffffff");
const QColor kTableRowBg2("#f7fbff");
const QColor kNetBg("#d7f1ff");
const QColor kBorder("#cfdbe8");

QString money(double value)
{
    return QString::number(value, 'f', 2) + " DT";
}

QString normalizeRoleKey(const QString &role)
{
    QString key = role.trimmed().toUpper();
    for (QChar &ch : key) {
        if (!ch.isLetterOrNumber()) {
            ch = '_';
        }
    }
    return key;
}

bool readConfigDouble(const QString &key, double fallbackValue, double *out)
{
    if (!out) {
        return false;
    }

    const QString text = ConfigManager::get(key).trimmed();
    if (text.isEmpty()) {
        *out = fallbackValue;
        return true;
    }

    bool ok = false;
    const double value = text.toDouble(&ok);
    if (!ok || !std::isfinite(value)) {
        *out = fallbackValue;
        return false;
    }

    *out = value;
    return true;
}
} // namespace

PayslipPdfExporter::PayrollConfig PayslipPdfExporter::loadPayrollConfig()
{
    PayrollConfig config;

    // Rates are configurable; defaults keep compatibility.
    readConfigDouble("PAYROLL_CNSS_RATE", 0.0918, &config.cnssRate);
    readConfigDouble("PAYROLL_TAX_RATE_DEFAULT", 0.10, &config.taxDefaultRate);

    const QStringList knownRoles = {
        "ouvrier", "technicien", "agent", "chef_equipe", "superviseur", "admin",
        "RH (Ressources Humaines)", "Agent d'exploitation", "Responsable stock", "Responsable finance"
    };

    for (const QString &role : knownRoles) {
        const QString key = "PAYROLL_TAX_RATE_ROLE_" + normalizeRoleKey(role);
        double roleRate = config.taxDefaultRate;
        if (readConfigDouble(key, config.taxDefaultRate, &roleRate)) {
            config.taxRateByRole.insert(normalizeRoleKey(role), roleRate);
        }
    }

    return config;
}

bool PayslipPdfExporter::calculateSalaryDetails(double heuresTravail,
                                                const QString &role,
                                                const PayrollConfig &config,
                                                SalaryDetails *salary,
                                                QString *errorMessage)
{
    if (!salary) {
        if (errorMessage) {
            *errorMessage = "Erreur interne de calcul.";
        }
        return false;
    }

    if (heuresTravail <= 0.0) {
        if (errorMessage) {
            *errorMessage = "Le nombre d'heures doit être strictement positif.";
        }
        return false;
    }

    if (role.trimmed().isEmpty()) {
        if (errorMessage) {
            *errorMessage = "Le rôle est obligatoire pour le calcul salarial.";
        }
        return false;
    }

    if (config.cnssRate < 0.0 || config.taxDefaultRate < 0.0) {
        if (errorMessage) {
            *errorMessage = "Les taux de paie configurés sont invalides.";
        }
        return false;
    }

    const double tauxHoraire = EmployeDAO::getTauxHoraire(role);
    if (tauxHoraire <= 0.0) {
        if (errorMessage) {
            *errorMessage = "Le taux horaire est invalide.";
        }
        return false;
    }

    // Required strict pipeline:
    // brut = heures * taux
    // cnss = brut * taux_cnss
    // impot = calculateTax(brut, role)
    // net = brut - cnss - impot
    const double brut = heuresTravail * tauxHoraire;
    if (brut < 0.0) {
        if (errorMessage) {
            *errorMessage = "Le salaire brut calculé est invalide.";
        }
        return false;
    }

    double appliedTaxRate = config.taxDefaultRate;
    const double cnss = brut * config.cnssRate;
    const double impot = calculateTax(brut, role, config, &appliedTaxRate);
    const double net = brut - cnss - impot;

    if (cnss < 0.0 || impot < 0.0 || net < 0.0) {
        if (errorMessage) {
            *errorMessage = "Les retenues calculées sont invalides.";
        }
        return false;
    }

    salary->hourlyRate = tauxHoraire;
    salary->hoursWorked = heuresTravail;
    salary->cnssRate = config.cnssRate;
    salary->taxRate = appliedTaxRate;
    salary->brut = brut;
    salary->cnss = cnss;
    salary->impot = impot;
    salary->net = net;

    return true;
}

double PayslipPdfExporter::calculateTax(double salaireBrut,
                                        const QString &role,
                                        const PayrollConfig &config,
                                        double *appliedRate)
{
    const QString roleKey = normalizeRoleKey(role);
    const double roleRate = config.taxRateByRole.value(roleKey, config.taxDefaultRate);
    const double usedRate = (roleRate < 0.0) ? 0.0 : roleRate;

    if (appliedRate) {
        *appliedRate = usedRate;
    }
    return salaireBrut * usedRate;
}

QString PayslipPdfExporter::createPDF(const EmployeUser &user, const QString &month, const QString &outputPath)
{
    if (outputPath.trimmed().isEmpty()) {
        return "Le chemin de sortie du PDF est vide.";
    }
    if (user.nom.trimmed().isEmpty() || user.prenom.trimmed().isEmpty()) {
        return "Les informations employé sont incomplètes (Nom/Prénom manquants).";
    }
    if (user.cin.trimmed().isEmpty() || user.role.trimmed().isEmpty()) {
        return "Les informations employé sont incomplètes (CIN/Rôle manquants).";
    }
    if (user.heures <= 0.0) {
        return "Le nombre d'heures doit être strictement positif.";
    }
    if (month.trimmed().isEmpty()) {
        return "Le mois de paie est obligatoire.";
    }

    QPdfWriter writer(outputPath);
    writer.setPageSize(QPageSize::A4);
    writer.setResolution(300);
    writer.setPageMargins(QMarginsF(8, 8, 8, 8));

    QPainter p(&writer);
    if (!p.isActive()) {
        return "Impossible d'initialiser le générateur PDF.";
    }

    const PayrollConfig payrollConfig = loadPayrollConfig();
    SalaryDetails salary;
    QString salaryError;
    try {
        if (!calculateSalaryDetails(user.heures, user.role, payrollConfig, &salary, &salaryError)) {
            return salaryError;
        }
    } catch (const std::exception &e) {
        return QString("Erreur de calcul salarial: %1").arg(e.what());
    }

    const int pageW = writer.width();
    const int pageBottom = writer.height() - kPageMargin;
    const int left = kPageMargin;
    const int width = pageW - (2 * kPageMargin);

    int pageNumber = 1;
    int y = drawHeader(p, writer, user, month, pageNumber);

    // B. Employee Information
    QVector<QPair<QString, QString>> employeeRows;
    employeeRows.append({"Nom", user.nom});
    employeeRows.append({"Prénom", user.prenom});
    employeeRows.append({"CIN", user.cin});
    employeeRows.append({"Rôle", user.role});

    y = drawTable(p, writer, y, employeeRows, left, width, pageBottom, pageNumber, user, month,
                  "B. Informations Employé");
    y += kSectionGap;

    // C. Work Information
    QVector<QPair<QString, QString>> workRows;
    workRows.append({"Mois", month});
    workRows.append({"Heures de travail", QString::number(salary.hoursWorked, 'f', 2) + " h"});
    workRows.append({"Statut", user.statut.isEmpty() ? "--" : user.statut});

    y = drawTable(p, writer, y, workRows, left, width, pageBottom, pageNumber, user, month,
                  "C. Informations Travail");
    y += kSectionGap;

    // D. Salary Details
    QVector<QPair<QString, QString>> salaryRows;
    salaryRows.append({"Taux horaire", money(salary.hourlyRate)});
    salaryRows.append({"Salaire brut", money(salary.brut)});
    salaryRows.append({QString("Retenue CNSS (%1%)").arg(QString::number(salary.cnssRate * 100.0, 'f', 2)), money(salary.cnss)});
    salaryRows.append({QString("Impôt (%1%)").arg(QString::number(salary.taxRate * 100.0, 'f', 2)), money(salary.impot)});

    y = drawTable(p, writer, y, salaryRows, left, width, pageBottom, pageNumber, user, month,
                  "D. Détails du Salaire");
    y += 10;
    y = drawNetHighlight(p, y, left, width, salary.net);
    y += kSectionGap;

    // E. Calculation Breakdown
    y = drawCalculationBreakdown(p, writer, y, left, width, pageBottom, pageNumber, user, month, salary);
    y += kSectionGap;

    // F. Footer
    y = ensurePageSpace(p, writer, y, 90, pageNumber, user, month);
    drawFooter(p, y, left, width);

    p.end();
    return "";
}

int PayslipPdfExporter::drawHeader(QPainter &p, QPdfWriter &writer,
                                   const EmployeUser &user,
                                   const QString &month,
                                   int pageNumber)
{
    Q_UNUSED(user);

    const int pageW = writer.width();
    const int left = kPageMargin;
    const int width = pageW - (2 * kPageMargin);

    p.fillRect(QRect(0, 0, pageW, kHeaderHeight), kHeaderColor);

    const int logoSize = 82;
    const QRect logoRect(left, 44, logoSize, logoSize);
    p.setBrush(QBrush(Qt::white));
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(logoRect, 12, 12);

    const QPixmap logo(":/vision_sight.jpeg");
    if (!logo.isNull()) {
        p.drawPixmap(logoRect.adjusted(8, 8, -8, -8),
                     logo.scaled(logoRect.width() - 16, logoRect.height() - 16,
                                 Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    QFont companyFont("Arial", 16, QFont::Bold);
    p.setFont(companyFont);
    p.setPen(kTextLight);
    p.drawText(QRect(left + 102, 48, width - 102, 30), Qt::AlignLeft | Qt::AlignVCenter,
               "VISION SIGHT");

    QFont titleFont("Arial", 24, QFont::Bold);
    p.setFont(titleFont);
    p.drawText(QRect(left + 102, 78, width - 102, 44), Qt::AlignLeft | Qt::AlignVCenter,
               "FICHE DE PAIE");

    QFont subFont("Arial", 10);
    p.setFont(subFont);
    p.setPen(QColor("#b6c7d8"));
    p.drawText(QRect(left + 102, 124, width - 102, 20), Qt::AlignLeft | Qt::AlignVCenter,
               QString("Mois: %1    Date: %2")
               .arg(month, QDate::currentDate().toString("dd/MM/yyyy")));

    p.drawText(QRect(left, 146, width, 20), Qt::AlignRight | Qt::AlignVCenter,
               QString("Page %1").arg(pageNumber));

    p.fillRect(QRect(left, kHeaderHeight - 4, width, 4), kAccentColor);
    return kHeaderHeight + 24;
}

int PayslipPdfExporter::ensurePageSpace(QPainter &p,
                                        QPdfWriter &writer,
                                        int y,
                                        int neededHeight,
                                        int &pageNumber,
                                        const EmployeUser &user,
                                        const QString &month)
{
    const int pageBottom = writer.height() - kPageMargin;
    if (y + neededHeight <= pageBottom) {
        return y;
    }

    writer.newPage();
    ++pageNumber;
    return drawHeader(p, writer, user, month, pageNumber);
}

int PayslipPdfExporter::drawSectionTitle(QPainter &p,
                                         int y,
                                         int left,
                                         int width,
                                         const QString &title)
{
    p.setPen(Qt::NoPen);
    p.setBrush(QBrush(kCardColor));
    p.drawRoundedRect(QRect(left, y, width, kSectionTitleH), 10, 10);

    QFont f("Arial", 11, QFont::Bold);
    p.setFont(f);
    p.setPen(kTextLight);
    p.drawText(QRect(left + 14, y, width - 20, kSectionTitleH), Qt::AlignLeft | Qt::AlignVCenter, title);

    return y + kSectionTitleH;
}

int PayslipPdfExporter::drawTable(QPainter &p, QPdfWriter &writer,
                                  int y,
                                  const QVector<QPair<QString, QString>> &rows,
                                  int left,
                                  int width,
                                  int pageBottom,
                                  int &pageNumber,
                                  const EmployeUser &user,
                                  const QString &month,
                                  const QString &sectionTitle)
{
    const int titleHeight = sectionTitle.trimmed().isEmpty() ? 0 : kSectionTitleH;
    const int needed = titleHeight + kTableHeaderH + (rows.size() * kTableRowH) + 8;
    y = ensurePageSpace(p, writer, y, needed, pageNumber, user, month);

    if (titleHeight > 0) {
        y = drawSectionTitle(p, y, left, width, sectionTitle);
    }

    const int col1 = static_cast<int>(width * 0.45);
    const int col2 = width - col1;

    p.setPen(QPen(kBorder, 1));
    p.setBrush(QBrush(kTableHeaderBg));
    p.drawRect(QRect(left, y, width, kTableHeaderH));

    QFont th("Arial", 10, QFont::Bold);
    p.setFont(th);
    p.setPen(kTextDark);
    p.drawText(QRect(left + 10, y, col1 - 10, kTableHeaderH), Qt::AlignLeft | Qt::AlignVCenter, "Champ");
    p.drawText(QRect(left + col1 + 10, y, col2 - 10, kTableHeaderH), Qt::AlignLeft | Qt::AlignVCenter, "Valeur");

    y += kTableHeaderH;

    QFont rowFont("Arial", 10);
    p.setFont(rowFont);

    for (int i = 0; i < rows.size(); ++i) {
        y = ensurePageSpace(p, writer, y, kTableRowH + 4, pageNumber, user, month);

        const QColor rowBg = (i % 2 == 0) ? kTableRowBg1 : kTableRowBg2;
        p.setPen(QPen(kBorder, 1));
        p.setBrush(QBrush(rowBg));
        p.drawRect(QRect(left, y, width, kTableRowH));
        p.drawLine(left + col1, y, left + col1, y + kTableRowH);

        p.setPen(kTextDark);
        p.drawText(QRect(left + 10, y, col1 - 14, kTableRowH), Qt::AlignLeft | Qt::AlignVCenter, rows[i].first);
        p.drawText(QRect(left + col1 + 10, y, col2 - 14, kTableRowH), Qt::AlignLeft | Qt::AlignVCenter, rows[i].second);

        y += kTableRowH;
    }

    return y;
}

int PayslipPdfExporter::drawNetHighlight(QPainter &p,
                                         int y,
                                         int left,
                                         int width,
                                         double salaireNet)
{
    const int h = 48;
    p.setPen(QPen(QColor("#8ad5f5"), 2));
    p.setBrush(QBrush(kNetBg));
    p.drawRoundedRect(QRect(left, y, width, h), 8, 8);

    QFont label("Arial", 11, QFont::Bold);
    p.setFont(label);
    p.setPen(kTextDark);
    p.drawText(QRect(left + 12, y, width / 2, h), Qt::AlignLeft | Qt::AlignVCenter, "Salaire net");

    QFont valueFont("Arial", 14, QFont::Bold);
    p.setFont(valueFont);
    p.setPen(QColor("#0a4a66"));
    p.drawText(QRect(left + width / 2, y, width / 2 - 12, h), Qt::AlignRight | Qt::AlignVCenter,
               money(salaireNet));

    return y + h;
}

int PayslipPdfExporter::drawCalculationBreakdown(QPainter &p,
                                                 QPdfWriter &writer,
                                                 int y,
                                                 int left,
                                                 int width,
                                                 int pageBottom,
                                                 int &pageNumber,
                                                 const EmployeUser &user,
                                                 const QString &month,
                                                 const SalaryDetails &salary)
{
    Q_UNUSED(pageBottom);

    y = ensurePageSpace(p, writer, y, 210, pageNumber, user, month);
    y = drawSectionTitle(p, y, left, width, "E. Détail du Calcul");

    const QVector<QPair<QString, QString>> rows = {
        {
            "Brut = heures × taux",
            QString("%1 h × %2 = %3")
                .arg(QString::number(salary.hoursWorked, 'f', 2),
                     money(salary.hourlyRate),
                     money(salary.brut))
        },
        {
            QString("CNSS = brut × %1%").arg(QString::number(salary.cnssRate * 100.0, 'f', 2)),
            QString("%1 × %2 = %3")
                .arg(money(salary.brut),
                     QString::number(salary.cnssRate, 'f', 4),
                     money(salary.cnss))
        },
        {
            QString("Impôt = brut × %1%").arg(QString::number(salary.taxRate * 100.0, 'f', 2)),
            QString("%1 × %2 = %3")
                .arg(money(salary.brut),
                     QString::number(salary.taxRate, 'f', 4),
                     money(salary.impot))
        },
        {
            "Net = brut - CNSS - impôt",
            QString("%1 - %2 - %3 = %4")
                .arg(money(salary.brut),
                     money(salary.cnss),
                     money(salary.impot),
                     money(salary.net))
        }
    };

    return drawTable(p, writer, y, rows, left, width, writer.height() - kPageMargin,
                     pageNumber, user, month, "");
}

int PayslipPdfExporter::drawFooter(QPainter &p,
                                   int y,
                                   int left,
                                   int width)
{
    p.setPen(QPen(kMuted, 1));
    p.drawLine(left, y + 16, left + width, y + 16);

    QFont footerFont("Arial", 10);
    p.setFont(footerFont);
    p.setPen(kMuted);

    p.drawText(QRect(left, y + 22, width / 2, 24),
               Qt::AlignLeft | Qt::AlignVCenter,
               "Date: " + QDate::currentDate().toString("dd/MM/yyyy"));

    p.drawText(QRect(left + width / 2, y + 22, width / 2, 24),
               Qt::AlignRight | Qt::AlignVCenter,
               "Signature: ____________________");

    return y + 48;
}

} // namespace employes
