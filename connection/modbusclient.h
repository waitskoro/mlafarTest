#pragma once

#include <QObject>
#include <QModbusDevice>
#include <QModbusDataUnit>

#include "registers/iregistersparameters.h"

class QModbusReply;
class QModbusTcpClient;

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
    void setPcoUnitId(int index);

    void readRegisters(const QVector<Registers::Register> &registers,
                       Registers::RegisterType,
                       int unitId);
signals:
    void stateChanged(bool connection);
    void registersRead(Registers::RegisterType, QVector<Registers::Register> &registers);

private:
    int m_index = 1;

    QModbusTcpClient *m_client;
    ConnectionParameters *m_parameters;

    bool isActiveConnection();
    quint16 convertAddress(const QString &address) const;

    void onReadFinished();
    void processReadResult(QModbusReply *reply);

    void onStateChanged(QModbusDevice::State state);

    Registers::RegisterType m_currentType;
    QVector<Registers::Register> m_currentRegisters;

    quint16 m_success = 0;
    quint16 m_failure = 0;
    quint16 m_requests = 0;


};

}
