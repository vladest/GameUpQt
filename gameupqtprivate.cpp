#include "gameupqt_p.h"
#include "gameonrequest.h"
#include "gamer.h"
#include "leaderboard.h"
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

static const char gameOnAPIUrl[] = "https://api.gameup.io/v0/";
static const char gameOnAccountAPIUrl[] = "https://accounts.gameup.io/v0/";

GameUpQtPrivate::GameUpQtPrivate(QObject *parent): QObject(parent)
  , gonRequest(Q_NULLPTR)
  , lasterror(QNetworkReply::NoError){

}

QString GameUpQtPrivate::apiKey() const {
    return m_apiKey;
}

QNetworkAccessManager *GameUpQtPrivate::manager() {
    return &m_manager;
}

void GameUpQtPrivate::setApiKey(const QString &apiKey) {
    m_apiKey = apiKey;
    gonRequest = new GameOnRequest(m_apiKey, &m_manager, this);
    connect(gonRequest, &GameOnRequest::finished, this, &GameUpQtPrivate::reqfinished);
}

bool GameUpQtPrivate::ping() {
    if (Q_NULLPTR == gonRequest)
        return false;
    gonRequest->get(QString(gameOnAPIUrl), QList<RequestParameter>());
    loop.exec();
    return (lasterror == QNetworkReply::NoError);
}

QString GameUpQtPrivate::loginAnonymous(const QString &username) {
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

QString GameUpQtPrivate::loginGameup(const QString &username) {
    gonRequest->setToken(m_usersTokens[username]);
    //TODO: need to implement multiplatform QtWebEngine (only desktop for now) or QtWebView (only iOS/Android/OSX atm) or redirect_uri on server side
    return QString("");
}

void GameUpQtPrivate::getGamer(const QString &username, Gamer *gamer) {
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

void GameUpQtPrivate::addUserToken(const QString &username, const QString &token) {
    m_usersTokens[username] = token;
}

void GameUpQtPrivate::getGamerAchievments(const QString &username, Gamer *gamer) {
    if (Q_NULLPTR == gamer)
        return;
    gonRequest->setToken(m_usersTokens[username]);
    gonRequest->get(QString(gameOnAPIUrl) + "gamer/achievement", QList<RequestParameter>());
    loop.exec();
    if (lasterror == QNetworkReply::NoError && lastData.size() > 0) {
        QJsonDocument jsdoc = QJsonDocument::fromJson(lastData);
        QJsonObject jobj = jsdoc.object();
        QJsonArray jarr = jobj.value(QString("achievements")).toArray();
        gamer->clearAchievments();
        foreach (QJsonValue v, jarr) {
            QJsonObject o = v.toObject();
            GamerAchievments *ach = new GamerAchievments;
            ach->setPublicID(o.value(QString("public_id")).toString());
            ach->setName(o.value(QString("name")).toString());
            ach->setDescription(o.value(QString("description")).toString());
            QString type = o.value(QString("type")).toString();
            if (type == "normal")
                ach->setType(GamerAchievments::Normal);
            else
                ach->setType(GamerAchievments::Incremental);
            QString state = o.value(QString("state")).toString();
            if (state == "visible")
                ach->setState(GamerAchievments::Visible);
            else if (state == "secret")
                ach->setState(GamerAchievments::Secret);
            else
                ach->setState(GamerAchievments::Hidden);
            ach->setPoints(o.value(QString("points")).toInt());
            ach->setRequiredCount(o.value(QString("required_count")).toInt());
            ach->setCount(o.value(QString("count")).toInt());
            ach->setProgressAt(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(o.value(QString("progress_at")).toDouble())));
            ach->setCompletedAt(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(o.value(QString("completed_at")).toDouble())));
            gamer->addAchievment(ach);
        }
    }
}

void GameUpQtPrivate::getLeaderboard(const QString &username, const QString &lbid, Leaderboard *leaderboard, Gamer *gamer) {
    gonRequest->setToken(m_usersTokens[username]);
    gonRequest->get(QString(gameOnAPIUrl) + "gamer/leaderboard/" + lbid, QList<RequestParameter>());
    loop.exec();
    if (lasterror == QNetworkReply::NoError && lastData.size() > 0) {
        QJsonDocument jsdoc = QJsonDocument::fromJson(lastData);
        if (leaderboard != Q_NULLPTR) {
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
        if (gamer != Q_NULLPTR) {
            QJsonObject jobj = jsdoc.object().value("rank").toObject();
            gamer->gamerLeaderboard()->setRank(jobj.value(QString("rank")).toInt());
            gamer->gamerLeaderboard()->setRankAt(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(jobj.value(QString("rank_at")).toDouble())));
            gamer->gamerLeaderboard()->setScore(jobj.value(QString("score")).toInt());
            gamer->gamerLeaderboard()->setScoreAt(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(jobj.value(QString("score_at")).toDouble())));
            gamer->gamerLeaderboard()->setLastScore(jobj.value(QString("last_score")).toInt());
            gamer->gamerLeaderboard()->setLastScoreAt(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(jobj.value(QString("last_score_at")).toDouble())));
            gamer->gamerLeaderboard()->setLastRank(jobj.value(QString("last_rank")).toInt());
            gamer->gamerLeaderboard()->setLastRankAt(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(jobj.value(QString("last_rank_at")).toDouble())));
            gamer->gamerLeaderboard()->setBestRank(jobj.value(QString("best_rank")).toInt());
            gamer->gamerLeaderboard()->setBestRankAt(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(jobj.value(QString("best_rank_at")).toDouble())));
        }
    }
}

void GameUpQtPrivate::setLeaderboardScore(const QString &username, const QString &lbid, int score) {
    QJsonObject jobj;
    jobj.insert("score", score);
    QJsonDocument jsdoc = QJsonDocument(jobj);
    gonRequest->setToken(m_usersTokens[username]);
    gonRequest->post(QString(gameOnAPIUrl) + "gamer/leaderboard/" + lbid, QList<RequestParameter>(), jsdoc.toJson());
    loop.exec();
}

void GameUpQtPrivate::reqfinished(int id, QNetworkReply::NetworkError error, QByteArray data) {
    lasterror = error;
    lastData = data;
    if (loop.isRunning())
        loop.quit();
    qDebug() << "data" << data << id;
    qDebug() << "Error:" << error;
}

