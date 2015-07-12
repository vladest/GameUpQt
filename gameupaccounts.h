#ifndef GAMEUPACCOUNTS_H
#define GAMEUPACCOUNTS_H

#include <QObject>


class GameUpAccounts : public QObject
{
    Q_OBJECT
public:
    enum LoginTypes {
        Anonymous,
        OAuth2,
        Facebook,
        Google,
        Twitter,
        GameUp
    };

    explicit GameUpAccounts(QObject *parent = 0);
    void login(const QString &apiKey, LoginTypes ltype = GameUpAccounts::GameUp);

signals:

public slots:
private:
    QString m_apiKey;
};

#endif // GAMEUPACCOUNTS_H
