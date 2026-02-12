#include "host_mainwindow.h"
#include "ui_host_mainwindow.h"

#include "quais/mainwindow.h"
#include "transaction/mainwindow.h"
#include "navires/mainwindow.h"
#include "suivi_des_captures/mainwindow.h"

HostMainWindow::HostMainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::HostMainWindow)
    , m_quaisWindow(new quais::MainWindow(this))
    , m_transactionsWindow(new transactions::MainWindow(this))
    , m_naviresWindow(new navires::MainWindow(this))
    , m_capturesWindow(new captures::MainWindow(this))
{
    ui->setupUi(this);

    ui->stack->addWidget(m_quaisWindow);
    ui->stack->addWidget(m_transactionsWindow);
    ui->stack->addWidget(m_naviresWindow);
    ui->stack->addWidget(m_capturesWindow);
    ui->stack->setCurrentWidget(m_quaisWindow);

    connect(m_quaisWindow, &quais::MainWindow::requestShowTransactions,
            this, &HostMainWindow::showTransactions);
    connect(m_transactionsWindow, &transactions::MainWindow::requestShowQuais,
            this, &HostMainWindow::showQuais);

        connect(m_quaisWindow, &quais::MainWindow::requestShowNavires,
            this, &HostMainWindow::showNavires);
        connect(m_transactionsWindow, &transactions::MainWindow::requestShowNavires,
            this, &HostMainWindow::showNavires);

        connect(m_naviresWindow, &navires::MainWindow::requestShowQuais,
            this, &HostMainWindow::showQuais);
        connect(m_naviresWindow, &navires::MainWindow::requestShowTransactions,
            this, &HostMainWindow::showTransactions);

        connect(m_quaisWindow, &quais::MainWindow::requestShowCaptures,
            this, &HostMainWindow::showCaptures);
        connect(m_transactionsWindow, &transactions::MainWindow::requestShowCaptures,
            this, &HostMainWindow::showCaptures);
        connect(m_naviresWindow, &navires::MainWindow::requestShowCaptures,
            this, &HostMainWindow::showCaptures);

        connect(m_capturesWindow, &captures::MainWindow::requestShowQuais,
            this, &HostMainWindow::showQuais);
        connect(m_capturesWindow, &captures::MainWindow::requestShowTransactions,
            this, &HostMainWindow::showTransactions);
        connect(m_capturesWindow, &captures::MainWindow::requestShowNavires,
            this, &HostMainWindow::showNavires);
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

void HostMainWindow::showNavires()
{
    ui->stack->setCurrentWidget(m_naviresWindow);
    m_naviresWindow->setSidebarActiveNavires();
}

void HostMainWindow::showCaptures()
{
    ui->stack->setCurrentWidget(m_capturesWindow);
    m_capturesWindow->setSidebarActiveCaptures();
}
