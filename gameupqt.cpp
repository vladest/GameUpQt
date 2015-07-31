#include "gameupqt.h"
#include "gameupqt_p.h"
#include <QDebug>

GameUpQt::GameUpQt(QQuickItem *parent): QQuickItem(parent)
  , d_ptr(new GameUpQtPrivate) {
    qRegisterMetaType<GameUpQt::ServerOps>("GameUpQt::ServerOps");
    connect(d_ptr, &GameUpQtPrivate::reqComplete, this, &GameUpQt::reqComplete);
}

void GameUpQt::setApiKey(QString apiKey) {
    Q_D(GameUpQt);
    d->setApiKey(apiKey);
    emit apiKeyChanged(apiKey);
}

bool GameUpQt::ping() {
    Q_D(GameUpQt);
    return d->ping();
}

QString GameUpQt::login(LoginType loginType, const QString &username) {
    Q_D(GameUpQt);
    return d->login(loginType, username);
}

Gamer *GameUpQt::getGamer(const QString &username) {
    Q_UNUSED(username);
    Q_D(GameUpQt);
    return d->getGamer();
}

void GameUpQt::addUserToken(const QString &username, const QString &token) {
    Q_D(GameUpQt);
    d->addUserToken(username, token);
}

Leaderboard *GameUpQt::getLeaderboard(const QString &username) {
    Q_UNUSED(username);
    Q_D(GameUpQt);
    return d->getLeaderboard();
}

void GameUpQt::updateGamerLeaderboard(const QString &username) {
    Q_D(GameUpQt);
    d->updateLeaderboard(username);
}

void GameUpQt::updateGamerAchievments(const QString &username) {
    Q_D(GameUpQt);
    d->updateGamerAchievments(username);
}

void GameUpQt::updateGamerRank(const QString &username) {
    Q_D(GameUpQt);
    d->updateGamerRank(username);
}

void GameUpQt::updateGamerData(const QString &username) {
    Q_D(GameUpQt);
    d->updateGamerData(username);
}

void GameUpQt::setLeaderboardScore(const QString &username, int score) {
    Q_D(GameUpQt);
    d->setLeaderboardScore(username, score);
}

void GameUpQt::setAsyncMode(bool asyncMode) {
    Q_D(GameUpQt);
    d->setAsyncMode(asyncMode);
    emit asyncModeChanged(asyncMode);
}
#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
void GameUpQt::setWebView(QQuickWebEngineView *webView) {
#else
void GameUpQt::setWebView(QQuickWebView *webView) {
#endif
    Q_D(GameUpQt);
    d->setWebView(webView);
    emit webViewChanged();
}

void GameUpQt::reqComplete(GameUpQt::ServerOps op) {
    //qDebug() << "got op:" << op;
    Q_D(GameUpQt);
    if (op == Ping) {
        bool res = (d->getLasterror() == QNetworkReply::NoError);
        emit pingResultChanged(res);
    } else if (op == Login) {
        emit loginCompleted(d->getLastToken());
    } else if (op == LeaderboardUpdate) {
        emit gamerLeaderboardUpdated();
    } else if (op == GamerDataUpdate) {
        emit gamerDataUpdated();
    } else if (op == GamerRankUpdate) {
        emit gamerRankUpdated();
    } else if (op == GamerAchievmentsUpdate) {
        emit gamerAchievmentsUpdated();
    } else if (op == SetLeaderboardScore) {
        emit leaderboardScoreSetFinished();
    }
}

QString GameUpQt::apiKey() const {
    Q_D(const GameUpQt);
    return d->apiKey();
}

QString GameUpQt::leaderboardID() const {
    Q_D(const GameUpQt);
    return d->getLeaderboardID();
}

void GameUpQt::setLeaderboardID(QString leaderboardID) {
    Q_D(GameUpQt);
    d->setLeaderboardID(leaderboardID);
    emit leaderboardIDChanged(leaderboardID);
}

bool GameUpQt::asyncMode() const {
    Q_D(const GameUpQt);
    return d->getAsyncMode();
}

#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
QQuickWebEngineView *GameUpQt::webView() const {
#else
QQuickWebView *GameUpQt::webView() const {
#endif
    Q_D(const GameUpQt);
    return d->webView();
}
