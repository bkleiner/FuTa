#-------------------------------------------------
#
# Project created by QtCreator 2012-03-10T20:30:50
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = FuTa

HEADERS += \
    widget.h \
    mainwindow.h \
    futa.h \
    file/wowfilemodel.h \
    file/wowfileitem.h \
    file/wowfile.h \
    widgets/waterwidget.h \
    widgets/layerwidget.h \
    widgets/heightwidget.h \
    widgets/filewizard.h \
    widgets/wowfilewidget.h \
    widgets/alphamapwidget.h \
    file/file.h \
    file/MapHeaders.h \
    file/maptile.h \
    file/mapchunk.h \
    file/misc.h \
    file/filebuffer.h

SOURCES += \
    widget.cpp \
    mainwindow.cpp \
    futa.cpp \
    file/wowfilemodel.cpp \
    file/wowfileitem.cpp \
    file/wowfile.cpp \
    widgets/waterwidget.cpp \
    widgets/layerwidget.cpp \
    widgets/heightwidget.cpp \
    widgets/filewizard.cpp \
    widgets/wowfilewidget.cpp \
    widgets/alphamapwidget.cpp \
    file/maptile.cpp \
    file/mapchunk.cpp \
    file/misc.cpp \
    file/filebuffer.cpp



CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11 -Wno-deprecated-register

RC_FILE = winres.rc
RESOURCES += ressources.qrc









