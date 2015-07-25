#ifndef GAMEUPCOMMON_H
#define GAMEUPCOMMON_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include "QtWebView/QQuickWebView"
#include "gamer.h"
#include "leaderboard.h"

class GameUpQt;
class GameOnRequest;
class Gamer;
class Leaderboard;
class GameUpQtPrivate: public QObject
{
public:
    GameUpQtPrivate(QObject *parent = 0);
    QString apiKey() const;
    QNetworkAccessManager *manager();
    void setApiKey(const QString &apiKey);

    /**
     * get gamer info
     * Gamer's token have to be already registered
    */

    void updateGamerData(const QString &username);
    /**
     * Fill up username token table
     * In order to make GameUp API works its up to application to fill it in advance
     * The library deals with gamer tokens, while user application deal with usernames
     */
    void addUserToken(const QString &username, const QString &token);
    void updateLeaderboard(const QString &username);
    void updateGamerRank(const QString &username);
    void setLeaderboardScore(const QString &username, int score);
    void updateGamerAchievments(const QString &username);
    bool ping();
    QString loginAnonymous(const QString &username);
    QString loginGameup(const QString &username);

    QQuickWebView* webView() const;
    void setWebView(QQuickWebView* webView);

    QString getLeaderboardID() const;
    void setLeaderboardID(const QString &leaderboardID);

    Gamer *getGamer();
    Leaderboard *getLeaderboard();

    bool getAsyncMode() const;
    void setAsyncMode(bool asyncMode);

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
    QQuickWebView* m_webView;
    QString m_achievmentsID;
    QString m_leaderboardID;
    Gamer m_gamer;
    Leaderboard m_leaderboard;
    bool m_asyncMode;
};

#endif // GAMEUPCOMMON_H
