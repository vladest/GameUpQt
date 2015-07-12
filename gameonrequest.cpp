#include <QTimer>
#include <QUrlQuery>
#include "gameonrequest.h"

static const char gameOnAPIUrl[] = "https://api.gameup.io/v0/";

GameOnRequest::GameOnRequest(const QString &apiKey, QNetworkAccessManager *manager, QObject *parent) :
    QObject(parent),
    m_accessManager(manager),
    m_apiKey(apiKey),
    id_(-1),
    status_(Idle)
{
    qRegisterMetaType<QNetworkReply::NetworkError>("QNetworkReply::NetworkError");
}

void GameOnRequest::get(const QString &partUrl, const QList<RequestParameter> &reqParameters) {
    if (!setup(partUrl, reqParameters))
        return;
    qDebug() << "api" << request_.rawHeader("username");
    reply_ = m_accessManager->get(request_);
    timedReplies_.add(reply_);
    connect(reply_, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onRequestError(QNetworkReply::NetworkError)), Qt::QueuedConnection);
    connect(reply_, SIGNAL(finished()), this, SLOT(onRequestFinished()), Qt::QueuedConnection);
}

void GameOnRequest::post(const QString &partUrl, const QList<RequestParameter> &reqParameters, const QByteArray &data) {
    if (!setup(partUrl, reqParameters))
        return;
    reply_ = m_accessManager->post(request_, data);
    timedReplies_.add(reply_);
    connect(reply_, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onRequestError(QNetworkReply::NetworkError)), Qt::QueuedConnection);
    connect(reply_, SIGNAL(finished()), this, SLOT(onRequestFinished()), Qt::QueuedConnection);
    connect(reply_, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(onUploadProgress(qint64,qint64)));
}


void GameOnRequest::put(const QString &partUrl, const QList<RequestParameter> &reqParameters, const QByteArray &data) {
    if (!setup(partUrl, reqParameters))
        return;
    reply_ = m_accessManager->put(request_, data);
    timedReplies_.add(reply_);
    connect(reply_, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onRequestError(QNetworkReply::NetworkError)), Qt::QueuedConnection);
    connect(reply_, SIGNAL(finished()), this, SLOT(onRequestFinished()), Qt::QueuedConnection);
    connect(reply_, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(onUploadProgress(qint64,qint64)));
}

bool GameOnRequest::setup(const QString &partUrl, const QList<RequestParameter> &parameters) {
    if (status_ != Idle) {
        qWarning() << "GameOnRequest::setup: Another request pending";
        return false;
    }
    id_++;

    QList<RequestParameter> params;
    QUrl url(QString(gameOnAPIUrl) + partUrl);
    QUrlQuery query(url);
    //params.append(RequestParameter(QByteArray("apiKey"), m_apiKey.toLatin1()));
    params.append(parameters);
    // Return a copy of the original request with authorization header set

    reqparameters_ = params;

    foreach(RequestParameter p, params) {
        query.addQueryItem(p.name, p.value);
    }
    url.setQuery(query);
    status_ = Requesting;
    request_.setUrl(url);

    request_.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request_.setHeader(QNetworkRequest::UserAgentHeader, "curl/7.30.0");
    request_.setRawHeader(QByteArray("Accept"), QByteArray("application/json"));
    QString auth = QString("%1:%2").arg(m_apiKey).arg(m_token);
    qDebug() << "auth:" << auth << auth.toLatin1().toBase64();
    request_.setRawHeader(QByteArray("Authorization"), QByteArray("Basic ") + auth.toLatin1().toBase64());

    return true;
}

QString GameOnRequest::getToken() const {
    return m_token;
}

void GameOnRequest::setToken(const QString &token) {
    m_token = token;
}

QNetworkReply *GameOnRequest::getReply() const {
    return reply_;
}

void GameOnRequest::onRefreshFinished(QNetworkReply::NetworkError error) {
    if (status_ != Requesting) {
        qWarning() << "GameOnRequest::onRefreshFinished: No pending request";
        return;
    }
    if (QNetworkReply::NoError == error) {
        QTimer::singleShot(100, this, SLOT(retry()));
    } else {
        error_ = error;
        QTimer::singleShot(10, this, SLOT(finish()));
    }
}

void GameOnRequest::onRequestFinished() {
    QNetworkReply *senderReply = qobject_cast<QNetworkReply *>(sender());
    error_ = senderReply->error();
    if (status_ == Idle) {
        return;
    }
    if (reply_ != senderReply) {
        return;
    }
    if (error_ == QNetworkReply::NoError) {
        QTimer::singleShot(10, this, SLOT(finish()));
    }
}

void GameOnRequest::onRequestError(QNetworkReply::NetworkError error) {
    qWarning() << "GameOnRequest::onRequestError: Error" << (int)error;
    if (status_ == Idle) {
        return;
    }
    if (reply_ != qobject_cast<QNetworkReply *>(sender())) {
        return;
    }
    int httpStatus = reply_->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    qWarning() << "GameOnRequest::onRequestError: HTTP status" << httpStatus << reply_->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    ///trace() << reply_->readAll();
    if ((status_ == Requesting) && (httpStatus == 401)) {
        // Call refresh. Note the instance might live in a different thread
//        if (QMetaObject::invokeMethod(authenticator_, "refresh")) {
//            return;
//        }
        qCritical() << "GameOnRequest::onRequestError: Invoking remote refresh failed";
    }
    error_ = error;
    QTimer::singleShot(10, this, SLOT(finish()));
}

void GameOnRequest::onUploadProgress(qint64 uploaded, qint64 total) {
    if (status_ == Idle) {
        qWarning() << "GameOnRequest::onUploadProgress: No pending request";
        return;
    }
    if (reply_ != qobject_cast<QNetworkReply *>(sender())) {
        return;
    }
    emit uploadProgress(id_, uploaded, total);
}

void GameOnRequest::finish() {
    QByteArray data;
    if (status_ == Idle) {
        qWarning() << "GameOnRequest::finish: No pending request";
        return;
    }
    data = reply_->readAll();
    status_ = Idle;
    timedReplies_.remove(reply_);
    reply_->disconnect(this);
    reply_->deleteLater();
    emit finished(id_, error_, data);
}

void GameOnRequest::retry() {
    if (status_ != Requesting) {
        qWarning() << "GameOnRequest::retry: No pending request";
        return;
    }
    timedReplies_.remove(reply_);
    reply_->disconnect(this);
    reply_->deleteLater();
    QUrl url = url_;
    QUrlQuery query(url);
    //query.addQueryItem(O2_OAUTH2_ACCESS_TOKEN, authenticator_->token());
    foreach(RequestParameter p, reqparameters_) {
        query.addQueryItem(p.name, p.value);
    }
    url.setQuery(query);
    request_.setUrl(url);
    status_ = ReRequesting;
    switch (operation_) {
    case QNetworkAccessManager::GetOperation:
        reply_ = m_accessManager->get(request_);
        break;
    case QNetworkAccessManager::PostOperation:
        reply_ = m_accessManager->post(request_, data_);
        break;
    default:
        reply_ = m_accessManager->put(request_, data_);
    }
    timedReplies_.add(reply_);
    connect(reply_, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onRequestError(QNetworkReply::NetworkError)), Qt::QueuedConnection);
    connect(reply_, SIGNAL(finished()), this, SLOT(onRequestFinished()), Qt::QueuedConnection);
    connect(reply_, SIGNAL(uploadProgress(qint64,qint64)), this, SLOT(onUploadProgress(qint64,qint64)));
}

