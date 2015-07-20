#include "gamer.h"

Gamer::Gamer(QObject *parent): QObject(parent) {

}

Gamer::~Gamer() {

}

QString Gamer::nickname() const {
    return m_nickname;
}

QString Gamer::name() const {
    return m_name;
}

int Gamer::timezone() const {
    return m_timezone;
}

QString Gamer::location() const {
    return m_location;
}

QString Gamer::locale() const {
    return m_locale;
}

QDateTime Gamer::createdAt() const {
    return m_createdAt;
}

GamerLeaderboard *Gamer::gamerLeaderboard() {
    return &m_gamerLeaderboard;
}

void Gamer::clearAchievments() {
    qDeleteAll(m_achievments);
    m_achievments.clear();
    emit achievmentsChanged();
}

void Gamer::addAchievment(GamerAchievments *ach) {
    m_achievments.append(ach);
    emit achievmentsChanged();
}

QQmlListProperty<GamerAchievments> Gamer::achievments() {
    return QQmlListProperty<GamerAchievments>(this, m_achievments);
}

void Gamer::setNickname(QString nickname) {
    if (m_nickname == nickname)
        return;

    m_nickname = nickname;
    emit nicknameChanged(nickname);
}

void Gamer::setName(QString name) {
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(name);
}

void Gamer::setTimezone(int timezone) {
    if (m_timezone == timezone)
        return;

    m_timezone = timezone;
    emit timezoneChanged(timezone);
}

void Gamer::setLocation(QString location) {
    if (m_location == location)
        return;

    m_location = location;
    emit locationChanged(location);
}

void Gamer::setLocale(QString locale) {
    if (m_locale == locale)
        return;

    m_locale = locale;
    emit localeChanged(locale);
}

void Gamer::setCreatedAt(QDateTime createdAt) {
    if (m_createdAt == createdAt)
        return;

    m_createdAt = createdAt;
    emit createdAtChanged(createdAt);
}
