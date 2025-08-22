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
    void setPcoUnitId(int index);
    void readRegisters(Registers::RegisterType, QVector<Registers::Register> *);

signals:
    void connectionErrorOccurred();
    void stateChanged(bool connection);
    void registersRead(Registers::RegisterType, QVector<Registers::Register> registers);

private:
    int m_index = 1;
    QModbusClient *m_client;
    QModbusReply *m_lastRequest;
    ConnectionParameters *m_parameters;

    void onStateChanged(QModbusDevice::State state);
    void handleReadReply(Registers::RegisterType type, QModbusReply *reply,
                         const QMap<int, Registers::Register*> &addressToRegister,
                         int startAddress);
    void handleSingleRegisterReply(Registers::RegisterType type,
                                   QModbusReply *reply,
                                   Registers::Register &reg);

    void checkAllRepliesComplete(Registers::RegisterType type,
                                QSharedPointer<QVector<Registers::Register>> registers,
                                QSharedPointer<int> pendingReplies);

    QModbusDataUnit writeRequest(Registers::Register) const;
    QModbusDataUnit readRequest(int startAddress, int numberOfEntries) const;

    int m_lastSuccessfulUnitId;

    bool m_connectionError;
    void reconnect();


};

}
