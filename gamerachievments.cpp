#include "gamerachievments.h"

GamerAchievments::GamerAchievments(QObject *parent) : QObject(parent)
{

}

QString GamerAchievments::publicID() const
{
    return m_publicID;
}

QString GamerAchievments::name() const
{
    return m_name;
}

QString GamerAchievments::description() const
{
    return m_description;
}

GamerAchievments::AchievmentType GamerAchievments::type() const
{
    return m_type;
}

int GamerAchievments::points() const
{
    return m_points;
}

GamerAchievments::AchievmentState GamerAchievments::state() const
{
    return m_state;
}

int GamerAchievments::requiredCount() const
{
    return m_requiredCount;
}

int GamerAchievments::count() const
{
    return m_count;
}

QDateTime GamerAchievments::progressAt() const
{
    return m_progressAt;
}

QDateTime GamerAchievments::completedAt() const
{
    return m_completedAt;
}

void GamerAchievments::setPublicID(QString publicID)
{
    if (m_publicID == publicID)
        return;

    m_publicID = publicID;
    emit publicIDChanged(publicID);
}

void GamerAchievments::setName(QString name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(name);
}

void GamerAchievments::setDescription(QString description)
{
    if (m_description == description)
        return;

    m_description = description;
    emit descriptionChanged(description);
}

void GamerAchievments::setType(GamerAchievments::AchievmentType type)
{
    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged(type);
}

void GamerAchievments::setPoints(int points)
{
    if (m_points == points)
        return;

    m_points = points;
    emit pointsChanged(points);
}

void GamerAchievments::setState(GamerAchievments::AchievmentState state)
{
    if (m_state == state)
        return;

    m_state = state;
    emit stateChanged(state);
}

void GamerAchievments::setRequiredCount(int requiredCount)
{
    if (m_requiredCount == requiredCount)
        return;

    m_requiredCount = requiredCount;
    emit requiredCountChanged(requiredCount);
}

void GamerAchievments::setCount(int count)
{
    if (m_count == count)
        return;

    m_count = count;
    emit countChanged(count);
}

void GamerAchievments::setProgressAt(QDateTime progressAt)
{
    if (m_progressAt == progressAt)
        return;

    m_progressAt = progressAt;
    emit progressAtChanged(progressAt);
}

void GamerAchievments::setCompletedAt(QDateTime completedAt)
{
    if (m_completedAt == completedAt)
        return;

    m_completedAt = completedAt;
    emit completedAtChanged(completedAt);
}

