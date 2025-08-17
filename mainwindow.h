#pragma once

#include <QMainWindow>

namespace Ui {
class MainWindow;
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
    void initConnection();

    Ui::MainWindow *m_ui;
    Connection::ConnectionDialog *m_connectionDialog;
    Connection::ConnectionManager *m_connectionManager;
    Connection::ConnectionParameters *m_connectionParameters;
};
