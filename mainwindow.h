#pragma once

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

namespace Registers {
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
};
