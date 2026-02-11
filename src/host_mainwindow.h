#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class HostMainWindow; }
QT_END_NAMESPACE

namespace quais { class MainWindow; }
namespace transactions { class MainWindow; }

class HostMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HostMainWindow(QWidget *parent = nullptr);
    ~HostMainWindow();

private slots:
    void showQuais();
    void showTransactions();

private:
    Ui::HostMainWindow *ui;
    quais::MainWindow *m_quaisWindow = nullptr;
    transactions::MainWindow *m_transactionsWindow = nullptr;
};
