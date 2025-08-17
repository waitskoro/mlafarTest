#pragma once

#include <QUrl>
#include <QObject>

namespace Connection {

class ConnectionParameters : public QObject
{
    Q_OBJECT

public:
    enum Protocol {
        IPv4,
        IPv6
    };

    explicit ConnectionParameters(QObject *parent = nullptr);

    QUrl url() const;
    Protocol protocol() const;
    int connectTimeout() const;
    int responseTimeout() const;
    int delayBetweenPolls() const;

    void setUrl(const QUrl& url);
    void setProtocol(Protocol protocol);
    void setConnectTimeout(int timeout);
    void setResponseTimeout(int timeout);
    void setDelayBetweenPolls(int delay);

signals:
    void parametersChanged();

private:
    int m_connectTimout = 3000;
    int m_responseTimeout = 2000;
    int m_delayBetweenPolls = 500;
    Protocol m_protocol = Protocol::IPv4;
    QUrl m_url = QUrl("http://127.0.0.1:502");
};

}
