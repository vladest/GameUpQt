#ifndef GAMEUPCOMMON_H
#define GAMEUPCOMMON_H

#include <QObject>
#include <QNetworkAccessManager>
#include<QNetworkReply>
#include <QEventLoop>

class GameOnRequest;
class Gamer;
class Leaderboard;
class GameUpCommon: public QObject
{
public:
    GameUpCommon(QObject *parent = 0);
    QString apiKey() const;
    QNetworkAccessManager *manager();
    void setApiKey(const QString &apiKey);

    /**
     * get gamer info
     * Gamer's token have to be already registered
    */

    void getGamer(const QString &username, Gamer *gamer);
    /**
     * Fill up username token table
     * In order to make GameUp API works its up to application to fill it in advance
     * The library deals with gamer tokens, while user application deal with usernames
     */
    void addUserToken(const QString &username, const QString &token);

    void getLeaderboard(const QString &username, const QString &lbid, Leaderboard *leaderboard);

signals:


public slots:
    bool ping();
    QString loginAnonymous(const QString &username);

private slots:
    void reqfinished(int id, QNetworkReply::NetworkError error, QByteArray data);


private:
    QString m_apiKey;
    QNetworkAccessManager m_manager;
    GameOnRequest *gonRequest;
    QEventLoop loop;
    QNetworkReply::NetworkError lasterror;
    QByteArray lastData;
    QMap<QString, QString> m_usersTokens;
};

#endif // GAMEUPCOMMON_H
