#pragma once

#include <QObject>

#include "iregistersparameters.h"

class JsonDataParser;

namespace Registers {

class RegistersDataModel;
class RegistersChangeModel;

class RegistersManager : public QObject
{
    Q_OBJECT
public:
    explicit RegistersManager(JsonDataParser *, QObject *parent = nullptr);

    void updateAllRegisters();

    QVector<Register> *registers(RegisterType);
    void updateRegistersValues(RegisterType, QVector<Register> *);
    void setRegisters(const QMap<RegisterType, QVector<Register>> &registers);

    RegistersDataModel *modelData(RegisterType);
    QMap<RegisterType, RegistersChangeModel*> modelChange();

signals:
    void saveRegisters(RegisterType, QVector<Register> *);

private:
    JsonDataParser *m_parser;
    QMap<RegisterType, RegistersDataModel*> m_modelsData;
    QMap<RegisterType, RegistersChangeModel*> m_modelsChange;

};

}
