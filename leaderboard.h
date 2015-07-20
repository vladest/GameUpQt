#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <QObject>
#include <QDateTime>
#include <QQmlListProperty>

class LeaderboardEntry: public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int score READ score WRITE setScore  NOTIFY scoreChanged)
    Q_PROPERTY(QDateTime scoreAt READ scoreAt WRITE setScoreAt NOTIFY scoreAtChanged)


public:
    explicit LeaderboardEntry(QObject *parent = 0);

    QString name() const;
    int score() const;
    QDateTime scoreAt() const;

public slots:
    void setName(QString name);
    void setScore(int score);
    void setScoreAt(QDateTime scoreAt);

signals:
    void nameChanged(QString name);
    void scoreChanged(int score);
    void scoreAtChanged(QDateTime scoreAt);
private:

    QString m_name;
    int m_score;
    QDateTime m_scoreAt;

};

class Leaderboard : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString public_id READ public_id WRITE setPublic_id NOTIFY public_idChanged)
    Q_PROPERTY(QString sort READ sort WRITE setSort NOTIFY sortChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QQmlListProperty<LeaderboardEntry> entries READ entries NOTIFY entriesChanged)

public:
    explicit Leaderboard(QObject *parent = 0);

    QString name() const;
    QString public_id() const;
    QString sort() const;
    QString type() const;
    QQmlListProperty<LeaderboardEntry> entries();
    void clearEntries();
    void addEntry(LeaderboardEntry*);

signals:
    void nameChanged(QString name);
    void public_idChanged(QString public_id);
    void sortChanged(QString sort);
    void typeChanged(QString type);
    void entriesChanged();

public slots:
    void setName(QString name);
    void setPublic_id(QString public_id);
    void setSort(QString sort);
    void setType(QString type);
private:
    QString m_name;
    QString m_public_id;
    QString m_sort;
    QString m_type;
    QList<LeaderboardEntry*> m_entries;

};

#endif // LEADERBOARD_H
