#ifndef GAMEUPQT_H
#define GAMEUPQT_H

#include <QtQuick/QQuickItem>
#include "gamer.h"
#include "leaderboard.h"


class GameUpQtPrivate;
class QQuickWebView;

class GameUpQt: public QQuickItem {

    Q_OBJECT

    Q_PROPERTY(QString apiKey READ apiKey WRITE setApiKey NOTIFY apiKeyChanged)
    Q_PROPERTY(QString leaderboardID READ leaderboardID WRITE setLeaderboardID NOTIFY leaderboardIDChanged)
    Q_PROPERTY(bool asyncMode READ asyncMode WRITE setAsyncMode NOTIFY asyncModeChanged)
    Q_PROPERTY(QQuickWebView* webView READ webView WRITE setWebView NOTIFY webViewChanged)

public:
    GameUpQt(QQuickItem *parent = 0);
    QString apiKey() const;
    QString leaderboardID() const;
    void setApiKey(QString apiKey);
    void setLeaderboardID(QString leaderboardID);
    bool asyncMode() const;
    QQuickWebView* webView() const;

public slots:
    bool ping();
    QString loginAnonymous(const QString &username);
    Gamer *getGamer(const QString &username);
    void addUserToken(const QString &username, const QString &token);
    Leaderboard *getLeaderboard(const QString &username);
    void updateGamerLeaderboard(const QString &username);
    void updateGamerAchievments(const QString &username);
    void setLeaderboardScore(const QString &username, int score);
    void setAsyncMode(bool asyncMode);
    void setWebView(QQuickWebView* webView);

signals:
    void apiKeyChanged(QString apiKey);
    void leaderboardIDChanged(QString leaderboardID);
    void asyncModeChanged(bool asyncMode);
    void webViewChanged(QQuickWebView* webView);

private:
    Q_DECLARE_PRIVATE(GameUpQt)
    GameUpQtPrivate *d_ptr;
};

#endif // GAMEUPQT_H
