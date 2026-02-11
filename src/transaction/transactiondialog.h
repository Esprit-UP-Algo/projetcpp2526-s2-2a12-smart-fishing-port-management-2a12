#ifndef TRANSACTIONDIALOG_H
#define TRANSACTIONDIALOG_H

#include <QDialog>
#include "transaction.h"

QT_BEGIN_NAMESPACE
namespace Ui { class TransactionDialog; }
QT_END_NAMESPACE

class TransactionDialog : public QDialog
{
    Q_OBJECT
public:
    explicit TransactionDialog(QWidget *parent = nullptr,
                               const QString &title = "Nouvelle Transaction");
    ~TransactionDialog();

    void setTransaction(const Transaction &t);
    Transaction transaction() const;

    void setReadOnly(bool ro);

private slots:
    void recalcTotal();

private:
    Ui::TransactionDialog *ui;
};

#endif // TRANSACTIONDIALOG_H
