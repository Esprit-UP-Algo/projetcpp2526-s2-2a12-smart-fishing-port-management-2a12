#ifndef EMPLOYE_USERDIALOG_H
#define EMPLOYE_USERDIALOG_H

#include <QDialog>
#include "employe_user.h"

namespace Ui { class EmployeUserDialog; }

namespace employes {

class EmployeUserDialog : public QDialog
{
    Q_OBJECT
public:
    explicit EmployeUserDialog(QWidget *parent = nullptr, const QString &title = "Nouvel utilisateur");
    ~EmployeUserDialog();

    void setUser(const EmployeUser &u);
    EmployeUser user() const;
    void setReadOnly(bool ro);

private:
    Ui::EmployeUserDialog *ui;
};

} // namespace employes

#endif // EMPLOYE_USERDIALOG_H
