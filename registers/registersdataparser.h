#pragma once

#include <QObject>

#include "iregistersparameters.h"

namespace Registers {

class RegistersDataParser : public QObject
{
    Q_OBJECT
public:
    explicit RegistersDataParser(QObject *parent = nullptr);

    QVector<Register> *registers(RegisterType);
    void saveRegisters(RegisterType, QVector<Register> &);

signals:
    void registers(QMap<RegisterType, QVector<Register>> *);

private:
    void createFile();
    void writeInFile();
    void readFromJson();

    QJsonObject fileObj();
    QString registerTypeToString(RegisterType type);

    QMap<RegisterType, QVector<Register>> *m_registers;

    void saveAllRegistersToFile();
    bool writeJsonToFile(const QJsonObject &json);
    RegisterType stringToRegisterType(const QString &str);
};

}
