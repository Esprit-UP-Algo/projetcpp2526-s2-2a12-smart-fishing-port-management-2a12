#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class HostMainWindow; }
QT_END_NAMESPACE

namespace quais { class MainWindow; }
namespace transactions { class MainWindow; }
namespace navires { class MainWindow; }
namespace captures { class MainWindow; }

class HostMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit HostMainWindow(QWidget *parent = nullptr);
    ~HostMainWindow();

private slots:
    void showQuais();
    void showTransactions();
    void showNavires();
    void showCaptures();

private:
    Ui::HostMainWindow *ui;
    quais::MainWindow *m_quaisWindow = nullptr;
    transactions::MainWindow *m_transactionsWindow = nullptr;
    navires::MainWindow *m_naviresWindow = nullptr;
    captures::MainWindow *m_capturesWindow = nullptr;
};
