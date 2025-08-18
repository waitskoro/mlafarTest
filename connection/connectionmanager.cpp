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
    connect(m_parameters,
            &ConnectionParameters::parametersChanged,
            this,
            &ConnectionManager::onParametersChanged);

    connect(m_client,
            &ModBusClient::stateChanged,
            this,
            &ConnectionManager::stateChanged);
}

void ConnectionManager::connectToServer()
{
    m_client->connectToServer();
}

void ConnectionManager::disconnectFromDevice()
{
    m_client->disconnectFromDevice();
}

void ConnectionManager::onParametersChanged()
{

}
