#include "connectionmanager.h"

#include "connectionparameters.h"

using namespace Connection;

ConnectionManager::ConnectionManager(ConnectionParameters *parameters,
                                     QObject *parent)
    : QObject(parent)
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
