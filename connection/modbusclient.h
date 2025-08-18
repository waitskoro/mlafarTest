#pragma once

#include <QObject>
#include <QModbusDevice>
#include <QModbusDataUnit>

class QModbusReply;
class QModbusClient;

namespace Connection {

class ConnectionParameters;

class ModBusClient : public QObject
{
    Q_OBJECT
public:
    explicit ModBusClient(ConnectionParameters *parameters,
                          QObject *parent = nullptr);
    ~ModBusClient();

    void connectToServer();
    void disconnectFromDevice();

signals:
    void stateChanged(bool connection);
    void message(const QString &text, int timeout = 0);

private:
    QModbusReply *m_lastRequest;
    QModbusClient *m_client;
    ConnectionParameters *m_parameters;

    void onStateChanged(QModbusDevice::State state);

    QModbusDataUnit readRequest() const;
    QModbusDataUnit writeRequest() const;
};

}
