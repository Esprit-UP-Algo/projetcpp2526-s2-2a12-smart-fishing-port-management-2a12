#include "employe_payslip.h"
#include "ui_employe_payslipdialog.h"

namespace employes {

EmployePayslipDialog::EmployePayslipDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EmployePayslipDialog)
{
    ui->setupUi(this);
    connect(ui->btnExportPDF, &QPushButton::clicked, this, &QDialog::accept);
}

EmployePayslipDialog::~EmployePayslipDialog()
{
    delete ui;
}

void EmployePayslipDialog::setInfo(const QString &employeeName, const QString &month, double heures)
{
    Q_UNUSED(employeeName);
    Q_UNUSED(month);
    Q_UNUSED(heures);
}

} // namespace employes
