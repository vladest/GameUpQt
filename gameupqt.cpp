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
}

bool GameUpQt::ping() {
    if (m_apiKey.isEmpty()) {
        qWarning() << "Api key is empty";
        return false;
    }
    if (guCommon == Q_NULLPTR) {
        guCommon = new GameUpCommon();
    }
    if (guCommon == Q_NULLPTR) {
        qWarning() << "Error allocating GameUpServer";
        return false;
    }
    guCommon->setApiKey(m_apiKey);
    return guCommon->ping();
}

QString GameUpQt::apiKey() const {
    return m_apiKey;
}
