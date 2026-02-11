#include "payslip.h"
#include "ui_payslipdialog.h"

PayslipDialog::PayslipDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PayslipDialog)
{
    ui->setupUi(this);
    connect(ui->btnExportPDF, &QPushButton::clicked, this, &QDialog::accept);
}

PayslipDialog::~PayslipDialog()
{
    delete ui;
}

void PayslipDialog::setInfo(const QString &employeeName, const QString &month, double heures)
{
    Q_UNUSED(employeeName);
    Q_UNUSED(month);
    Q_UNUSED(heures);
}
