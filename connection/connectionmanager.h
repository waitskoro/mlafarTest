#pragma once

#include <QObject>

namespace Connection {

class ModBusClient;
class ConnectionParameters;

class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionManager(ConnectionParameters *parameters,
                               QObject *parent = nullptr);

    void connectToServer();
    void disconnectFromDevice();

signals:
    void stateChanged(bool connection);

private:
    ModBusClient *m_client;
    ConnectionParameters *m_parameters;

    void onParametersChanged();

};

}
