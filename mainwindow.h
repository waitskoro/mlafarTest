#pragma once

#include <QMainWindow>

#include "registers/registersdatamodel.h"
#include "registers/registerschangemodel.h"
#include "registers/iregistersparameters.h"

namespace Ui {
class MainWindow;
}

namespace Registers {
class RegistersDialog;
class RegistersManager;
}

namespace Connection {
class ConnectionDialog;
class ConnectionManager;
class ConnectionParameters;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void initRegisters();
    void initConnection();

    Ui::MainWindow *m_ui;
    Registers::RegistersManager *m_registersManager;

    Connection::ConnectionParameters *m_connectionParameters;
    Connection::ConnectionDialog *m_connectionDialog;
    Connection::ConnectionManager *m_connectionManager;

    Registers::RegistersDialog *m_registersDialog;

    QMap<Registers::RegisterType, Registers::RegistersDataModel> *m_dataModel;
    QMap<Registers::RegisterType, Registers::RegistersChangeModel> *m_changeModel;
};
