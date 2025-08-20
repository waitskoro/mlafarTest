#pragma once

#include <QWidget>
#include <QDialog>

#include "connectionparameters.h"

namespace Ui {
class ConnectionDialog;
}

namespace Connection {

class ConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectionDialog(ConnectionParameters *parametrs, QWidget *parent = nullptr);
    ~ConnectionDialog();
signals:
    void saveConnection(ConnectionParameters *);

private:
    void updateUiFromParameters();

    void onSaveClicked();
    void onCancelClicked();

    Ui::ConnectionDialog *ui;
    ConnectionParameters *m_parameters;

    QMap<int, ConnectionParameters::Protocol> protocolMap = {
        {0, ConnectionParameters::IPv4},
        {1, ConnectionParameters::IPv6}
    };
};

}
