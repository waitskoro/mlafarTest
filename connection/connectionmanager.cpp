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
}

void ConnectionManager::onParametersChanged()
{

}
