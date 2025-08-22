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
    , m_lastRequest(nullptr)
    , m_parameters(parameters)
    , m_connectionError(false)
    , m_lastSuccessfulUnitId(1)
{
    connect(m_client, &QModbusClient::errorOccurred, this, [this](QModbusDevice::Error error) {
        if (error != QModbusDevice::NoError) {
            qWarning() << "Modbus error:" << error << m_client->errorString();

            // Отмечаем ошибку соединения только для критических ошибок
            if (error == QModbusDevice::ConnectionError ||
                error == QModbusDevice::ReadError ||
                error == QModbusDevice::WriteError) {
                m_connectionError = true;
                qDebug() << "Критическая ошибка соединения, пытаемся переподключиться...";
                QTimer::singleShot(1000, this, &ModBusClient::reconnect);
            }
            // Таймауты не считаем критическими ошибками соединения
            else if (error == QModbusDevice::TimeoutError) {
                qDebug() << "Таймаут запроса (возможно неверный Unit ID)";
            }
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

    m_connectionError = false;

    if (m_client->state() != QModbusDevice::ConnectedState) {
        const QUrl url = m_parameters->url();

        m_client->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
        m_client->setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());
        m_client->setTimeout(TIMEOUT_M);

        if (!m_client->connectDevice()) {
            qWarning() << "Connection attempt failed to start: " << m_client->errorString();
            m_connectionError = true;
        }

    } else {
        m_client->disconnectDevice();
    }
}

void ModBusClient::reconnect()
{
    if (m_connectionError) {
        qDebug() << "Переподключение к устройству...";
        disconnectFromDevice();
        QTimer::singleShot(500, this, [this]() {
            connectToServer();
        });
    }
}

void ModBusClient::disconnectFromDevice()
{
    if (m_client->state() != QModbusDevice::UnconnectedState) {
        m_client->disconnectDevice();
    }
    m_connectionError = false;
}

void ModBusClient::setPcoUnitId(int index)
{
    int previousIndex = m_index;
    m_index = index;
    qDebug() << "Unit ID установлен на:" << m_index;

    // Если переключаемся с нерабочего unit ID на рабочий, принудительно переподключаемся
    if (previousIndex != m_index && m_index == m_lastSuccessfulUnitId) {
        qDebug() << "Переключение на рабочий Unit ID, принудительное переподключение...";
        m_connectionError = true;
        reconnect();
    }

    // При смене unit ID сбрасываем возможные временные ошибки
    if (m_client->state() == QModbusDevice::ConnectedState) {
        m_connectionError = false;
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
            m_connectionError = false;
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

QModbusDataUnit ModBusClient::readRequest(int startAddress, int numberOfEntries) const
{
    return QModbusDataUnit(QModbusDataUnit::RegisterType::HoldingRegisters,
                          startAddress,
                          numberOfEntries);
}

void ModBusClient::readRegisters(Registers::RegisterType type, QVector<Registers::Register> *registers)
{
    if (!m_client) {
        qDebug() << "Modbus клиент не инициализирован";
        return;
    }

    // Если есть ошибка соединения, пытаемся переподключиться
    if (m_connectionError && m_client->state() == QModbusDevice::UnconnectedState) {
        qDebug() << "Попытка переподключения из-за ошибки соединения";
        connectToServer();
        return;
    }

    if (m_client->state() != QModbusDevice::ConnectedState) {
        qDebug() << "Устройство не подключено, состояние:" << m_client->state();
        return;
    }

    // Определяем server address в зависимости от типа устройства
    int serverAddress = 0; // По умолчанию для DOS
    if (type == Registers::RegisterType::Pco) {
        serverAddress = m_index;
        qDebug() << "Чтение регистров PCO с unit ID:" << serverAddress;
    } else {
        qDebug() << "Чтение регистров DOS с unit ID:" << serverAddress;
    }

    if (!registers || registers->isEmpty()) {
        qDebug() << "Пустой массив регистров для типа:" << type;
        emit registersRead(type, *registers);
        return;
    }

    // Создаем shared pointer для безопасного управления памятью
    auto sharedRegisters = QSharedPointer<QVector<Registers::Register>>::create(*registers);
    auto pendingReplies = QSharedPointer<int>::create(registers->size());

    // Для каждого регистра отправляем отдельный запрос
    for (int i = 0; i < registers->size(); ++i) {
        bool ok;
        int address = registers->at(i).address.toInt(&ok, 0);
        if (!ok) {
            qDebug() << QString("Неверный адрес регистра: %1").arg(registers->at(i).address);
            (*pendingReplies)--;
            checkAllRepliesComplete(type, sharedRegisters, pendingReplies);
            continue;
        }

        QModbusDataUnit readUnit = readRequest(address, 1);

        if (auto *reply = m_client->sendReadRequest(readUnit, serverAddress)) {
            connect(reply, &QModbusReply::finished, this,
                   [this, type, reply, i, sharedRegisters, pendingReplies]() {
                handleSingleRegisterReply(type, reply, (*sharedRegisters)[i]);
                reply->deleteLater();

                // Уменьшаем счетчик ожидаемых ответов
                (*pendingReplies)--;
                checkAllRepliesComplete(type, sharedRegisters, pendingReplies);
            });
        } else {
            qDebug() << "Ошибка отправки запроса для адреса" << address << ":" << m_client->errorString();
            (*pendingReplies)--;
            checkAllRepliesComplete(type, sharedRegisters, pendingReplies);
        }

        // Небольшая задержка между запросами
        QThread::msleep(5);
    }

    // Проверяем, не завершились ли все запросы сразу
    checkAllRepliesComplete(type, sharedRegisters, pendingReplies);
}

void ModBusClient::checkAllRepliesComplete(Registers::RegisterType type,
                                          QSharedPointer<QVector<Registers::Register>> registers,
                                          QSharedPointer<int> pendingReplies)
{
    if (*pendingReplies <= 0) {
        qDebug() << "Все запросы завершены, отправляем сигнал registersRead";
        // Отправляем копию регистров, а не указатель
        emit registersRead(type, *registers);
    }
}

void ModBusClient::handleSingleRegisterReply(Registers::RegisterType type, QModbusReply *reply, Registers::Register &reg)
{
    if (reply->error() == QModbusDevice::NoError) {
        const auto unit = reply->result();
        if (unit.valueCount() > 0) {
            quint16 value = unit.value(0);
            reg.value = QString::number(value);
            m_connectionError = false; // Сбрасываем ошибку при успешном ответе

            // Запоминаем последний рабочий unit ID
            if (type == Registers::RegisterType::Pco) {
                m_lastSuccessfulUnitId = m_index;
            }

            qDebug() << "Регистр" << reg.address << "прочитан успешно, значение:" << value;
        }
    } else {
        qDebug() << "Ошибка ответа для адреса" << reg.address << ": " + reply->errorString();

        if (reply->error() == QModbusDevice::TimeoutError) {
            qDebug() << "Таймаут запроса для адреса" << reg.address << "(Unit ID:" << m_index << ")";
        } else {
            m_connectionError = true;
        }
    }
}
