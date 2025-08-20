#pragma once

#include <QMainWindow>

#include "registers/iregistersparameters.h"

class JsonDataParser;

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
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void readRegisters();
    void initRegisters();
    void initConnection();
    void updateAllRegisters();

    Ui::MainWindow *m_ui;

    JsonDataParser *m_jsonParser;

    Connection::ConnectionDialog *m_connectionDialog;
    Connection::ConnectionManager *m_connectionManager;

    Registers::RegistersDialog *m_registersDialog;
    Registers::RegistersManager *m_registersManager;

};
