QT       += network qml quick
OBJECTS_DIR = .obj
MOC_DIR = .moc

QT       -= gui

TARGET = $$qtLibraryTarget(GameUpQt)
contains(QT_CONFIG, reduce_exports): CONFIG += hide_symbols
TEMPLATE = lib
#CONFIG += staticlib
CONFIG += plugin
TARGETPATH = GameUpQt
API_VER=1.0

#Qt 5.5 and higher
QT += webview-private

SOURCES += gameupqt.cpp \
    gameonrequest.cpp \
    replyserver.cpp \
    gureply.cpp \
    gamer.cpp \
    leaderboard.cpp \
    gamerachievments.cpp \
    gamerleaderboard.cpp \
    gameupqtplugin.cpp \
    gameupqtprivate.cpp

HEADERS += gameupqt.h \
    gameonrequest.h \
    replyserver.h \
    gureply.h \
    gamer.h \
    leaderboard.h \
    gamerachievments.h \
    gamerleaderboard.h \
    gameupqtplugin.h \
    gameupqt_p.h

importPath = $$[QT_INSTALL_QML]/$$replace(TARGETPATH, \\., /).$$API_VER
target.path = $${importPath}

qmldir.path = $${importPath}
qmldir.files += $$PWD/qmldir


INSTALLS += target qmldir
