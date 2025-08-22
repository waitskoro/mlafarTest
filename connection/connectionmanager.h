#pragma once

#include <QObject>
#include "registers/iregistersparameters.h"

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
    void setPcoUnitId(int index);
    void readRegisters(Registers::RegisterType, QVector<Registers::Register> *, int);

signals:
    void stateChanged(bool connection);
    void registersRead(Registers::RegisterType, QVector<Registers::Register>);

private:
    ModBusClient *m_client;
    ConnectionParameters *m_parameters;

};

}
