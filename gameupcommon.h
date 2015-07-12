#ifndef GAMEUPCOMMON_H
#define GAMEUPCOMMON_H

#include <QObject>
#include <QNetworkAccessManager>
#include<QNetworkReply>
#include <QEventLoop>

class GameOnRequest;
class GameUpCommon: public QObject
{
public:
    GameUpCommon(QObject *parent = 0);
    QString apiKey() const;
    QNetworkAccessManager *manager();
    void setApiKey(const QString &apiKey);

signals:


public slots:
    bool ping();
    QString loginAnonymous(const QString &username);

private slots:
    void reqfinished(int id, QNetworkReply::NetworkError error, QByteArray data);


private:
    QString m_apiKey;
    QString m_token;
    QNetworkAccessManager m_manager;
    GameOnRequest *gonRequest;
    QEventLoop loop;
    QNetworkReply::NetworkError lasterror;
    QByteArray lastData;
};

#endif // GAMEUPCOMMON_H
