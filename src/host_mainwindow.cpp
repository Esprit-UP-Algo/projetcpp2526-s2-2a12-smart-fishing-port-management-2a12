#include "host_mainwindow.h"
#include "ui_host_mainwindow.h"

#include "../../gestiondesguai/src/mainwindow.h"
#include "../../transaction/mainwindow.h"

HostMainWindow::HostMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HostMainWindow)
    , m_quaisWindow(new quais::MainWindow(this))
    , m_transactionsWindow(new transactions::MainWindow(this))
{
    ui->setupUi(this);

    ui->stack->addWidget(m_quaisWindow);
    ui->stack->addWidget(m_transactionsWindow);
    ui->stack->setCurrentWidget(m_quaisWindow);

    connect(m_quaisWindow, &quais::MainWindow::requestShowTransactions,
            this, &HostMainWindow::showTransactions);
    connect(m_transactionsWindow, &transactions::MainWindow::requestShowQuais,
            this, &HostMainWindow::showQuais);
}

HostMainWindow::~HostMainWindow()
{
    delete ui;
}

void HostMainWindow::showQuais()
{
    ui->stack->setCurrentWidget(m_quaisWindow);
    m_quaisWindow->setSidebarActiveQuais();
}

void HostMainWindow::showTransactions()
{
    ui->stack->setCurrentWidget(m_transactionsWindow);
    m_transactionsWindow->setSidebarActiveVentes();
}
