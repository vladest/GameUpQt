#ifndef GUREPLYSERVER_H
#define GUREPLYSERVER_H

#include <QTcpServer>
#include <QMap>
#include <QByteArray>
#include <QString>

/// HTTP server to process authentication response.
class GUReplyServer: public QTcpServer {
    Q_OBJECT

public:
    explicit GUReplyServer(QObject *parent = 0);
    ~GUReplyServer();

signals:
    void verificationReceived(QMap<QString, QString>);

public slots:
    void onIncomingConnection();
    void onBytesReady();
    QMap<QString, QString> parseQueryParams(QByteArray *data);
};

#endif // GUREPLYSERVER_H
