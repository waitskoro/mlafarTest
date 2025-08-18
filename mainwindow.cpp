#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "connection/connectiondialog.h"
#include "connection/connectionmanager.h"
#include "connection/connectionparameters.h"

#include "registers/registersdialog.h"
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
    , m_registersDialog(new Registers::RegistersDialog(this))
    , m_dataModel(new QMap<RegisterType, RegistersChangeModel>())
    , m_changeModel(new QMap<RegisterType, RegistersChangeModel>())
{
    m_ui->setupUi(this);

    initRegisters();
    initConnection();

    connect(m_ui->actionExit, &QAction::triggered, [this]() { close(); });
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::initRegisters()
{
    connect(m_ui->actionRegisters, &QAction::triggered, [this] {
        m_registersDialog->show();
    });

    connect(m_registersDialog, &RegistersDialog::neededAllRegisters, [this]() {
        m_registersDialog->setRegisters(Dos, m_registersManager->registers(Dos));
        m_registersDialog->setRegisters(Pco, m_registersManager->registers(Pco));
        m_registersDialog->setRegisters(Common, m_registersManager->registers(Common));
    });

    connect(m_registersDialog,
            &RegistersDialog::neededRegisters,
            [this](RegisterType type) {
                m_registersDialog->setRegisters(type, m_registersManager->registers(type));
            });

    connect(m_registersDialog,
            &RegistersDialog::saveRegisters,
            [this](RegisterType type, QVector<Register> &regs) {
                m_registersManager->saveRegisters(type, regs);
            });

    m_registersDialog->setRegisters(Dos, m_registersManager->registers(Dos));
    m_registersDialog->setRegisters(Pco, m_registersManager->registers(Pco));
    m_registersDialog->setRegisters(Common, m_registersManager->registers(Common));
}

void MainWindow::initConnection()
{
    connect(m_ui->actionSettings, &QAction::triggered, [this]() {
        m_connectionDialog->show();
    });

    connect(m_ui->actionConnection, &QAction::triggered, [this](){
        if (m_ui->actionConnection->text() == "Подключиться")
            m_connectionManager->connectToServer();
        else if (m_ui->actionConnection->text() == "Отключиться")
            m_connectionManager->disconnectFromDevice();
    });

    connect(m_connectionManager,
            &ConnectionManager::stateChanged,
            [this](bool state){
                state ? m_ui->actionConnection->setText("Отключиться")
                      : m_ui->actionConnection->setText("Подключиться");
    });
}
