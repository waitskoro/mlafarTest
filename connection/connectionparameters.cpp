#include "connectionparameters.h"

#include <QDebug>

using namespace Connection;

ConnectionParameters::ConnectionParameters(QObject *parent)
    : QObject(parent)
{}

QUrl ConnectionParameters::url() const
{
    return m_url;
}

ConnectionParameters::Protocol ConnectionParameters::protocol() const
{
    return m_protocol;
}

int ConnectionParameters::connectTimeout() const
{
    return m_connectTimout;
}

int ConnectionParameters::responseTimeout() const
{
    return m_responseTimeout;
}

int ConnectionParameters::delayBetweenPolls() const
{
    return m_delayBetweenPolls;
}

void ConnectionParameters::setUrl(const QUrl& url) {
    if (m_url != url) {
        m_url = url;
        emit parametersChanged();
    }
}

void ConnectionParameters::setProtocol(Protocol protocol)
{
    if (m_protocol != protocol) {
        m_protocol = protocol;
        emit parametersChanged();
    }
}

void ConnectionParameters::setConnectTimeout(int timeout) {
    if (m_connectTimout != timeout) {
        m_connectTimout = timeout;
        emit parametersChanged();
    }
}

void ConnectionParameters::setResponseTimeout(int timeout) {
    if (m_responseTimeout != timeout) {
        m_responseTimeout = timeout;
        emit parametersChanged();
    }
}

void ConnectionParameters::setDelayBetweenPolls(int delay) {
    if (m_delayBetweenPolls != delay) {
        m_delayBetweenPolls = delay;
        emit parametersChanged();
    }
}
