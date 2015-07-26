#ifndef GAMEUPCOMMON_H
#define GAMEUPCOMMON_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include "gamer.h"
#include "leaderboard.h"
#include "gameupqt.h"

class GameOnRequest;
class Gamer;
class Leaderboard;

#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
    class QQuickWebEngineView;
    class QQuickWebEngineLoadRequest;
#else
    class QQuickWebView;
#endif

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
    QString login(GameUpQt::LoginType loginType, const QString &username);

#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
    QQuickWebEngineView* webView() const;
    void setWebView(QQuickWebEngineView* webView);
#else
    QQuickWebView* webView() const;
    void setWebView(QQuickWebView* webView);
#endif
    QString getLeaderboardID() const;
    void setLeaderboardID(const QString &leaderboardID);

    Gamer *getGamer();
    Leaderboard *getLeaderboard();

    bool getAsyncMode() const;
    void setAsyncMode(bool asyncMode);

private slots:
    void reqfinished(int id, QNetworkReply::NetworkError error, QByteArray data);
#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
    void webViewLoadingProgress(QQuickWebEngineLoadRequest *loadRequest);
#else
    void webViewLoadingProgress(QQuickWebEngineLoadRequest *loadRequest);
#endif



private:
    QString m_apiKey;
    QNetworkAccessManager m_manager;
    GameOnRequest *gonRequest;
    QEventLoop loop;
    QNetworkReply::NetworkError lasterror;
    QByteArray lastData;
    QString m_lastToken;
    QMap<QString, QString> m_usersTokens;

#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
    QQuickWebEngineView* m_webView;
#else
    QQuickWebView* m_webView;
#endif
    QString m_achievmentsID;
    QString m_leaderboardID;
    Gamer m_gamer;
    Leaderboard m_leaderboard;
    bool m_asyncMode;
};

#endif // GAMEUPCOMMON_H
