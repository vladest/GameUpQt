#ifndef GAMEUPQTPLUGIN_H
#define GAMEUPQTPLUGIN_H

#include <QQmlExtensionPlugin>

/**
 * A plugin that exposes Box2D to QML in the form of declarative items.
 */
class Q_DECL_EXPORT GameUpQtPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT

    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    explicit GameUpQtPlugin(QObject *parent = 0);

    void registerTypes(const char *uri);
};

#endif // GAMEUPQTPLUGIN_H
