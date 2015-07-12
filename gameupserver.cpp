#include "gameupserver.h"
#include "gameonrequest.h"

GameUpServer::GameUpServer(QObject *parent) :
    QObject(parent) {

}

bool GameUpServer::ping() {
    gonRequest->get("", QList<RequestParameter>());
}

