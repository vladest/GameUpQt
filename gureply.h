#ifndef O2TIMEDREPLYLIST_H
#define O2TIMEDREPLYLIST_H

#include <QList>
#include <QTimer>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QByteArray>

/// A network request/reply pair that can time out.
class GUReply: public QTimer {
    Q_OBJECT

public:
    GUReply(QNetworkReply *reply, int timeOut = 60 * 1000, QObject *parent = 0);

signals:
    void error(QNetworkReply::NetworkError);

public slots:
    /// When time out occurs, the QNetworkReply's error() signal is triggered.
    void onTimeOut();

public:
    QNetworkReply *reply;
};

/// List of O2Replies.
class GUReplyList {
public:
    /// Destructor.
    /// Deletes all O2Reply instances in the list.
    virtual ~GUReplyList();

    /// Create a new O2Reply from a QNetworkReply, and add it to this list.
    void add(QNetworkReply *reply);

    /// Add an O2Reply to the list, while taking ownership of it.
    void add(GUReply *reply);

    /// Remove item from the list that corresponds to a QNetworkReply.
    void remove(QNetworkReply *reply);

    /// Find an O2Reply in the list, corresponding to a QNetworkReply.
    /// @return Matching O2Reply or NULL.
    GUReply *find(QNetworkReply *reply);

protected:
    QList<GUReply *> replies_;
};

#endif // O2TIMEDREPLYLIST_H
