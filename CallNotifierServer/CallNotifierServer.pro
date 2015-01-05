#-------------------------------------------------
#
# Project created by QtCreator 2014-10-08T16:10:37
#
#-------------------------------------------------

QT       += network core gui multimedia
#DEFINES += UNICODE

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEPENDPATH += . ./cryptopp
INCLUDEPATH +=  ./cryptopp

CONFIG(debug, debug|release): LIBS+=  -L"../cryptopp_debug/debug"
CONFIG(release, debug|release): LIBS+=  -L"../cryptopp_release/release"

CONFIG(debug, debug|release): LIBS+=  -L"../cryptopp_debug_MSVC/debug"
CONFIG(debug, debug|release): INCLUDEPATH+=  ../cryptopp_debug_MSVC/debug/
CONFIG(release, debug|release): LIBS+=  -L"../cryptopp_release_MSVC/release"
CONFIG(release, debug|release): INCLUDEPATH+=  ../cryptopp_release_MSVC/release/

LIBS+= -lcryptopp

TARGET = CallNotifierServer
TEMPLATE = app

SOURCES += main.cpp\
        mainwnd.cpp \
    SettingsDialog.cpp \
    AESWrapper.cpp \
    ServerSocket.cpp \
    ListenerSocket.cpp \
    HistoryItemDelegate.cpp \
    NotificationItem.cpp \
    LinkLabel.cpp \
    NotificationItemDelegate.cpp

HEADERS  += mainwnd.h \
    SettingsDialog.h \
    AESWrapper.h \
    ServerSocket.h \
    ListenerSocket.h \
    HistoryItemDelegate.h \
    NotificationItem.h \
    LinkLabel.h \
    NotificationItemDelegate.h

FORMS    += mainwnd.ui \
    SettingsDialog.ui

RESOURCES += \
    CallNotifierServer.qrc

RC_FILE = cn.rc

OTHER_FILES +=
