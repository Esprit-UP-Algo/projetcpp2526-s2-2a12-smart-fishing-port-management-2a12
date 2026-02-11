#ifndef PAYSLIP_H
#define PAYSLIP_H

#include <QDialog>

namespace Ui { class PayslipDialog; }

class PayslipDialog : public QDialog
{
    Q_OBJECT
public:
    explicit PayslipDialog(QWidget *parent = nullptr);
    ~PayslipDialog();

    void setInfo(const QString &employeeName, const QString &month, double heures);

private:
    Ui::PayslipDialog *ui;
};

#endif // PAYSLIP_H
