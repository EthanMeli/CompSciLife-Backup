QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    gamelogic.cpp \
    lifeeventdialog.cpp \
    main.cpp \
    mainwindow.cpp \
    moneypopupdialog.cpp \
    playerinfodialog.cpp \
    shopdialog.cpp \
    supermoneypopupdialog.cpp \
    textpopupdialog.cpp \
    tiles.cpp \
    powerdialog.cpp

HEADERS += \
    gamelogic.h \
    lifeeventdialog.h \
    mainwindow.h \
    moneypopupdialog.h \
    playerinfodialog.h \
    shopdialog.h \
    supermoneypopupdialog.h \
    textpopupdialog.h \
    tiles.h \
    powerdialog.h

FORMS += \
    lifeeventdialog.ui \
    mainwindow.ui \
    moneypopupdialog.ui \
    playerinfodialog.ui \
    shopdialog.ui \
    supermoneypopupdialog.ui \
    textpopupdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
