#include "connectionmanager.h"

#include "modbusclient.h"
#include "connectionparameters.h"

using namespace Connection;

ConnectionManager::ConnectionManager(ConnectionParameters *parameters,
                                     QObject *parent)
    : QObject(parent)
    , m_client(new ModBusClient(parameters, this))
    , m_parameters(parameters)
{
    connect(m_client,
            &ModBusClient::stateChanged,
            this,
            &ConnectionManager::stateChanged);

    connect(m_client,
            &ModBusClient::registersRead,
            this,
            &ConnectionManager::registersRead);    
}

void ConnectionManager::connectToServer()
{
    m_client->connectToServer();
}

void ConnectionManager::disconnectFromDevice()
{
    m_client->disconnectFromDevice();
}

void ConnectionManager::setPcoUnitId(int index)
{
    m_client->setPcoUnitId(index);
}

void ConnectionManager::readRegisters(Registers::RegisterType type, QVector<Registers::Register> *registers, int unitId)
{
    m_client->readRegisters(*registers, type, unitId);
}
