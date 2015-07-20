#ifndef GAMER_H
#define GAMER_H

#include <QObject>
#include <QDateTime>
class GamerLeaderboard: public QObject {
    Q_OBJECT
    /*
    "rank": 1,
        "rank_at": 1394019063468,
        "score": 1000,
        "score_at": 1394019063468,
        "last_score": 100,
        "last_score_at": 1394017053468,
        "last_rank": 22,
        "last_rank_at": 1419703699693,
        "best_rank": 1,
        "best_rank_at": 1394019063468
        */
    Q_PROPERTY(int rank READ rank WRITE setRank NOTIFY rankChanged)
    Q_PROPERTY(QDateTime rankAt READ rankAt WRITE setRankAt NOTIFY rankAtChanged)
    Q_PROPERTY(int score READ score WRITE setScore NOTIFY scoreChanged)
    Q_PROPERTY(QDateTime scoreAt READ scoreAt WRITE setScoreAt NOTIFY scoreAtChanged)
    Q_PROPERTY(int lastScore READ lastScore WRITE setLastScore NOTIFY lastScoreChanged)
    Q_PROPERTY(QDateTime lastScoreAt READ lastScoreAt WRITE setLastScoreAt NOTIFY lastScoreAtChanged)
    Q_PROPERTY(int lastRank READ lastRank WRITE setLastRank NOTIFY lastRankChanged)
    Q_PROPERTY(QDateTime lastRankAt READ lastRankAt WRITE setLastRankAt NOTIFY lastRankAtChanged)
    Q_PROPERTY(int bestRank READ bestRank WRITE setBestRank NOTIFY bestRankChanged)
    Q_PROPERTY(QDateTime bestRankAt READ bestRankAt WRITE setBestRankAt NOTIFY bestRankAtChanged)

public:
    GamerLeaderboard(QObject * parent = 0);
    virtual ~GamerLeaderboard();

    QDateTime rankAt() const;
    int score() const;
    QDateTime scoreAt() const;
    int lastScore() const;
    QDateTime lastScoreAt() const;
    int lastRank() const;
    QDateTime lastRankAt() const;
    int bestRank() const;
    QDateTime bestRankAt() const;
    int rank() const;

public slots:
    void setRankAt(QDateTime rankAt);
    void setScore(int score);
    void setScoreAt(QDateTime scoreAt);
    void setLastScore(int lastScore);
    void setLastScoreAt(QDateTime lastScoreAt);
    void setLastRank(int lastRank);
    void setLastRankAt(QDateTime lastRankAt);
    void setBestRank(int bestRank);
    void setBestRankAt(QDateTime bestRankAt);
    void setRank(int rank);

signals:
    void rankAtChanged(QDateTime rankAt);
    void scoreChanged(int score);
    void scoreAtChanged(QDateTime scoreAt);
    void lastScoreChanged(int lastScore);
    void lastScoreAtChanged(QDateTime lastScoreAt);
    void lastRankChanged(int lastRank);
    void lastRankAtChanged(QDateTime lastRankAt);
    void bestRankChanged(int bestRank);
    void bestRankAtChanged(QDateTime bestRankAt);
    void rankChanged(int rank);

private:
    QDateTime m_rankAt;
    int m_score;
    QDateTime m_scoreAt;
    int m_lastScore;
    QDateTime m_lastScoreAt;
    int m_lastRank;
    QDateTime m_lastRankAt;
    int m_bestRank;
    QDateTime m_bestRankAt;
    int m_rank;
};

class Gamer: public QObject {
    Q_OBJECT

    Q_PROPERTY(QString nickname READ nickname WRITE setNickname NOTIFY nicknameChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int timezone READ timezone WRITE setTimezone NOTIFY timezoneChanged)
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY locationChanged)
    Q_PROPERTY(QString locale READ locale WRITE setLocale NOTIFY localeChanged)
    Q_PROPERTY(QDateTime createdAt READ createdAt WRITE setCreatedAt NOTIFY createdAtChanged)
    Q_PROPERTY(GamerLeaderboard* gamerLeaderboard READ gamerLeaderboard NOTIFY gamerLeaderboardChanged)

public:
    Gamer(QObject * parent = 0);
    virtual ~Gamer();

    QString nickname() const;
    QString name() const;
    int timezone() const;
    QString location() const;
    QString locale() const;
    QDateTime createdAt() const;
    GamerLeaderboard* gamerLeaderboard();

signals:
    void nicknameChanged(QString nickname);
    void nameChanged(QString name);
    void timezoneChanged(int timezone);
    void locationChanged(QString location);
    void localeChanged(QString locale);
    void createdAtChanged(QDateTime createdAt);
    void gamerLeaderboardChanged(GamerLeaderboard* gamerLeaderboard);

public slots:
    void setNickname(QString nickname);
    void setName(QString name);
    void setTimezone(int timezone);
    void setLocation(QString location);
    void setLocale(QString locale);
    void setCreatedAt(QDateTime createdAt);

private:
    QString m_nickname; /** Nickname, intended for easy public display. */
    QString m_name; /** A real name, if one was provided when the gamer signed up. */
    int m_timezone; /** Time zone of the gamer. */
    QString m_location; /** Location of the gamer. */
    QString m_locale; /** Locale string for this gamer. */
    QDateTime m_createdAt; /** When the gamer first registered with GameUp. */
    GamerLeaderboard m_gamerLeaderboard;
};

#endif // GAMER_H
