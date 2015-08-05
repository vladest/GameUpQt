#ifndef GAMEUPCOMMON_H
#define GAMEUPCOMMON_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QQmlListProperty>
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
    Q_OBJECT
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
    //get list of leaderboards
    void getLeaderboards(const QString &username);
    void updateGamerRank(const QString &username);
    void setLeaderboardScore(const QString &username, int score, const QString &metadata = "");
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
    Leaderboard *getLeaderboard(const QString &id);

    bool getAsyncMode() const;
    void setAsyncMode(bool asyncMode);

    QNetworkReply::NetworkError getLasterror() const;

    QString getLastToken() const;    
    QQmlListProperty<Leaderboard> leaderboards();
private slots:
    void reqfinished(int id, QNetworkReply::NetworkError error, const QByteArray &data, const QVariant &reqId);
#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
    void webViewLoadingProgress(QQuickWebEngineLoadRequest *loadRequest);
#else
    void webViewLoadingProgress(QQuickWebEngineLoadRequest *loadRequest);
#endif

signals:
    void reqComplete(GameUpQt::ServerOps op);

private:
    void doUpdateGamerData();
    void doUpdateGamerAchievments();
    void doUpdateLeaderboard();
    void doUpdateGamerRank();
    void doParseGamerToken();

    //parse list of leaderboards
    void doUpdateLeaderboards();
    //parse single leaderboard entry
    void parseLeaderboard(const QJsonObject &jobj);
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
    QMap <QString, Leaderboard*> m_leaderboards;
    bool m_asyncMode;
    QString m_lastUsername;
};

#endif // GAMEUPCOMMON_H
