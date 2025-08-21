#include "registersmanager.h"
#include "registersdatamodel.h"
#include "registerschangemodel.h"

#include "jsondataparser.h"

using namespace Registers;

RegistersManager::RegistersManager(JsonDataParser *parser, QObject *parent)
    : QObject(parent)
    , m_parser(parser)
{
    for (const auto type : typesChange)
        m_modelsChange[type] = new RegistersChangeModel(this);

    for (const auto type : typesData)
        m_modelsData[type] = new RegistersDataModel(this);

    updateAllRegisters();

    connect(m_parser, &JsonDataParser::registersChanged, [this](){
        updateAllRegisters();
    });
}

QVector<Register>* RegistersManager::registers(RegisterType type)
{
    return m_modelsData[type]->registers();
}

void RegistersManager::setRegisters(const QMap<RegisterType, QVector<Register>> &registers)
{
    QVector<Register> commonRegs = registers.value(Common);
    QVector<Register> dosRegs = registers.value(Dos);
    QVector<Register> pcoRegs = registers.value(Pco);

    QVector<Register> dosAll = commonRegs + dosRegs;
    QVector<Register> pcoAll = commonRegs + pcoRegs;

    if (m_modelsData.contains(Dos)) {
        m_modelsData[Dos]->setRegisters(dosAll);
    }

    if (m_modelsData.contains(Pco)) {
        m_modelsData[Pco]->setRegisters(pcoAll);
    }

    m_modelsChange[Dos]->setRegisters(registers.value(Dos));
    m_modelsChange[Pco]->setRegisters(registers.value(Pco));
    m_modelsChange[Common]->setRegisters(registers.value(Common));
}

void RegistersManager::updateRegistersValues(RegisterType type, QVector<Register> *registers)
{
    if (m_modelsData.contains(type) && registers) {
         m_modelsData[type]->setRegisters(*registers);
     }
}

RegistersDataModel* RegistersManager::modelData(RegisterType type)
{
    return m_modelsData[type];
}

QMap<RegisterType, RegistersChangeModel*> RegistersManager::modelChange()
{
    return m_modelsChange;
}

void RegistersManager::updateAllRegisters()
{
    setRegisters(m_parser->readRegisters());
}
