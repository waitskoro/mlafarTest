#pragma once

#include <QObject>

#include "iregistersparameters.h"

namespace Registers {

class RegistersDataModel;
class RegistersDataParser;
class RegistersChangeModel;

class RegistersManager : public QObject
{
    Q_OBJECT
public:
    explicit RegistersManager(QObject *parent = nullptr);

    RegistersDataModel *modelData(RegisterType);
    RegistersChangeModel *modelChange(RegisterType);

    void saveRegisters(RegisterType type);
    QVector<Register> *changeRegisters(RegisterType);

private:
    RegistersDataParser *m_parser;

    QMap<RegisterType, RegistersDataModel*> m_modelsData;
    QMap<RegisterType, RegistersChangeModel*> m_modelsChange;
};

}
