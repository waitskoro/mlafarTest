#pragma once

#include <QObject>
#include <QString>
#include <QVector>

#include "registers/registersjsonconverter.h"
#include "connection/connectionjsonconverter.h"

namespace Registers {
    enum RegisterType;
    class Register;
}

namespace Connection {
    class ConnectionParameters;
}

class JsonDataParser : public QObject
{
    Q_OBJECT
public:
    explicit JsonDataParser(QObject *parent = nullptr);

    Connection::ConnectionParameters *readConnectionParameters();
    bool writeConnectionParameters(Connection::ConnectionParameters *parameters);

    QVector<Registers::Register> *readRegisters(Registers::RegisterType type);
    bool writeRegisters(Registers::RegisterType type, const QVector<Registers::Register>& registers);

    static QString registerTypeToString(Registers::RegisterType type);
    static Registers::RegisterType stringToRegisterType(const QString &str);
};

