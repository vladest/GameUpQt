#-------------------------------------------------
#
# Project created by QtCreator 2015-06-28T15:30:49
#
#-------------------------------------------------

QT       += network qml quick
OBJECTS_DIR = .obj
MOC_DIR = .moc

QT       -= gui

TARGET = GameUpQt
TEMPLATE = lib
CONFIG += staticlib

SOURCES += gameupqt.cpp \
#    gameupaccounts.cpp \
#    gameupgamer.cpp \
#    gameupgame.cpp \
#    gameupserver.cpp \
    gameonrequest.cpp \
    replyserver.cpp \
    gureply.cpp \
    gameupcommon.cpp

HEADERS += gameupqt.h \
#    gameupaccounts.h \
#    gameupgamer.h \
#    gameupgame.h \
#    gameupserver.h \
    gameonrequest.h \
    replyserver.h \
    gureply.h \
    gameupcommon.h
unix {
    target.path = . #/usr/lib
    INSTALLS += target
}
