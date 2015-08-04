#include "gameupqt_p.h"
#include "gameonrequest.h"
#include "gamer.h"
#include "leaderboard.h"
#include <QUrlQuery>
#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
#include <QtWebEngine/qtwebengineglobal.h>
#include <QtWebEngine/private/qquickwebengineview_p.h>
#include <QtWebEngine/private/qquickwebengineloadrequest_p.h>
#else
#include "QtWebView/QQuickWebView"
#endif // QT_WEBVIEW_WEBENGINE_BACKEND
#include <QCryptographicHash>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

static const char gameOnAPIUrl[] = "https://api.gameup.io/v0/";
static const char gameOnAccountAPIUrl[] = "https://accounts.gameup.io/v0/";

GameUpQtPrivate::GameUpQtPrivate(QObject *parent): QObject(parent)
  , gonRequest(Q_NULLPTR)
  , lasterror(QNetworkReply::NoError)
  , m_webView(Q_NULLPTR)
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

Leaderboard *GameUpQtPrivate::getLeaderboard(const QString &id) {
    return m_leaderboards[id];
}

bool GameUpQtPrivate::getAsyncMode() const {
    return m_asyncMode;
}

void GameUpQtPrivate::setAsyncMode(bool asyncMode) {
    m_asyncMode = asyncMode;
}


QNetworkReply::NetworkError GameUpQtPrivate::getLasterror() const {
    return lasterror;
}

QString GameUpQtPrivate::getLastToken() const {
    return m_lastToken;
}


#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
void GameUpQtPrivate::webViewLoadingProgress(QQuickWebEngineLoadRequest *loadRequest) {

    qDebug() << "loadrequest" << loadRequest->url();
    QUrlQuery query = QUrlQuery(loadRequest->url());
    if (query.hasQueryItem("token")) {
        m_lastToken = query.queryItemValue("token");
    }
    if (!m_lastToken.isEmpty() || loadRequest->errorCode() != 0 || loadRequest->status() == QQuickWebEngineView::LoadFailedStatus) {
        disconnect(m_webView, &QQuickWebEngineView::loadingChanged, this, &GameUpQtPrivate::webViewLoadingProgress);
        if (!m_asyncMode && loop.isRunning())
            loop.quit();
        emit reqComplete(GameUpQt::Login);
    }
}

#else
void GameUpQtPrivate::webViewLoadingProgress(QQuickWebEngineLoadRequest *loadRequest) {
}
#endif

bool GameUpQtPrivate::ping() {
    if (Q_NULLPTR == gonRequest)
        return false;
    gonRequest->get(QString(gameOnAPIUrl), QList<RequestParameter>(), GameUpQt::Ping);
    if (!m_asyncMode) {
        loop.exec();
        return (lasterror == QNetworkReply::NoError);
    }
    return false;
}

void GameUpQtPrivate::doParseGamerToken() {
    if (lasterror == QNetworkReply::NoError && lastData.size() > 0) {
        QJsonDocument jsdoc = QJsonDocument::fromJson(lastData);
        QJsonObject sett2 = jsdoc.object();
        QJsonValue value = sett2.value(QString("token"));
        m_lastToken = value.toString();
        gonRequest->setToken(m_lastToken);
        addUserToken(m_lastUsername, m_lastToken);
    }
}

QString GameUpQtPrivate::login(GameUpQt::LoginType loginType, const QString &username) {
    m_lastToken.clear();
    m_lastUsername = username;
    gonRequest->setToken(m_usersTokens[username]);
    if (loginType == GameUpQt::Anonymous) {
        QString userhash = QString(QCryptographicHash::hash((username.toLocal8Bit()),QCryptographicHash::Md5).toHex());
        if (userhash.length() < 32 || userhash.length() > 128) {
            qWarning() << "Invalid username hash length" << userhash.length();
            return m_lastToken;
        }

        QJsonObject jobj;
        jobj.insert("id", userhash);
        QJsonDocument jsdoc = QJsonDocument(jobj);
        gonRequest->post(QString(gameOnAccountAPIUrl) + "gamer/login/anonymous", QList<RequestParameter>(), jsdoc.toJson(), GameUpQt::Login);
    } else if (loginType >= GameUpQt::GameUp && loginType <= GameUpQt::Google ) {
        if (m_webView) {
            QString loginString = "gameup";
            if (loginType == GameUpQt::Twitter)
                loginString = "twitter";
            else if (loginType == GameUpQt::Facebook)
                loginString = "facebook";
            else if (loginType == GameUpQt::Google)
                loginString = "google";
            QUrl url(QString(gameOnAccountAPIUrl) + "gamer/login/" + loginString);
            QUrlQuery query(url);
            query.addQueryItem(QByteArray("apiKey"), m_apiKey.toLatin1());
            url.setQuery(query);
            m_lastToken.clear();
#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
            connect(m_webView, &QQuickWebEngineView::loadingChanged, this, &GameUpQtPrivate::webViewLoadingProgress);
#else
#endif
            m_webView->setUrl(url);
        } else {
            qWarning() << "WebView does not setup! Check webView property";
        }
    }
    if (!m_asyncMode) {
        loop.exec();
        if (loginType == GameUpQt::Anonymous) {
            doParseGamerToken();
        }
    }
    return m_lastToken;
}

void GameUpQtPrivate::updateGamerData(const QString &username) {
    gonRequest->setToken(m_usersTokens[username]);
    gonRequest->get(QString(gameOnAPIUrl) + "gamer", QList<RequestParameter>(), GameUpQt::GamerDataUpdate);
    if (!m_asyncMode) {
        loop.exec();
        doUpdateGamerData();
    }
}

void GameUpQtPrivate::doUpdateGamerData() {
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
    gonRequest->get(QString(gameOnAPIUrl) + "gamer/achievement", QList<RequestParameter>(), GameUpQt::GamerAchievmentsUpdate);
    if (!m_asyncMode) {
        loop.exec();
        doUpdateGamerAchievments();
    }
}

void GameUpQtPrivate::doUpdateGamerAchievments() {
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
    gonRequest->get(QString(gameOnAPIUrl) + "gamer/leaderboard/" + m_leaderboardID, QList<RequestParameter>(), GameUpQt::LeaderboardUpdate);
    if (!m_asyncMode) {
        loop.exec();
        doUpdateLeaderboard();
    }
}

void GameUpQtPrivate::getLeaderboards(const QString &username) {
    gonRequest->setToken(m_usersTokens[username]);
    gonRequest->get(QString(gameOnAPIUrl) + "game/leaderboard", QList<RequestParameter>(), GameUpQt::LeaderboardUpdate);
    if (!m_asyncMode) {
        loop.exec();
        doUpdateLeaderboard();
    }
}

void GameUpQtPrivate::parseLeaderboard(const QJsonObject &jobj) {
    QString l_id = jobj.value(QString("leaderboard_id")).toString();
    if (l_id.isEmpty())
        return;
    Leaderboard *l_ = m_leaderboards[l_id];
    if (l_ == Q_NULLPTR)
        l_ = new Leaderboard;
    m_leaderboards[l_id] = l_;
    l_->setId(l_id);
    l_->setName(jobj.value(QString("name")).toString());
    l_->setPublic_id(jobj.value(QString("public_id")).toString());
    l_->setSort(jobj.value(QString("sort")).toString());
    l_->setType(jobj.value(QString("type")).toString());
    l_->setDisplayHint(jobj.value(QString("display_hint")).toString());
    l_->setLimit(jobj.value(QString("limit")).toInt(0));
    QJsonArray tagsjarr = jobj.value(QString("tags")).toArray();
    QStringList tags;
    foreach (QJsonValue v, tagsjarr)
        tags.append(v.toString());
    l_->setTags(tags);
    QJsonArray jarr = jobj.value(QString("entries")).toArray();
    if (jarr.count() > 0) {
        l_->clearEntries();
        foreach (QJsonValue v, jarr) {
            QJsonObject o = v.toObject();
            LeaderboardEntry *e = new LeaderboardEntry;
            e->setName(o.value(QString("name")).toString());
            e->setScore(o.value(QString("score")).toInt());
            e->setScoreAt(QDateTime::fromMSecsSinceEpoch(static_cast<qint64>(o.value(QString("score_at")).toDouble())));
            l_->addEntry(e);
        }
    }
}

void GameUpQtPrivate::doUpdateLeaderboards() {
    if (lasterror == QNetworkReply::NoError && lastData.size() > 0) {
        QJsonDocument jsdoc = QJsonDocument::fromJson(lastData);
        QJsonArray jarr = jsdoc.object().value(QString("leaderboards")).toArray();
        if (jarr.count() > 0) {
            foreach (QJsonValue v, jarr) {
                QJsonObject o = v.toObject();
                parseLeaderboard(o);
            }
        }
    }
}

void GameUpQtPrivate::doUpdateLeaderboard() {
    if (lasterror == QNetworkReply::NoError && lastData.size() > 0) {
        QJsonDocument jsdoc = QJsonDocument::fromJson(lastData);
        QJsonObject jobj = jsdoc.object().value("leaderboard").toObject();
        if (!jobj.isEmpty()) {
            parseLeaderboard(jobj);
        }
    }
}

void GameUpQtPrivate::updateGamerRank(const QString &username) {
    gonRequest->setToken(m_usersTokens[username]);
    gonRequest->get(QString(gameOnAPIUrl) + "gamer/leaderboard/" + m_leaderboardID, QList<RequestParameter>(), GameUpQt::GamerRankUpdate);
    if (!m_asyncMode) {
        loop.exec();
        doUpdateGamerRank();
    }
}

void GameUpQtPrivate::doUpdateGamerRank() {
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

void GameUpQtPrivate::setLeaderboardScore(const QString &username, int score, const QString &metadata) {
    QJsonObject jobj;
    jobj.insert("score", score);
    if (!metadata.isEmpty())
        jobj.insert("metadata", metadata);
    QJsonDocument jsdoc = QJsonDocument(jobj);
    gonRequest->setToken(m_usersTokens[username]);
    gonRequest->post(QString(gameOnAPIUrl) + "gamer/leaderboard/" + m_leaderboardID, QList<RequestParameter>(), jsdoc.toJson(), GameUpQt::SetLeaderboardScore);
    if (!m_asyncMode) {
        loop.exec();
    }
}

void GameUpQtPrivate::reqfinished(int id, QNetworkReply::NetworkError error, const QByteArray &data, const QVariant &reqId) {
    lasterror = error;
    lastData = data;
    if (m_asyncMode) {
        GameUpQt::ServerOps op = static_cast<GameUpQt::ServerOps>(reqId.toInt());
        if (op == GameUpQt::Login) {
            doParseGamerToken();
        } else if (op == GameUpQt::LeaderboardUpdate) {
            doUpdateLeaderboard();
        } else if (op == GameUpQt::GamerDataUpdate) {
            doUpdateGamerData();
        } else if (op == GameUpQt::GamerRankUpdate) {
            doUpdateGamerRank();
        } else if (op == GameUpQt::GamerAchievmentsUpdate) {
            doUpdateGamerAchievments();
        }
        emit reqComplete(op);
    } else if (loop.isRunning()) {
        loop.quit();
    }
    qDebug() << "data" << data << id;
    qDebug() << "Error:" << error;
}

