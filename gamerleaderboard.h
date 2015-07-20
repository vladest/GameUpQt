#ifndef GAMERLEADERBOARD_H
#define GAMERLEADERBOARD_H

#include <QObject>
#include <QDateTime>

class GamerLeaderboard: public QObject {
    Q_OBJECT

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

#endif // GAMERLEADERBOARD_H
