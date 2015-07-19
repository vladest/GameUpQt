#include "leaderboard.h"

LeaderboardEntry::LeaderboardEntry(QObject *parent) : QObject(parent){

}

QString LeaderboardEntry::name() const {
    return m_name;
}

int LeaderboardEntry::score() const {
    return m_score;
}

QDateTime LeaderboardEntry::scoreAt() const {
    return m_scoreAt;
}

void LeaderboardEntry::setName(QString name) {
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(name);
}

void LeaderboardEntry::setScore(int score) {
    if (m_score == score)
        return;

    m_score = score;
    emit scoreChanged(score);
}

void LeaderboardEntry::setScoreAt(QDateTime scoreAt) {
    if (m_scoreAt == scoreAt)
        return;

    m_scoreAt = scoreAt;
    emit scoreAtChanged(scoreAt);
}

Leaderboard::Leaderboard(QObject *parent) : QObject(parent) {

}

QString Leaderboard::name() const {
    return m_name;
}

QString Leaderboard::public_id() const {
    return m_public_id;
}

QString Leaderboard::sort() const {
    return m_sort;
}

QString Leaderboard::type() const {
    return m_type;
}

QQmlListProperty<LeaderboardEntry> Leaderboard::entries() {
    return QQmlListProperty<LeaderboardEntry>(this, m_entries);
}

void Leaderboard::clearEntries() {
    qDeleteAll(m_entries);
    m_entries.clear();
    emit entriesChanged();
}

void Leaderboard::addEntry(LeaderboardEntry *e) {
    m_entries.append(e);
    emit entriesChanged();
}

void Leaderboard::setName(QString name) {
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(name);
}

void Leaderboard::setPublic_id(QString public_id) {
    if (m_public_id == public_id)
        return;

    m_public_id = public_id;
    emit public_idChanged(public_id);
}

void Leaderboard::setSort(QString sort) {
    if (m_sort == sort)
        return;

    m_sort = sort;
    emit sortChanged(sort);
}

void Leaderboard::setType(QString type) {
    if (m_type == type)
        return;

    m_type = type;
    emit typeChanged(type);
}
