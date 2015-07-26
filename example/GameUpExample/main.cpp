#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QNetworkProxyFactory>

#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
#include <QtWebEngine/qtwebengineglobal.h>
#endif

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QNetworkProxyFactory::setUseSystemConfiguration(true);

#ifdef QT_WEBVIEW_WEBENGINE_BACKEND
    QtWebEngine::initialize();
#endif


    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}

