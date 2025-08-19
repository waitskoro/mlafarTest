#pragma once

#include <QMainWindow>

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
    void updateAllRegisters();

    Ui::MainWindow *m_ui;

    Connection::ConnectionParameters *m_connectionParameters;
    Connection::ConnectionDialog *m_connectionDialog;
    Connection::ConnectionManager *m_connectionManager;

    Registers::RegistersDialog *m_registersDialog;
    Registers::RegistersManager *m_registersManager;
};
