#include "modbusclient.h"

#include <QVariant>
#include <QModbusClient>
#include <QModbusTcpClient>

#include "connectionparameters.h"

using namespace Connection;

ModBusClient::ModBusClient(ConnectionParameters *parameters, QObject *parent)
    : QObject(parent)
    , m_lastRequest(nullptr)
    , m_modbusDevice(new QModbusTcpClient(this))
    , m_parameters(parameters)
{
    connect(m_modbusDevice,
            &QModbusClient::errorOccurred,
            [this](QModbusDevice::Error) {
                emit message(m_modbusDevice->errorString(), 5000);
            });

    connect(m_modbusDevice, &QModbusClient::stateChanged,
            this, &ModBusClient::onStateChanged);
}

ModBusClient::~ModBusClient()
{
    if (m_modbusDevice)
        m_modbusDevice->disconnectDevice();
    delete m_modbusDevice;
}

void ModBusClient::connectToServer()
{
    if (m_modbusDevice)
        m_modbusDevice->disconnectDevice();

    emit message("", 0);

    if (m_modbusDevice->state() != QModbusDevice::ConnectedState) {
        QUrl url = m_parameters->url();
        m_modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter,
                                               QVariant(url.port()));
        m_modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter,
                                               QVariant(url.host()));
        m_modbusDevice->setTimeout(m_parameters->responseTimeout());
        m_modbusDevice->setNumberOfRetries(3);

        if (!m_modbusDevice->connectDevice()) {
            emit message("Connect failed: " + m_modbusDevice->errorString(), 5000);
        }
    } else {
        m_modbusDevice->disconnectDevice();
    }
}

void ModBusClient::onStateChanged(QModbusDevice::State state)
{
    bool connected = (state == QModbusDevice::ConnectedState);
    emit connectionState(connected);
}

QModbusDataUnit ModBusClient::readRequest() const
{
    return {};
}

QModbusDataUnit ModBusClient::writeRequest() const
{
    return {};
}
