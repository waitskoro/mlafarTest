#include "registersmanager.h"
#include "registersdatamodel.h"
#include "registerschangemodel.h"

using namespace Registers;

RegistersManager::RegistersManager(QObject *parent)
    : QObject(parent)
{
    for (const auto type : types) {
        m_modelsData[type] = new RegistersDataModel(this);
        m_modelsChange[type] = new RegistersChangeModel(this);
    }
}

QVector<Register>* RegistersManager::registers(RegisterType type)
{
    return m_modelsData[type]->registers();
}

void RegistersManager::setRegisters(RegisterType type, QVector<Register> *registers)
{
    m_modelsData[type]->setRegisters(registers);
    m_modelsChange[type]->setRegisters(*registers);
}

RegistersDataModel* RegistersManager::modelData(RegisterType type)
{
    Q_ASSERT(m_modelsData.contains(type));
    return m_modelsData[type];
}

RegistersChangeModel* RegistersManager::modelChange(RegisterType type)
{
    Q_ASSERT(m_modelsChange.contains(type));
    return m_modelsChange[type];
}
