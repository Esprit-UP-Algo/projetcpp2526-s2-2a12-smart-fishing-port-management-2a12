#ifndef EMPLOYE_PAYSLIP_H
#define EMPLOYE_PAYSLIP_H

#include <QDialog>

namespace Ui { class EmployePayslipDialog; }

namespace employes {

class EmployePayslipDialog : public QDialog
{
    Q_OBJECT
public:
    explicit EmployePayslipDialog(QWidget *parent = nullptr);
    ~EmployePayslipDialog();

    void setInfo(const QString &employeeName, const QString &month, double heures);

private:
    Ui::EmployePayslipDialog *ui;
};

} // namespace employes

#endif // EMPLOYE_PAYSLIP_H
