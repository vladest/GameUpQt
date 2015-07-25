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
  , lasterror(QNetworkReply::NoError)
  , m_asyncMode(false) {

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

#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
QQuickWebEngineView *GameUpQtPrivate::webView() const {
#else
QQuickWebView *GameUpQt::webView() const {
#endif
    return m_webView;
}

#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
void GameUpQtPrivate::setWebView(QQuickWebEngineView *webView) {
#else
void GameUpQtPrivate::setWebView(QQuickWebView *webView) {
#endif
    m_webView = webView;
}

Gamer *GameUpQtPrivate::getGamer() {
    return &m_gamer;
}

QString GameUpQtPrivate::getLeaderboardID() const {
    return m_leaderboardID;
}

void GameUpQtPrivate::setLeaderboardID(const QString &leaderboardID) {
    m_leaderboardID = leaderboardID;
}

Leaderboard *GameUpQtPrivate::getLeaderboard() {
    return &m_leaderboard;
}

bool GameUpQtPrivate::getAsyncMode() const {
    return m_asyncMode;
}

void GameUpQtPrivate::setAsyncMode(bool asyncMode) {
    m_asyncMode = asyncMode;
}

void GameUpQtPrivate::updateGamerData(const QString &username) {
    gonRequest->setToken(m_usersTokens[username]);
    gonRequest->get(QString(gameOnAPIUrl) + "gamer", QList<RequestParameter>());
    loop.exec();
    if (lasterror == QNetworkReply::NoError && lastData.size() > 0) {
        QJsonDocument jsdoc = QJsonDocument::fromJson(lastData);
        QJsonObject jobj = jsdoc.object();
        QJsonValue nickvalue = jobj.value(QString("nickname"));
        QJsonValue namevalue = jobj.value(QString("name"));
        QJsonValue createdvalue = jobj.value(QString("created_at"));
        m_gamer.setNickname(nickvalue.toString());
        m_gamer.setName(namevalue.toString());
        m_gamer.setCreatedAt(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(createdvalue.toDouble())));
    }
}

void GameUpQtPrivate::addUserToken(const QString &username, const QString &token) {
    m_usersTokens[username] = token;
}

void GameUpQtPrivate::updateGamerAchievments(const QString &username) {
    gonRequest->setToken(m_usersTokens[username]);
    gonRequest->get(QString(gameOnAPIUrl) + "gamer/achievement", QList<RequestParameter>());
    loop.exec();
    if (lasterror == QNetworkReply::NoError && lastData.size() > 0) {
        QJsonDocument jsdoc = QJsonDocument::fromJson(lastData);
        QJsonObject jobj = jsdoc.object();
        QJsonArray jarr = jobj.value(QString("achievements")).toArray();
        m_gamer.clearAchievments();
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
            m_gamer.addAchievment(ach);
        }
    }
}

void GameUpQtPrivate::updateLeaderboard(const QString &username) {
    gonRequest->setToken(m_usersTokens[username]);
    gonRequest->get(QString(gameOnAPIUrl) + "gamer/leaderboard/" + m_leaderboardID, QList<RequestParameter>());
    loop.exec();
    if (lasterror == QNetworkReply::NoError && lastData.size() > 0) {
        QJsonDocument jsdoc = QJsonDocument::fromJson(lastData);
        QJsonObject jobj = jsdoc.object().value("leaderboard").toObject();
        m_leaderboard.setName(jobj.value(QString("name")).toString());
        m_leaderboard.setPublic_id(jobj.value(QString("public_id")).toString());
        m_leaderboard.setSort(jobj.value(QString("sort")).toString());
        m_leaderboard.setType(jobj.value(QString("type")).toString());
        QJsonArray jarr = jobj.value(QString("entries")).toArray();
        m_leaderboard.clearEntries();
        foreach (QJsonValue v, jarr) {
            QJsonObject o = v.toObject();
            LeaderboardEntry *e = new LeaderboardEntry;
            e->setName(o.value(QString("name")).toString());
            e->setScore(o.value(QString("score")).toInt());
            e->setScoreAt(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(o.value(QString("score_at")).toDouble())));
            m_leaderboard.addEntry(e);
        }
    }
}

void GameUpQtPrivate::updateGamerRank(const QString &username) {
    gonRequest->setToken(m_usersTokens[username]);
    gonRequest->get(QString(gameOnAPIUrl) + "gamer/leaderboard/" + m_leaderboardID, QList<RequestParameter>());
    loop.exec();
    if (lasterror == QNetworkReply::NoError && lastData.size() > 0) {
        QJsonDocument jsdoc = QJsonDocument::fromJson(lastData);
        QJsonObject jobj = jsdoc.object().value("rank").toObject();
        m_gamer.gamerLeaderboard()->setRank(jobj.value(QString("rank")).toInt());
        m_gamer.gamerLeaderboard()->setRankAt(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(jobj.value(QString("rank_at")).toDouble())));
        m_gamer.gamerLeaderboard()->setScore(jobj.value(QString("score")).toInt());
        m_gamer.gamerLeaderboard()->setScoreAt(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(jobj.value(QString("score_at")).toDouble())));
        m_gamer.gamerLeaderboard()->setLastScore(jobj.value(QString("last_score")).toInt());
        m_gamer.gamerLeaderboard()->setLastScoreAt(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(jobj.value(QString("last_score_at")).toDouble())));
        m_gamer.gamerLeaderboard()->setLastRank(jobj.value(QString("last_rank")).toInt());
        m_gamer.gamerLeaderboard()->setLastRankAt(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(jobj.value(QString("last_rank_at")).toDouble())));
        m_gamer.gamerLeaderboard()->setBestRank(jobj.value(QString("best_rank")).toInt());
        m_gamer.gamerLeaderboard()->setBestRankAt(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(jobj.value(QString("best_rank_at")).toDouble())));
    }
}

void GameUpQtPrivate::setLeaderboardScore(const QString &username, int score) {
    QJsonObject jobj;
    jobj.insert("score", score);
    QJsonDocument jsdoc = QJsonDocument(jobj);
    gonRequest->setToken(m_usersTokens[username]);
    gonRequest->post(QString(gameOnAPIUrl) + "gamer/leaderboard/" + m_leaderboardID, QList<RequestParameter>(), jsdoc.toJson());
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
