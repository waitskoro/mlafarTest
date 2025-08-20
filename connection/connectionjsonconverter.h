#pragma once

#include <QObject>
#include <QJsonObject>

#include "connectionparameters.h"

namespace Connection {

static QJsonObject connectionParametersToJson(const ConnectionParameters *parameters)
{
    QJsonObject obj;
    QString protocolStr;

    switch(parameters->protocol()) {
        case ConnectionParameters::IPv4: protocolStr = "IPv4"; break;
        case ConnectionParameters::IPv6: protocolStr = "IPv6"; break;
        default: protocolStr = "Unknown"; break;
    }

    obj["url"] = parameters->url().toString();
    obj["protocol"] = protocolStr;
    obj["responseTimeout"] = QString::number(parameters->responseTimeout());
    obj["connectionTimeout"] = QString::number(parameters->connectTimeout());
    obj["delayBetweenPolls"] = QString::number(parameters->delayBetweenPolls());

    return obj;
}

static ConnectionParameters *connectionParametersFromJson(const QJsonObject &json)
{
    ConnectionParameters* parameters = new ConnectionParameters();

    parameters->setUrl(QUrl(json["url"].toString()));
    parameters->setProtocol(json["protocol"].toString() == "IPv6" ? ConnectionParameters::IPv6 : ConnectionParameters::IPv4);
    parameters->setConnectTimeout(json["connectionTimeout"].toString().toInt());
    parameters->setResponseTimeout(json["responseTimeout"].toString().toInt());
    parameters->setDelayBetweenPolls(json["delayBetweenPolls"].toString().toInt());

    return parameters;
}

static ConnectionParameters::Protocol protocolFromString(const QString &protocolStr)
{
    if (protocolStr.compare("IPv4", Qt::CaseInsensitive) == 0)
        return ConnectionParameters::IPv4;
    else if (protocolStr.compare("IPv6", Qt::CaseInsensitive) == 0)
        return ConnectionParameters::IPv6;

    return ConnectionParameters::IPv4;
}

}

