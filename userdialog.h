#ifndef USERDIALOG_H
#define USERDIALOG_H

#include <QDialog>
#include "user.h"

namespace Ui { class UserDialog; }

class UserDialog : public QDialog
{
    Q_OBJECT
public:
    explicit UserDialog(QWidget *parent = nullptr, const QString &title = "Nouvel utilisateur");
    ~UserDialog();

    void setUser(const User &u);
    User user() const;
    void setReadOnly(bool ro);

private:
    Ui::UserDialog *ui;
};

#endif // USERDIALOG_H
