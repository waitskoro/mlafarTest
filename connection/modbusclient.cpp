#include "modbusclient.h"

#include <QDebug>
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
    , m_client(new QModbusTcpClient(this))
    , m_lastRequest(nullptr)
    , m_parameters(parameters)
{
    connect(m_client, &QModbusClient::errorOccurred, this, [this](QModbusDevice::Error error) {
        if (error != QModbusDevice::NoError) {
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

    if (m_client->state() != QModbusDevice::ConnectedState) {
        const QUrl url = m_parameters->url();

        m_client->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
        m_client->setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());

        if (!m_client->connectDevice()) {
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
            break;
        }
        case QModbusDevice::ClosingState: {
            qDebug() << "Отключение от сервера";
            break;
        }
        default: return;
    }

    emit stateChanged(state == QModbusDevice::ConnectedState);
}

QModbusDataUnit ModBusClient::readRequest(int startAddress, int numberOfEntries) const
{
    return QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters,
                          startAddress,
                          numberOfEntries);
}

QModbusDataUnit ModBusClient::writeRequest(Registers::Register reg) const
{
    return QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters,
                          reg.address.toInt(nullptr, 0),
                          1);
}

void ModBusClient::readRegisters(Registers::RegisterType type, QVector<Registers::Register> *registers)
{
    if (!m_client || !(m_client->state() == QModbusDevice::ConnectedState)) {
        qDebug() << "Устройство не подключено";
        return;
    }

    QVector<QModbusDataUnit> readUnits;
    for (const auto &reg : *registers) {
        bool ok;
        int address = reg.address.toInt(&ok, 0);
        if (ok) {
            readUnits.append(readRequest(address, 1));
        } else {
            qDebug() << QString("Неверный адрес регистра: %1").arg(reg.address);
        }
    }

    for (const auto &unit : readUnits) {
        if (auto *reply = m_client->sendReadRequest(unit, 1)) {
            connect(reply, &QModbusReply::finished, this, [this, type, reply, registers]() {
                handleReadReply(type, reply, registers);
                reply->deleteLater();
            });
        } else {
            qDebug() << "Ошибка чтения:" << m_client->errorString();
        }
    }
}

void ModBusClient::handleReadReply(Registers::RegisterType type, QModbusReply *reply, QVector<Registers::Register> *registers)
{
    if (reply->error() == QModbusDevice::NoError) {
        const auto unit = reply->result();
        int address = unit.startAddress();
        quint16 value = unit.value(0);

        for (auto &reg : *registers) {
            if (reg.address.toInt(nullptr, 0) == address) {
                reg.value = QString::number(value);
                break;
            }
        }
        qDebug() << "Регистры прочтены";
        emit registersRead(type, registers);
    } else {
        qDebug() << "Ошибка ответа: " + reply->errorString();
    }
}
