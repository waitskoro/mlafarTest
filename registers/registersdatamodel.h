#pragma once

#include <QObject>

namespace Registers {

class RegistersDataModel : public QObject
{
    Q_OBJECT
public:
    explicit RegistersDataModel(QObject *parent = nullptr);

signals:

};

}

