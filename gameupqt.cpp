#include "gameupqt.h"
#include "gameupcommon.h"
#include <QDebug>

GameUpQt::GameUpQt(QQuickItem *parent): QQuickItem(parent)
, guCommon(Q_NULLPTR){
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
    if (guCommon == Q_NULLPTR) {
        qWarning() << "Error allocating GameUpServer";
        return false;
    }
    return guCommon->ping();
}

QString GameUpQt::loginAnonymous(const QString &username) {
    if (guCommon == Q_NULLPTR) {
        qWarning() << "Error allocating GameUpServer";
        return QString("");
    }
    return guCommon->loginAnonymous(username);
}

QString GameUpQt::apiKey() const {
    return m_apiKey;
}
