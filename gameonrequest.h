#ifndef GAMEONREQUEST_H
#define GAMEONREQUEST_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#include "gureply.h"

struct RequestParameter {
    RequestParameter(const QByteArray &n, const QByteArray &v): name(n), value(v) {
    }
    bool operator <(const RequestParameter &other) const {
        return (name == other.name)? (value < other.value): (name < other.name);
    }
    QByteArray name;
    QByteArray value;
};

class GameOnRequest : public QObject
{
    Q_OBJECT
public:
    explicit GameOnRequest(const QString &apiKey, QNetworkAccessManager *manager, QObject *parent = 0);

    void get(const QString &partUrl, const QList<RequestParameter> &reqParameters);
    void post(const QString &partUrl, const QList<RequestParameter> &reqParameters, const QByteArray &data);
    void put(const QString &partUrl, const QList<RequestParameter> &reqParameters, const QByteArray &data);

    QNetworkReply *getReply() const;

    QString getToken() const;
    void setToken(const QString &token);

signals:
    /// Emitted when a request has been completed or failed.
    void finished(int id, QNetworkReply::NetworkError error, QByteArray data);

    /// Emitted when an upload has progressed.
    void uploadProgress(int id, qint64 bytesSent, qint64 bytesTotal);

protected slots:
    /// Handle refresh completion.
    void onRefreshFinished(QNetworkReply::NetworkError error);

    /// Handle request finished.
    void onRequestFinished();

    /// Handle request error.
    void onRequestError(QNetworkReply::NetworkError error);

    /// Re-try request (after successful token refresh).
    void retry();

    /// Finish the request, emit finished() signal.
    void finish();

    /// Handle upload progress.
    void onUploadProgress(qint64 uploaded, qint64 total);

private:
    bool setup(const QString &partUrl, const QList<RequestParameter> &parameters);

    enum Status {
        Idle, Requesting, ReRequesting
    };

private:

    QNetworkAccessManager *m_accessManager;
    QNetworkRequest request_;
    QList<RequestParameter> reqparameters_;
    QString m_apiKey;
    QString m_token;
    int id_;
    GUReplyList timedReplies_;
    QNetworkReply *reply_;
    QByteArray data_;
    QUrl url_;
    Status status_;
    QNetworkAccessManager::Operation operation_;
    QNetworkReply::NetworkError error_;
};

#endif // GAMEONREQUEST_H