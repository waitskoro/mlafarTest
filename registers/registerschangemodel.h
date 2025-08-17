#pragma once

#include <QObject>

namespace Registers {

class RegistersChangeModel : public QObject
{
    Q_OBJECT
public:
    explicit RegistersChangeModel(QObject *parent = nullptr);

signals:

};

}
