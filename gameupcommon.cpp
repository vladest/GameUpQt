#include "gameupcommon.h"
#include "gameonrequest.h"
#include "gamer.h"
#include "leaderboard.h"
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

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

void GameUpCommon::getGamer(const QString &username, Gamer *gamer) {
    if (!gamer)
        return;
    gonRequest->setToken(m_usersTokens[username]);
    gonRequest->get(QString(gameOnAPIUrl) + "gamer", QList<RequestParameter>());
    loop.exec();
    if (lasterror == QNetworkReply::NoError && lastData.size() > 0) {
        QJsonDocument jsdoc = QJsonDocument::fromJson(lastData);
        QJsonObject jobj = jsdoc.object();
        QJsonValue nickvalue = jobj.value(QString("nickname"));
        QJsonValue namevalue = jobj.value(QString("name"));
        QJsonValue createdvalue = jobj.value(QString("created_at"));
        gamer->setNickname(nickvalue.toString());
        gamer->setName(namevalue.toString());
        gamer->setCreatedAt(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(createdvalue.toDouble())));
    }
}

void GameUpCommon::addUserToken(const QString &username, const QString &token) {
    m_usersTokens[username] = token;
}

void GameUpCommon::getLeaderboard(const QString &username, const QString &lbid, Leaderboard *leaderboard) {
    if (!leaderboard)
        return;
    gonRequest->setToken(m_usersTokens[username]);
    gonRequest->get(QString(gameOnAPIUrl) + "gamer/leaderboard/" + lbid, QList<RequestParameter>());
    loop.exec();
    if (lasterror == QNetworkReply::NoError && lastData.size() > 0) {
        QJsonDocument jsdoc = QJsonDocument::fromJson(lastData);
        QJsonObject jobj = jsdoc.object().value("leaderboard").toObject();
        leaderboard->setName(jobj.value(QString("name")).toString());
        leaderboard->setPublic_id(jobj.value(QString("public_id")).toString());
        leaderboard->setSort(jobj.value(QString("sort")).toString());
        leaderboard->setType(jobj.value(QString("type")).toString());
        QJsonArray jarr = jobj.value(QString("entries")).toArray();
        leaderboard->clearEntries();
        foreach (QJsonValue v, jarr) {
            QJsonObject o = v.toObject();
            LeaderboardEntry *e = new LeaderboardEntry;
            e->setName(o.value(QString("name")).toString());
            e->setScore(o.value(QString("score")).toInt());
            e->setScoreAt(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(o.value(QString("score_at")).toDouble())));
            leaderboard->addEntry(e);
        }
    }
}

void GameUpCommon::reqfinished(int id, QNetworkReply::NetworkError error, QByteArray data) {
    lasterror = error;
    lastData = data;
    if (loop.isRunning())
        loop.quit();
    qDebug() << "data" << data << id;
    qDebug() << "Error:" << error;
}

