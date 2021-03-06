#include "gamerleaderboard.h"

GamerLeaderboard::GamerLeaderboard(QObject *parent): QObject(parent) {

}

GamerLeaderboard::~GamerLeaderboard() {

}

QDateTime GamerLeaderboard::rankAt() const {
    return m_rankAt;
}

int GamerLeaderboard::score() const {
    return m_score;
}

QDateTime GamerLeaderboard::scoreAt() const {
    return m_scoreAt;
}

int GamerLeaderboard::lastScore() const {
    return m_lastScore;
}

QDateTime GamerLeaderboard::lastScoreAt() const {
    return m_lastScoreAt;
}

int GamerLeaderboard::lastRank() const {
    return m_lastRank;
}

QDateTime GamerLeaderboard::lastRankAt() const {
    return m_lastRankAt;
}

int GamerLeaderboard::bestRank() const {
    return m_bestRank;
}

QDateTime GamerLeaderboard::bestRankAt() const {
    return m_bestRankAt;
}

int GamerLeaderboard::rank() const {
    return m_rank;
}

void GamerLeaderboard::setRankAt(QDateTime rankAt) {
    if (m_rankAt == rankAt)
        return;

    m_rankAt = rankAt;
    emit rankAtChanged(rankAt);
}

void GamerLeaderboard::setScore(int score) {
    if (m_score == score)
        return;

    m_score = score;
    emit scoreChanged(score);
}

void GamerLeaderboard::setScoreAt(QDateTime scoreAt) {
    if (m_scoreAt == scoreAt)
        return;

    m_scoreAt = scoreAt;
    emit scoreAtChanged(scoreAt);
}

void GamerLeaderboard::setLastScore(int lastScore) {
    if (m_lastScore == lastScore)
        return;

    m_lastScore = lastScore;
    emit lastScoreChanged(lastScore);
}

void GamerLeaderboard::setLastScoreAt(QDateTime lastScoreAt) {
    if (m_lastScoreAt == lastScoreAt)
        return;

    m_lastScoreAt = lastScoreAt;
    emit lastScoreAtChanged(lastScoreAt);
}

void GamerLeaderboard::setLastRank(int lastRank) {
    if (m_lastRank == lastRank)
        return;

    m_lastRank = lastRank;
    emit lastRankChanged(lastRank);
}

void GamerLeaderboard::setLastRankAt(QDateTime lastRankAt) {
    if (m_lastRankAt == lastRankAt)
        return;

    m_lastRankAt = lastRankAt;
    emit lastRankAtChanged(lastRankAt);
}

void GamerLeaderboard::setBestRank(int bestRank) {
    if (m_bestRank == bestRank)
        return;

    m_bestRank = bestRank;
    emit bestRankChanged(bestRank);
}

void GamerLeaderboard::setBestRankAt(QDateTime bestRankAt) {
    if (m_bestRankAt == bestRankAt)
        return;

    m_bestRankAt = bestRankAt;
    emit bestRankAtChanged(bestRankAt);
}

void GamerLeaderboard::setRank(int rank) {
    if (m_rank == rank)
        return;

    m_rank = rank;
    emit rankChanged(rank);
}
