#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <QObject>
#include <QDateTime>
#include <QQmlListProperty>

class LeaderboardEntry: public QObject {
    Q_OBJECT

    Q_PROPERTY(int rank READ rank WRITE setRank  NOTIFY rankChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int score READ score WRITE setScore  NOTIFY scoreChanged)
    Q_PROPERTY(QString metadata READ metadata WRITE setMetadata NOTIFY metadataChanged)
    Q_PROPERTY(QDateTime scoreAt READ scoreAt WRITE setScoreAt NOTIFY scoreAtChanged)


public:
    explicit LeaderboardEntry(QObject *parent = 0);

    QString name() const;
    int score() const;
    QDateTime scoreAt() const;
    QString metadata() const;
    int rank() const;

public slots:
    void setName(QString name);
    void setScore(int score);
    void setScoreAt(QDateTime scoreAt);
    void setMetadata(QString metadata);
    void setRank(int rank);

signals:
    void nameChanged(QString name);
    void scoreChanged(int score);
    void scoreAtChanged(QDateTime scoreAt);
    void metadataChanged(QString metadata);
    void rankChanged(int rank);

private:

    QString m_name;
    int m_score;
    QDateTime m_scoreAt;
    QString m_metadata;
    int m_rank;
};

class Leaderboard : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString public_id READ public_id WRITE setPublic_id NOTIFY public_idChanged)
    Q_PROPERTY(QString sort READ sort WRITE setSort NOTIFY sortChanged)
    Q_PROPERTY(QString type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QString displayHint READ displayHint WRITE setDisplayHint NOTIFY displayHintChanged)
    Q_PROPERTY(QStringList tags READ tags WRITE setTags NOTIFY tagsChanged)
    Q_PROPERTY(int limit READ limit WRITE setLimit NOTIFY limitChanged)
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
    QString displayHint() const;
    QStringList tags() const;
    int limit() const;
    QString id() const;

signals:
    void nameChanged(QString name);
    void public_idChanged(QString public_id);
    void sortChanged(QString sort);
    void typeChanged(QString type);
    void entriesChanged();
    void displayHintChanged(QString displayHint);
    void tagsChanged(QStringList tags);
    void limitChanged(int limit);
    void idChanged(QString id);

public slots:
    void setName(QString name);
    void setPublic_id(QString public_id);
    void setSort(QString sort);
    void setType(QString type);
    void setDisplayHint(QString displayHint);
    void setTags(QStringList tags);
    void setLimit(int limit);
    void setId(QString id);

private:
    QString m_name;
    QString m_public_id;
    QString m_sort;
    QString m_type;
    QList<LeaderboardEntry*> m_entries;
    QString m_displayHint;
    QStringList m_tags;
    int m_limit;
    QString m_id;
};

#endif // LEADERBOARD_H
