#pragma once

#include <QObject>

#include "iregistersparameters.h"

namespace Registers {

enum RegisterType {
    Pco,
    Dos,
    Common
};

class RegistersDataParser : public QObject
{
    Q_OBJECT
public:
    explicit RegistersDataParser(QObject *parent = nullptr);

    const QVector<Register> *registers(RegisterType);

private:
    void createFile();
    void writeInFile();
    void readFromJson();

    QJsonObject fileObj();

    QMap<RegisterType, QVector<Register>> *m_registers;
};

}
