#ifndef GAMEUPQT_H
#define GAMEUPQT_H

#include <QtQuick/QQuickItem>
#include "gamer.h"
#include "leaderboard.h"

class GameUpCommon;
class GameUpQt: public QQuickItem {

    Q_OBJECT

    Q_PROPERTY(QString apiKey READ apiKey WRITE setApiKey NOTIFY apiKeyChanged)
    Q_PROPERTY(QString achievmentsID READ achievmentsID WRITE setAchievmentsID NOTIFY achievmentsIDChanged)
    Q_PROPERTY(QString leaderboardID READ leaderboardID WRITE setLeaderboardID NOTIFY leaderboardIDChanged)
public:
    GameUpQt(QQuickItem *parent = 0);
    QString apiKey() const;
    QString achievmentsID() const;
    QString leaderboardID() const;
    void setApiKey(QString apiKey);
    void setAchievmentsID(QString achievmentsID);
    void setLeaderboardID(QString leaderboardID);

public slots:
    bool ping();
    QString loginAnonymous(const QString &username);
    Gamer *getGamer(const QString &username);
    void addUserToken(const QString &username, const QString &token);
    Leaderboard *getLeaderboard(const QString &username);

signals:
    void apiKeyChanged(QString apiKey);
    void achievmentsIDChanged(QString achievmentsID);
    void leaderboardIDChanged(QString leaderboardID);

private:
    QString m_apiKey;
    GameUpCommon *guCommon;
    QString m_achievmentsID;
    QString m_leaderboardID;
    Gamer m_gamer;
    Leaderboard m_leaderboard;
};

#endif // GAMEUPQT_H
