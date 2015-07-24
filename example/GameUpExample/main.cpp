#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QNetworkProxyFactory>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QNetworkProxyFactory::setUseSystemConfiguration(true);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

