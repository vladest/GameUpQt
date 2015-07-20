#ifndef GAMERACHIEVMENTS_H
#define GAMERACHIEVMENTS_H

#include <QObject>
#include <QDateTime>

class GamerAchievments : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString publicID READ publicID WRITE setPublicID NOTIFY publicIDChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(AchievmentType type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(int points READ points WRITE setPoints NOTIFY pointsChanged)
    Q_PROPERTY(AchievmentState state READ state WRITE setState NOTIFY stateChanged)
    Q_PROPERTY(int requiredCount READ requiredCount WRITE setRequiredCount NOTIFY requiredCountChanged)
    Q_PROPERTY(int count READ count WRITE setCount NOTIFY countChanged)
    Q_PROPERTY(QDateTime progressAt READ progressAt WRITE setProgressAt NOTIFY progressAtChanged)
    Q_PROPERTY(QDateTime completedAt READ completedAt WRITE setCompletedAt NOTIFY completedAtChanged)


public:
    enum AchievmentType {
        Normal,
        Incremental
    };
    enum AchievmentState {
        Visible,
        Secret,
        Hidden
    };

    Q_ENUM(AchievmentType)
    Q_ENUM(AchievmentState)

    explicit GamerAchievments(QObject *parent = 0);

    QString publicID() const;
    QString name() const;
    QString description() const;
    AchievmentType type() const;
    int points() const;
    AchievmentState state() const;
    int requiredCount() const;
    int count() const;
    QDateTime progressAt() const;
    QDateTime completedAt() const;

signals:

    void publicIDChanged(QString publicID);
    void nameChanged(QString name);
    void descriptionChanged(QString description);
    void typeChanged(AchievmentType type);
    void pointsChanged(int points);
    void stateChanged(AchievmentState state);
    void requiredCountChanged(int requiredCount);
    void countChanged(int count);
    void progressAtChanged(QDateTime progressAt);
    void completedAtChanged(QDateTime completedAt);

public slots:
    void setPublicID(QString publicID);
    void setName(QString name);
    void setDescription(QString description);
    void setType(AchievmentType type);
    void setPoints(int points);
    void setState(AchievmentState state);
    void setRequiredCount(int requiredCount);
    void setCount(int count);
    void setProgressAt(QDateTime progressAt);
    void setCompletedAt(QDateTime completedAt);
private:
    QString m_publicID;
    QString m_name;
    QString m_description;
    AchievmentType m_type;
    int m_points;
    AchievmentState m_state;
    int m_requiredCount;
    int m_count;
    QDateTime m_progressAt;
    QDateTime m_completedAt;
};

#endif // GAMERACHIEVMENTS_H
