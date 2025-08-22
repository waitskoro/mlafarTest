#include "modbusclient.h"

#include <QDebug>
#include <QTimer>
#include <QThread>
#include <QVariant>
#include <QModbusClient>
#include <QModbusTcpClient>
#include <limits.h>
#include <memory>

#include "connectionparameters.h"

using namespace Connection;

namespace {
int TIMEOUT_M = 3000;
}

ModBusClient::ModBusClient(ConnectionParameters *parameters, QObject *parent)
    : QObject(parent)
    , m_client(new QModbusTcpClient(this))
    , m_parameters(parameters)
{
    connect(m_client, &QModbusClient::errorOccurred, [this]() {
        qDebug() << m_client->errorString();
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
        m_client->setTimeout(TIMEOUT_M);

        if (!m_client->connectDevice()) {
            qWarning() << "Connection attempt failed to start: " << m_client->errorString();
        }

    } else {
        m_client->disconnectDevice();
    }
}

void ModBusClient::disconnectFromDevice()
{
    if (m_client->state() != QModbusDevice::UnconnectedState) {
        m_client->disconnectDevice();
    }
}

void ModBusClient::setPcoUnitId(int index)
{
    m_index = index;
    qDebug() << "Unit ID установлен на:" << m_index;
}

void ModBusClient::onReadFinished()
{
    auto *reply = qobject_cast<QModbusReply *>(sender());
    if (!reply) {
        return;
    }

    processReadResult(reply);
    reply->deleteLater();
}

void ModBusClient::processReadResult(QModbusReply *reply)
{
    if (reply->error() != QModbusDevice::NoError) {
            qDebug() << "Modbus error: " << reply->errorString();
            return;
        }

        const QModbusDataUnit unit = reply->result();
        const QVector<quint16> values = unit.values();

        for (auto &reg : m_currentRegisters) {
            quint16 addr = convertAddress(reg.address);
            int index = addr - unit.startAddress();

            if (index < 0 || index >= values.size()) {
                reg.value = "N/A";
                continue;
            }

            if (reg.type == "uint16") {
                reg.value = QString::number(values[index]);
            } else if (reg.type == "int16") {
                reg.value = QString::number(static_cast<int16_t>(values[index]));
            } else if (reg.type == "uint32") {
                if (index + 1 < values.size()) {
                    quint32 val = (static_cast<quint32>(values[index + 1]) << 16) | values[index];
                    reg.value = QString::number(val);
                }
            } else if (reg.type == "int32") {
                if (index + 1 < values.size()) {
                    int32_t val = (static_cast<int32_t>(values[index + 1]) << 16) | values[index];
                    reg.value = QString::number(val);
                }
            } else if (reg.type == "uint64") {
                if (index + 3 < values.size()) {
                    quint64 val = (static_cast<quint64>(values[index + 3]) << 48) |
                                  (static_cast<quint64>(values[index + 2]) << 32) |
                                  (static_cast<quint64>(values[index + 1]) << 16) |
                                  values[index];
                    reg.value = QString::number(val);
                }
            } else if (reg.type.startsWith("char[")) {
                QString str;
                int strLength = reg.type.mid(reg.type.indexOf('[') + 1).remove(']').toInt();
                for (int i = 0; i < (strLength + 1) / 2 && index + i < values.size(); i++) {
                    quint16 regValue = values[index + i];
                    str.append(QChar(regValue & 0xFF)); // Младший байт
                    str.append(QChar(regValue >> 8));   // Старший байт
                }
                reg.value = str.trimmed();
            }
        }

        emit registersRead(m_currentType, m_currentRegisters);
}

bool ModBusClient::isActiveConnection()
{
    return m_client && m_client->state() == QModbusDevice::ConnectedState;
}

void ModBusClient::readRegisters(const QVector<Registers::Register> &registers, Registers::RegisterType type, int unitId)
{
    if (!m_client || m_client->state() != QModbusDevice::ConnectedState) {
        qDebug() << ("Modbus client not connected");
        return;
    }

    if (registers.isEmpty()) {
        qDebug() << "No registers to read";
        return;
    }

    m_currentRegisters = registers;
    m_currentType = type;

    quint16 startAddress = convertAddress(registers.first().address);
    quint16 maxEndAddress = startAddress;

    for (const auto &reg : registers) {
        quint16 currentAddr = convertAddress(reg.address);
        quint16 endAddr = currentAddr;

        if (reg.type.contains("uint32") || reg.type.contains("int32")) {
            endAddr = currentAddr + 1;
        } else if (reg.type.contains("uint64")) {
            endAddr = currentAddr + 3;
        } else if (reg.type.contains("char[")) {
            int strLength = reg.type.mid(reg.type.indexOf('[') + 1).remove(']').toInt();
            endAddr = currentAddr + static_cast<quint16>((strLength + 1) / 2);
        }

        if (endAddr > maxEndAddress) {
            maxEndAddress = endAddr;
        }
    }

    quint16 registerCount = maxEndAddress - startAddress + 1;

    if (registerCount > 125) {
        qDebug() << "Too many registers to read at once";
        return;
    }

    QModbusDataUnit request(QModbusDataUnit::HoldingRegisters, startAddress, registerCount);

    if (auto *reply = m_client->sendReadRequest(request, unitId)) {
        if (!reply->isFinished()) {
            connect(reply, &QModbusReply::finished, this, &ModBusClient::onReadFinished);
        } else {
            reply->deleteLater();
            qDebug() << "Reply finished immediately";
        }
    } else {
        qDebug() << "Failed to send read request:" << m_client->errorString();
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
        case QModbusDevice::UnconnectedState: {
            qDebug() << "Отключено от сервера";
            break;
        }
    }

    emit stateChanged(state == QModbusDevice::ConnectedState);
}

quint16 ModBusClient::convertAddress(const QString &address) const
{
    bool ok;
    return address.toUShort(&ok, 16);
}
