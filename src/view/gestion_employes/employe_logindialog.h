#ifndef EMPLOYE_LOGINDIALOG_H
#define EMPLOYE_LOGINDIALOG_H

#include <QDialog>
#include "../../modele/gestion_employes/employe_dao.h"
#include "../../modele/gestion_employes/auth_helper.h"

namespace Ui {
class EmployeLoginDialog;
}

namespace employes {

class EmployeLoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EmployeLoginDialog(QWidget *parent = nullptr);
    ~EmployeLoginDialog();

private slots:
    void onLoginClicked();

private:
    Ui::EmployeLoginDialog *ui;
};

} // namespace employes

#endif // EMPLOYE_LOGINDIALOG_H
