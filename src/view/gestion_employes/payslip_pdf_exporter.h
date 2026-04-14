#ifndef PAYSLIP_PDF_EXPORTER_H
#define PAYSLIP_PDF_EXPORTER_H

#include <QMap>
#include <QString>
#include <QVector>

#include "../../modele/gestion_employes/employe_user.h"

class QPainter;
class QPdfWriter;

namespace employes {

class PayslipPdfExporter
{
public:
    struct PayrollConfig {
        double cnssRate = 0.0918;
        double taxDefaultRate = 0.10;
        QMap<QString, double> taxRateByRole;
    };

    struct SalaryDetails {
        double hourlyRate = 0.0;
        double hoursWorked = 0.0;
        double cnssRate = 0.0;
        double taxRate = 0.0;
        double brut = 0.0;
        double cnss = 0.0;
        double impot = 0.0;
        double net = 0.0;
    };

    // Returns an empty string on success, otherwise an error message.
    static QString createPDF(const EmployeUser &user, const QString &month, const QString &outputPath);

private:
    static PayrollConfig loadPayrollConfig();
    static bool calculateSalaryDetails(double heuresTravail,
                                       const QString &role,
                                       const PayrollConfig &config,
                                       SalaryDetails *salary,
                                       QString *errorMessage);

    static double calculateTax(double salaireBrut,
                               const QString &role,
                               const PayrollConfig &config,
                               double *appliedRate);

    static int drawHeader(QPainter &p, QPdfWriter &writer,
                          const EmployeUser &user,
                          const QString &month,
                          int pageNumber);

    static int ensurePageSpace(QPainter &p,
                               QPdfWriter &writer,
                               int y,
                               int neededHeight,
                               int &pageNumber,
                               const EmployeUser &user,
                               const QString &month);

    static int drawSectionTitle(QPainter &p,
                                int y,
                                int left,
                                int width,
                                const QString &title);

    static int drawTable(QPainter &p, QPdfWriter &writer,
                         int y,
                         const QVector<QPair<QString, QString>> &rows,
                         int left,
                         int width,
                         int pageBottom,
                         int &pageNumber,
                         const EmployeUser &user,
                         const QString &month,
                          const QString &sectionTitle);

    static int drawNetHighlight(QPainter &p,
                                int y,
                                int left,
                                int width,
                                double salaireNet);

    static int drawCalculationBreakdown(QPainter &p,
                                        QPdfWriter &writer,
                                        int y,
                                        int left,
                                        int width,
                                        int pageBottom,
                                        int &pageNumber,
                                        const EmployeUser &user,
                                        const QString &month,
                                        const SalaryDetails &salary);

    static int drawFooter(QPainter &p,
                          int y,
                          int left,
                          int width);
};

} // namespace employes

#endif // PAYSLIP_PDF_EXPORTER_H
