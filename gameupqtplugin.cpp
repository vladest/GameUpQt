#include "gameupqtplugin.h"
#include "gameupqt.h"

const int versionMajor = 1;
const int versionMinor = 0;

GameUpQtPlugin::GameUpQtPlugin(QObject *parent) :
    QQmlExtensionPlugin(parent) {
}

void GameUpQtPlugin::registerTypes(const char *uri)
{

    Q_ASSERT(QLatin1String(uri) == QLatin1String("GameUpQt"));

    qmlRegisterType<GameUpQt>(uri, versionMajor, versionMinor, "GameUpQt");
    qmlRegisterType<Gamer>(uri, versionMajor, versionMinor, "Gamer");
    qmlRegisterType<Leaderboard>(uri, versionMajor, versionMinor, "Leaderboard");
    qmlRegisterType<LeaderboardEntry>(uri, versionMajor, versionMinor, "LeaderboardEntry");
}
