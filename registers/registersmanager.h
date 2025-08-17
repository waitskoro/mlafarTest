#pragma once

#include <QObject>

namespace Registers {

class RegistersDataParser;

class RegistersManager : public QObject
{
    Q_OBJECT
public:
    explicit RegistersManager(QObject *parent = nullptr);

private:
    RegistersDataParser *m_parser;
};

}
