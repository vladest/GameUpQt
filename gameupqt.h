#ifndef GAMEUPQT_H
#define GAMEUPQT_H

#include <QtQuick/QQuickItem>
#include "gamer.h"

class GameUpCommon;
class GameUpQt: public QQuickItem {

    Q_OBJECT

    Q_PROPERTY(QString apiKey READ apiKey WRITE setApiKey NOTIFY apiKeyChanged)
public:
    GameUpQt(QQuickItem *parent = 0);
    QString apiKey() const;
public slots:
    void setApiKey(QString apiKey);
    bool ping();
    QString loginAnonymous(const QString &username);
    Gamer *getGamer(const QString &username);
    void addUserToken(const QString &username, const QString &token);

signals:
    void apiKeyChanged(QString apiKey);
private:
    QString m_apiKey;
    GameUpCommon *guCommon;
};

#endif // GAMEUPQT_H
