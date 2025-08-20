#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>

#include "jsondataparser.h"

#include "connection/connectiondialog.h"
#include "connection/connectionmanager.h"
#include "connection/connectionparameters.h"

#include "registers/registersdialog.h"
#include "registers/registersmanager.h"
#include "registers/registersdatamodel.h"

using namespace Registers;
using namespace Connection;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_ui(new Ui::MainWindow)
    , m_jsonParser(new JsonDataParser(this))
    , m_connectionDialog(new ConnectionDialog(m_jsonParser->readConnectionParameters(), this))
    , m_connectionManager(new ConnectionManager(m_jsonParser->readConnectionParameters(), this))
    , m_registersDialog(new Registers::RegistersDialog(this))
    , m_registersManager(new RegistersManager(this))
{
    m_ui->setupUi(this);

    initRegisters();
    initConnection();

    m_ui->tableViewDos->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_ui->tableViewPco->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    m_ui->tableViewCommon->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    connect(m_ui->actionExit, &QAction::triggered, [this]() { close(); });
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::initRegisters()
{
    updateAllRegisters();

    m_ui->tableViewDos->setModel(m_registersManager->modelData(Dos));
    m_ui->tableViewPco->setModel(m_registersManager->modelData(Pco));
    m_ui->tableViewCommon->setModel(m_registersManager->modelData(Common));

    connect(m_ui->actionRegisters, &QAction::triggered, [this] {
        m_registersDialog->show();
    });

    connect(m_registersDialog, &RegistersDialog::neededAllRegisters, [this]() {
        updateAllRegisters();
    });

    connect(m_registersDialog,
            &RegistersDialog::neededRegisters,
            [this](RegisterType type) {
                m_registersDialog->setRegisters(type,
                                                m_registersManager->registers(type));
            });

    connect(m_registersDialog,
            &RegistersDialog::saveRegisters,
            [this](RegisterType type, QVector<Register> *registers) {
                m_jsonParser->writeRegisters(type, *registers);
                updateAllRegisters();
            });
}

void MainWindow::initConnection()
{
    connect(m_ui->actionSettings, &QAction::triggered, [this]() {
        m_connectionDialog->show();
    });

    connect(m_connectionDialog,
            &ConnectionDialog::saveConnection,
            [this](ConnectionParameters *parameters){
               if (m_jsonParser->writeConnectionParameters(parameters)) {
                   qDebug() << "Connection parameteres is saved";
               }
            });

    connect(m_ui->actionConnection, &QAction::triggered, [this](){
        if (m_ui->actionConnection->text() == "Подключиться") {
            m_connectionManager->connectToServer();
        } else {
            m_connectionManager->disconnectFromDevice();
        }
    });

    connect(m_connectionManager,
            &ConnectionManager::stateChanged,
            [this](bool state){
                m_ui->actionConnection->setText(state ? "Отключиться" : "Подключиться");
                if (state)
                    readRegisters();
            });

    connect(m_connectionManager,
            &ConnectionManager::registersRead,
            [this](Registers::RegisterType type, QVector<Registers::Register> *registers){
                m_registersManager->setRegisters(type, registers);
            });
}

void MainWindow::readRegisters()
{
    QTimer *m_timer = new QTimer();
    m_timer->setInterval(1000);
    m_timer->start();

    connect(m_timer, &QTimer::timeout, [this](){
        for (const auto type : types) {
            m_connectionManager->readRegisters(type, m_jsonParser->readRegisters(type));
            m_registersManager->setRegisters(type, m_jsonParser->readRegisters(type));
        }
    });
}

void MainWindow::updateAllRegisters()
{
    for (const auto type : types) {
        m_registersDialog->setRegisters(type, m_jsonParser->readRegisters(type));
        m_registersManager->setRegisters(type, m_jsonParser->readRegisters(type));
    }
}
