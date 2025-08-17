#include "registersmanager.h"

#include "registersdataparser.h"

using namespace Registers;

RegistersManager::RegistersManager(QObject *parent)
    : QObject(parent)
    , m_parser(new RegistersDataParser(this))
{

}
