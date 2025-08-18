#include "registersmanager.h"

#include "registersdataparser.h"

using namespace Registers;

RegistersManager::RegistersManager(QObject *parent)
    : QObject(parent)
    , m_parser(new RegistersDataParser(this))
{}

QVector<Register> *RegistersManager::registers(RegisterType type)
{
    return m_parser->registers(type);
}

void RegistersManager::saveRegisters(RegisterType type, QVector<Register> &regs)
{
    m_parser->saveRegisters(type, regs);
}
