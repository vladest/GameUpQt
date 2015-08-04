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

QString LeaderboardEntry::metadata() const {
    return m_metadata;
}

int LeaderboardEntry::rank() const {
    return m_rank;
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

void LeaderboardEntry::setMetadata(QString metadata) {
    if (m_metadata == metadata)
        return;

    m_metadata = metadata;
    emit metadataChanged(metadata);
}

void LeaderboardEntry::setRank(int rank) {
    if (m_rank == rank)
        return;

    m_rank = rank;
    emit rankChanged(rank);
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

QString Leaderboard::displayHint() const {
    return m_displayHint;
}

QStringList Leaderboard::tags() const {
    return m_tags;
}

int Leaderboard::limit() const {
    return m_limit;
}

QString Leaderboard::id() const {
    return m_id;
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

void Leaderboard::setDisplayHint(QString displayHint) {
    if (m_displayHint == displayHint)
        return;

    m_displayHint = displayHint;
    emit displayHintChanged(displayHint);
}

void Leaderboard::setTags(QStringList tags) {
    if (m_tags == tags)
        return;

    m_tags = tags;
    emit tagsChanged(tags);
}

void Leaderboard::setLimit(int limit) {
    if (m_limit == limit)
        return;

    m_limit = limit;
    emit limitChanged(limit);
}

void Leaderboard::setId(QString id) {
    if (m_id == id)
        return;

    m_id = id;
    emit idChanged(id);
}
