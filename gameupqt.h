#ifndef GAMEUPQT_H
#define GAMEUPQT_H

#include <QtQuick/QQuickItem>

class Gamer;
class Leaderboard;
class GameUpQtPrivate;

#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
    class QQuickWebEngineView;
#else
    class QQuickWebView;
#endif

class GameUpQt: public QQuickItem {

    Q_OBJECT

    Q_PROPERTY(QString apiKey READ apiKey WRITE setApiKey NOTIFY apiKeyChanged)
    Q_PROPERTY(bool asyncMode READ asyncMode WRITE setAsyncMode NOTIFY asyncModeChanged)
    Q_PROPERTY(Gamer* gamer READ gamer NOTIFY gamerChanged)
    Q_PROPERTY(QQmlListProperty<Leaderboard> leaderboards READ leaderboards NOTIFY leaderboardsChanged)
#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
    Q_PROPERTY(QQuickWebEngineView* webView READ webView WRITE setWebView NOTIFY webViewChanged)
#else
    Q_PROPERTY(QQuickWebView* webView READ webView WRITE setWebView NOTIFY webViewChanged)
#endif

public:
    enum LoginType {
        Anonymous,
        GameUp,
        Twitter,
        Facebook,
        Google,
        OAuth2
    };
    Q_ENUM(LoginType)

    enum ServerOps {
        Ping,
        Login,
        LeaderboardUpdate,
        GetLeaderboards,
        GamerDataUpdate,
        GamerRankUpdate,
        GamerAchievmentsUpdate,
        SetLeaderboardScore
    };
    Q_ENUM(ServerOps)

    GameUpQt(QQuickItem *parent = 0);
    QString apiKey() const;
    void setApiKey(QString apiKey);
    bool asyncMode() const;
#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
    QQuickWebEngineView* webView() const;
#else
    QQuickWebView* webView() const;
#endif
    Gamer* gamer();
    QQmlListProperty<Leaderboard> leaderboards();

public slots:
    bool ping();
    QString login(LoginType loginType, const QString &username = "");
    void addUserToken(const QString &username, const QString &token);
    void updateGamerLeaderboard(const QString &username);
    void updateGamerAchievments(const QString &username);
    void updateGamerRank(const QString &username);
    void updateGamerData(const QString &username);
    void updateLeaderboards(const QString &username);
    void setLeaderboardScore(const QString &username, const QString &leaderboardId, int score, const QString &metadata);
    void setAsyncMode(bool asyncMode);
    Leaderboard* leaderboard(const QString &id);
#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
    void setWebView(QQuickWebEngineView* webView);
#else
    void setWebView(QQuickWebView* webView);
#endif
private slots:
    void reqComplete(ServerOps op);
signals:
    void apiKeyChanged(QString apiKey);
    void asyncModeChanged(bool asyncMode);
    void webViewChanged();
    void gamerChanged();
    void leaderboardChanged();

    void pingResultChanged(bool ok);
    void loginCompleted(const QString &token);
    void gamerLeaderboardUpdated();
    void gamerDataUpdated();
    void gamerRankUpdated();
    void gamerAchievmentsUpdated();
    void leaderboardScoreSetFinished();
    void leaderboardsChanged();

private:
    Q_DECLARE_PRIVATE(GameUpQt)
    GameUpQtPrivate *d_ptr;
};

//Q_DECLARE_METATYPE(GameUpQt::ServerOps)

#endif // GAMEUPQT_H
