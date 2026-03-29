#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>

class QLineEdit;
class QLabel;

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);

private slots:
    void tryConnect();

private:
    void loadSettings();
    void saveSettings(const QString &dsn, const QString &user, const QString &password);

    QLineEdit *editDsn = nullptr;
    QLineEdit *editUser = nullptr;
    QLineEdit *editPassword = nullptr;
    QLabel *lblStatus = nullptr;
};

#endif // LOGINDIALOG_H
