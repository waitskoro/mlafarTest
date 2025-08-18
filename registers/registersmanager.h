#pragma once

#include <QObject>

#include "iregistersparameters.h"

namespace Registers {

class RegistersDataParser;

class RegistersManager : public QObject
{
    Q_OBJECT
public:
    explicit RegistersManager(QObject *parent = nullptr);

    QVector<Register> *registers(RegisterType);
    void saveRegisters(RegisterType type, QVector<Register> &regs);

private:
    RegistersDataParser *m_parser;
};

}
