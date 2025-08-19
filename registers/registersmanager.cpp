#include "registersmanager.h"
#include "registersdatamodel.h"
#include "registersdataparser.h"
#include "registerschangemodel.h"

using namespace Registers;

RegistersManager::RegistersManager(QObject *parent)
    : QObject(parent)
    , m_parser(new RegistersDataParser(this))
{
    for (const auto type : types) {
        m_modelsData[type] = new RegistersDataModel(this);
        m_modelsChange[type] = new RegistersChangeModel(this);

        m_modelsData[type]->setRegisters(*m_parser->registers(type));
        m_modelsChange[type]->setRegisters(*m_parser->registers(type));
    }
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

void RegistersManager::saveRegisters(RegisterType type)
{
    m_parser->saveRegisters(type, *m_parser->registers(type));
}

QVector<Register>* RegistersManager::changeRegisters(RegisterType type)
{
    return m_parser->registers(type);
}
