#include "gameupqt.h"
#include "gameupcommon.h"
#include <QDebug>

GameUpQt::GameUpQt(QQuickItem *parent): QQuickItem(parent)
, guCommon(Q_NULLPTR){
}

#define CHECK_GU_PRIVATE(ret) \
    if (guCommon == Q_NULLPTR) { \
    qWarning() << "Error allocating GameUpServer"; \
    return ret ; \
    }

void GameUpQt::setApiKey(QString apiKey) {
    if (m_apiKey == apiKey)
        return;

    m_apiKey = apiKey;
    emit apiKeyChanged(apiKey);

    if (guCommon == Q_NULLPTR) {
        guCommon = new GameUpCommon();
    }
    if (guCommon == Q_NULLPTR) {
        qWarning() << "Error allocating GameUpServer";
        return;
    }
    guCommon->setApiKey(m_apiKey);
}

bool GameUpQt::ping() {
    CHECK_GU_PRIVATE(false);
    return guCommon->ping();
}

QString GameUpQt::loginAnonymous(const QString &username) {
    CHECK_GU_PRIVATE(QString(""));
    return guCommon->loginAnonymous(username);
}

Gamer *GameUpQt::getGamer(const QString &username) {
    CHECK_GU_PRIVATE(Q_NULLPTR);
    return guCommon->getGamer(username);
}

void GameUpQt::addUserToken(const QString &username, const QString &token) {
    CHECK_GU_PRIVATE();
    guCommon->addUserToken(username, token);
}

QString GameUpQt::apiKey() const {
    return m_apiKey;
}
