#ifndef EMPLOYE_USERDIALOG_H
#define EMPLOYE_USERDIALOG_H

#include <QDialog>
#include "../../modele/gestion_employes/employe_user.h"

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
    void setupFieldValidators();
    void setupRealtimeValidation();
    bool validateForm(bool showMessage);
    void updateFieldState();
    void markInvalid(QWidget *widget, bool invalid, const QString &toolTip = QString());

    Ui::EmployeUserDialog *ui;
    bool m_isEditMode = false;
};

} // namespace employes

#endif // EMPLOYE_USERDIALOG_H
