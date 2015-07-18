#include "gameupcommon.h"
#include "gameonrequest.h"
#include "gamer.h"

#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>

static const char gameOnAPIUrl[] = "https://api.gameup.io/v0/";
static const char gameOnAccountAPIUrl[] = "https://accounts.gameup.io/v0/";

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
    gonRequest->get(QString(gameOnAPIUrl), QList<RequestParameter>());
    loop.exec();
    return (lasterror == QNetworkReply::NoError);
}

QString GameUpCommon::loginAnonymous(const QString &username) {
    QString token;
    QString userhash = QString(QCryptographicHash::hash((username.toLocal8Bit()),QCryptographicHash::Md5).toHex());
    if (userhash.length() < 32 || userhash.length() > 128) {
        qWarning() << "Invalid username hash length" << userhash.length();
        return token;
    }

    QJsonObject jobj;
    jobj.insert("id", userhash);
    QJsonDocument jsdoc = QJsonDocument(jobj);
    gonRequest->post(QString(gameOnAccountAPIUrl) + "gamer/login/anonymous", QList<RequestParameter>(), jsdoc.toJson());
    loop.exec();
    if (lasterror == QNetworkReply::NoError && lastData.size() > 0) {
        jsdoc = QJsonDocument::fromJson(lastData);
        QJsonObject sett2 = jsdoc.object();
        QJsonValue value = sett2.value(QString("token"));
        token = value.toString();
        gonRequest->setToken(token);
        addUserToken(username, token);
    }
    return token;
}

Gamer *GameUpCommon::getGamer(const QString &username) {
    Gamer *gamer = Q_NULLPTR;
    gonRequest->setToken(m_usersTokens[username]);
    gonRequest->get(QString(gameOnAPIUrl) + "gamer", QList<RequestParameter>());
    loop.exec();
    if (lasterror == QNetworkReply::NoError && lastData.size() > 0) {
        QJsonDocument jsdoc = QJsonDocument::fromJson(lastData);
        QJsonObject sett2 = jsdoc.object();
        QJsonValue nickvalue = sett2.value(QString("nickname"));
        QJsonValue namevalue = sett2.value(QString("name"));
        QJsonValue createdvalue = sett2.value(QString("created_at"));
        gamer = new Gamer;
        gamer->setNickname(nickvalue.toString());
        gamer->setName(namevalue.toString());
        gamer->setCreatedAt(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(createdvalue.toDouble())));
    }
    return gamer;
}

void GameUpCommon::addUserToken(const QString &username, const QString &token) {
    m_usersTokens[username] = token;
}

void GameUpCommon::reqfinished(int id, QNetworkReply::NetworkError error, QByteArray data) {
    lasterror = error;
    lastData = data;
    if (loop.isRunning())
        loop.quit();
    qDebug() << "data" << data << id;
    qDebug() << "Error:" << error;
}

