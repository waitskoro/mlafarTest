#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "connection/connectiondialog.h"
#include "connection/connectionmanager.h"
#include "connection/connectionparameters.h"

#include "registers/registersmanager.h"

using namespace Registers;
using namespace Connection;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_registersManager(new RegistersManager(this))
    , m_connectionParameters(new ConnectionParameters(this))
    , m_connectionDialog(new ConnectionDialog(m_connectionParameters, this))
    , m_connectionManager(new ConnectionManager(m_connectionParameters, this))
{
    m_ui->setupUi(this);

    initRegisters();
    initConnection();

    connect(m_ui->actionExit, &QAction::triggered, [this]() {
        close();
    });

    connect(m_ui->actionSettings, &QAction::triggered, [this] {
        m_connectionDialog->show();
    });
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::initRegisters()
{

}

void MainWindow::initConnection()
{

}

