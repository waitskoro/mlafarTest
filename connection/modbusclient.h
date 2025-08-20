#pragma once

#include <QObject>
#include <QModbusDevice>
#include <QModbusDataUnit>

#include "registers/iregistersparameters.h"

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
    void readRegisters(Registers::RegisterType, QVector<Registers::Register> *);

signals:
    void stateChanged(bool connection);
    void registersRead(Registers::RegisterType, QVector<Registers::Register> *);

private:
    QModbusClient *m_client;
    QModbusReply *m_lastRequest;
    ConnectionParameters *m_parameters;

    void onStateChanged(QModbusDevice::State state);
    void handleReadReply(Registers::RegisterType, QModbusReply *reply, QVector<Registers::Register> *registers);

    QModbusDataUnit writeRequest(Registers::Register) const;
    QModbusDataUnit readRequest(int startAddress, int numberOfEntries) const;

};

}
