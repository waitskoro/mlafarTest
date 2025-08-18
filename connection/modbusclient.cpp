#include "modbusclient.h"

#include <QVariant>
#include <QModbusClient>
#include <QModbusTcpClient>

#include "connectionparameters.h"

using namespace Connection;

namespace {
int TIMEOUT_M = 3000;
}

ModBusClient::ModBusClient(ConnectionParameters *parameters, QObject *parent)
    : QObject(parent)
    , m_lastRequest(nullptr)
    , m_client(new QModbusTcpClient(this))
    , m_parameters(parameters)
{
    connect(m_client, &QModbusClient::errorOccurred, this, [this](QModbusDevice::Error error) {
        if (error != QModbusDevice::NoError) {
            emit message("Error: " + m_client->errorString(), TIMEOUT_M);
            qWarning() << "Modbus error:" << error << m_client->errorString();
        }
    });

    connect(m_client, &QModbusClient::stateChanged,
            this, &ModBusClient::onStateChanged);
}

ModBusClient::~ModBusClient()
{
    if (m_client)
        m_client->disconnectDevice();
    delete m_client;
}

void ModBusClient::connectToServer()
{
    if (m_client)
        m_client->disconnectDevice();

    emit message("", 0);

    if (m_client->state() != QModbusDevice::ConnectedState) {
        const QUrl url = m_parameters->url();

        m_client->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
        m_client->setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());

        if (!m_client->connectDevice()) {
            emit message("Connection attempt failed to start", TIMEOUT_M);
            qWarning() << "Connection attempt failed to start: " << m_client->errorString();
        }

    } else m_client->disconnectDevice();

}

void ModBusClient::disconnectFromDevice()
{
    if (m_client->state() != QModbusDevice::UnconnectedState) {
        m_client->disconnectDevice();
    }
}

void ModBusClient::onStateChanged(QModbusDevice::State state)
{
    switch (state) {
        case QModbusDevice::ConnectingState: {
            qDebug() << "Подключение к серверу...";
            break;
        }
        case QModbusDevice::ConnectedState: {
            qDebug() << "Подключено к серверу";
            emit message("Подключено к серверу", TIMEOUT_M);
            break;
        }
        case QModbusDevice::ClosingState: {
            qDebug() << "Отключение от сервера";
            emit message("Отключение от сервера", TIMEOUT_M);
            break;
        }
        default: return;
    }

    emit stateChanged(state == QModbusDevice::ConnectedState);

}

QModbusDataUnit ModBusClient::readRequest() const
{
    return {};
}

QModbusDataUnit ModBusClient::writeRequest() const
{
    return {};
}
