#include <QTimer>
#include <QNetworkReply>

#include "gureply.h"

GUReply::GUReply(QNetworkReply *r, int timeOut, QObject *parent): QTimer(parent), reply(r) {
    setSingleShot(true);
    connect(this, SIGNAL(error(QNetworkReply::NetworkError)), reply, SIGNAL(error(QNetworkReply::NetworkError)), Qt::QueuedConnection);
    connect(this, SIGNAL(timeout()), this, SLOT(onTimeOut()), Qt::QueuedConnection);
    start(timeOut);
}

void GUReply::onTimeOut() {
    emit error(QNetworkReply::TimeoutError);
}

GUReplyList::~GUReplyList() {
    foreach (GUReply *timedReply, replies_) {
        delete timedReply;
    }
}

void GUReplyList::add(QNetworkReply *reply) {
    add(new GUReply(reply));
}

void GUReplyList::add(GUReply *reply) {
    replies_.append(reply);
}

void GUReplyList::remove(QNetworkReply *reply) {
    GUReply *o2Reply = find(reply);
    if (o2Reply) {
        o2Reply->stop();
        (void)replies_.removeOne(o2Reply);
    }
}

GUReply *GUReplyList::find(QNetworkReply *reply) {
    foreach (GUReply *timedReply, replies_) {
        if (timedReply->reply == reply) {
            return timedReply;
        }
    }
    return 0;
}
