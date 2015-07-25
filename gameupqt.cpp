#include "gameupqt.h"
#include "gameupqt_p.h"
#include <QDebug>

GameUpQt::GameUpQt(QQuickItem *parent): QQuickItem(parent)
  , d_ptr(new GameUpQtPrivate),
    m_asyncMode(true){
    d_ptr->q_ptr = this;
}

void GameUpQt::setApiKey(QString apiKey) {
    if (m_apiKey == apiKey)
        return;

    m_apiKey = apiKey;
    emit apiKeyChanged(apiKey);
    Q_D(GameUpQt);
    d->setApiKey(m_apiKey);
}

bool GameUpQt::ping() {
    Q_D(GameUpQt);
    return d->ping();
}

QString GameUpQt::loginAnonymous(const QString &username) {
    Q_D(GameUpQt);
    return d->loginAnonymous(username);
}

Gamer *GameUpQt::getGamer(const QString &username) {
    Q_D(GameUpQt);
    d->getGamer(username, &m_gamer);
    return &m_gamer;
}

void GameUpQt::addUserToken(const QString &username, const QString &token) {
    Q_D(GameUpQt);
    d->addUserToken(username, token);
}

Leaderboard *GameUpQt::getLeaderboard(const QString &username) {
    Q_D(GameUpQt);
    d->getLeaderboard(username, m_leaderboardID, &m_leaderboard, Q_NULLPTR);
    return &m_leaderboard;
}

GamerLeaderboard *GameUpQt::getGamerLeaderboard(const QString &username) {
    Q_D(GameUpQt);
    d->getLeaderboard(username, m_leaderboardID, Q_NULLPTR, &m_gamer);
    return m_gamer.gamerLeaderboard();
}

void GameUpQt::getGamerAchievments(const QString &username) {
    Q_D(GameUpQt);
    d->getGamerAchievments(username, &m_gamer);
}

void GameUpQt::setLeaderboardScore(const QString &username, int score) {
    Q_D(GameUpQt);
    d->setLeaderboardScore(username, m_leaderboardID, score);
}

void GameUpQt::setAsyncMode(bool asyncMode) {
    if (m_asyncMode == asyncMode)
        return;

    m_asyncMode = asyncMode;
    emit asyncModeChanged(asyncMode);
}

QString GameUpQt::apiKey() const {
    return m_apiKey;
}

QString GameUpQt::achievmentsID() const {
    return m_achievmentsID;
}

QString GameUpQt::leaderboardID() const {
    return m_leaderboardID;
}

void GameUpQt::setAchievmentsID(QString achievmentsID) {
    if (m_achievmentsID == achievmentsID)
        return;

    m_achievmentsID = achievmentsID;
    emit achievmentsIDChanged(achievmentsID);
}

void GameUpQt::setLeaderboardID(QString leaderboardID) {
    if (m_leaderboardID == leaderboardID)
        return;

    m_leaderboardID = leaderboardID;
    emit leaderboardIDChanged(leaderboardID);
}

bool GameUpQt::asyncMode() const {
    return m_asyncMode;
}
