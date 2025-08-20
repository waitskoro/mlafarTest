#pragma once

#include <QObject>

#include "iregistersparameters.h"

namespace Registers {

class RegistersDataModel;
class RegistersChangeModel;

class RegistersManager : public QObject
{
    Q_OBJECT
public:
    explicit RegistersManager(QObject *parent = nullptr);

    QVector<Register> *registers(RegisterType);
    void setRegisters(RegisterType, QVector<Register> *);

    RegistersDataModel *modelData(RegisterType);
    RegistersChangeModel *modelChange(RegisterType);

signals:
    void saveRegisters(RegisterType, QVector<Register> *);

private:
    QMap<RegisterType, RegistersDataModel*> m_modelsData;
    QMap<RegisterType, RegistersChangeModel*> m_modelsChange;
};

}
