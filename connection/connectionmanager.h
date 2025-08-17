#pragma once

#include <QObject>

namespace Connection {

class ConnectionParameters;

class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionManager(ConnectionParameters *parameters, QObject *parent = nullptr);

signals:

private:
    ConnectionParameters *m_parameters;

    void onParametersChanged();

};

}
