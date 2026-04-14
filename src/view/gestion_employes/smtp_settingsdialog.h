#ifndef SMTP_SETTINGSDIALOG_H
#define SMTP_SETTINGSDIALOG_H

#include <QDialog>

class QLineEdit;

namespace employes {

class SmtpSettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SmtpSettingsDialog(QWidget *parent = nullptr);

private slots:
    void onSaveClicked();

private:
    void loadValues();

    QLineEdit *m_host = nullptr;
    QLineEdit *m_port = nullptr;
    QLineEdit *m_user = nullptr;
    QLineEdit *m_password = nullptr;
    QLineEdit *m_from = nullptr;
};

} // namespace employes

#endif // SMTP_SETTINGSDIALOG_H