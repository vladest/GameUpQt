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
    Q_PROPERTY(QString leaderboardID READ leaderboardID WRITE setLeaderboardID NOTIFY leaderboardIDChanged)
    Q_PROPERTY(bool asyncMode READ asyncMode WRITE setAsyncMode NOTIFY asyncModeChanged)
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
        GamerDataUpdate,
        GamerRankUpdate,
        GamerAchievmentsUpdate,
        SetLeaderboardScore
    };
    Q_ENUM(ServerOps)

    GameUpQt(QQuickItem *parent = 0);
    QString apiKey() const;
    QString leaderboardID() const;
    void setApiKey(QString apiKey);
    void setLeaderboardID(QString leaderboardID);
    bool asyncMode() const;
#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
    QQuickWebEngineView* webView() const;
#else
    QQuickWebView* webView() const;
#endif
public slots:
    bool ping();
    QString login(LoginType loginType, const QString &username = "");
    Gamer *getGamer(const QString &username);
    void addUserToken(const QString &username, const QString &token);
    Leaderboard *getLeaderboard(const QString &username);
    void updateGamerLeaderboard(const QString &username);
    void updateGamerAchievments(const QString &username);
    void updateGamerRank(const QString &username);
    void updateGamerData(const QString &username);
    void setLeaderboardScore(const QString &username, int score);
    void setAsyncMode(bool asyncMode);
#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
    void setWebView(QQuickWebEngineView* webView);
#else
    void setWebView(QQuickWebView* webView);
#endif
private slots:
    void reqComplete(ServerOps op);
signals:
    void apiKeyChanged(QString apiKey);
    void leaderboardIDChanged(QString leaderboardID);
    void asyncModeChanged(bool asyncMode);
    void webViewChanged();

    void pingResultChanged(bool ok);
    void loginCompleted(const QString &token);
    void gamerLeaderboardUpdated();
    void gamerDataUpdated();
    void gamerRankUpdated();
    void gamerAchievmentsUpdated();
    void leaderboardScoreSetFinished();

private:
    Q_DECLARE_PRIVATE(GameUpQt)
    GameUpQtPrivate *d_ptr;
};

//Q_DECLARE_METATYPE(GameUpQt::ServerOps)

#endif // GAMEUPQT_H
