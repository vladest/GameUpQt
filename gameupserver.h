#ifndef GAMEUPSERVER_H
#define GAMEUPSERVER_H

#include <QObject>
#include "gameupcommon.h"

class QNetworkAccessManager;
class GameOnRequest;
class GameUpServer : public QObject {
    Q_OBJECT
public:
    explicit GameUpServer(QObject *parent = 0);
    friend class GameUpCommon;
public slots:
    bool ping();
signals:

private:

private:
    GameOnRequest *gonRequest;
};

#endif // GAMEUPSERVER_H
