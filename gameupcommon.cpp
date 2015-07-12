#include "gameupcommon.h"
#include "gameonrequest.h"

GameUpCommon::GameUpCommon(QObject *parent): QObject(parent)
, gonRequest(Q_NULLPTR)
, lasterror(QNetworkReply::NoError){

}

QString GameUpCommon::apiKey() const {
    return m_apiKey;
}

QNetworkAccessManager *GameUpCommon::manager() {
    return &m_manager;
}

void GameUpCommon::setApiKey(const QString &apiKey) {
    m_apiKey = apiKey;
    gonRequest = new GameOnRequest(m_apiKey, &m_manager, this);
    connect(gonRequest, &GameOnRequest::finished, this, &GameUpCommon::reqfinished);
}

bool GameUpCommon::ping() {
    if (Q_NULLPTR == gonRequest)
        return false;
    gonRequest->get("", QList<RequestParameter>());
    loop.exec();
    return (lasterror == QNetworkReply::NoError);
}

void GameUpCommon::reqfinished(int id, QNetworkReply::NetworkError error, QByteArray data) {
    lasterror = error;
    if (loop.isRunning())
        loop.quit();
    qDebug() << "data" << data << id;
    qDebug() << "Error:" << error;
}

