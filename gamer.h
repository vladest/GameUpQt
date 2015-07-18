#ifndef GAMER_H
#define GAMER_H

#include <QObject>
#include <QDateTime>

class Gamer: public QObject {
    Q_OBJECT

    Q_PROPERTY(QString nickname READ nickname WRITE setNickname NOTIFY nicknameChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(int timezone READ timezone WRITE setTimezone NOTIFY timezoneChanged)
    Q_PROPERTY(QString location READ location WRITE setLocation NOTIFY locationChanged)
    Q_PROPERTY(QString locale READ locale WRITE setLocale NOTIFY localeChanged)
    Q_PROPERTY(QDateTime createdAt READ createdAt WRITE setCreatedAt NOTIFY createdAtChanged)

public:
    Gamer(QObject * parent = 0);
    virtual ~Gamer();

    QString nickname() const;
    QString name() const;
    int timezone() const;
    QString location() const;
    QString locale() const;
    QDateTime createdAt() const;

signals:
    void nicknameChanged(QString nickname);
    void nameChanged(QString name);
    void timezoneChanged(int timezone);
    void locationChanged(QString location);
    void localeChanged(QString locale);
    void createdAtChanged(QDateTime createdAt);

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
};

#endif // GAMER_H
