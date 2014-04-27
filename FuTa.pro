#-------------------------------------------------
#
# Project created by QtCreator 2012-03-10T20:30:50
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = FuTa

HEADERS += \
    widget.h \
    mainwindow.h \
    futa.h \
    MapHeaders.h \
    file/wowfilemodel.h \
    file/wowfilemanager.h \
    file/wowfileitem.h \
    file/wowfile.h \
    file/wdlfile.h \
    file/mpqfile.h \
    file/dbcfile.h \
    file/adtfile.h \
    widgets/waterwidget.h \
    widgets/lowresheightwidget.h \
    widgets/layerwidget.h \
    widgets/heightwidget.h \
    widgets/heightview.h \
    widgets/filewizard.h \
    widgets/wowfilewidget.h \
    widgets/alphamapwidget.h \
    file/m2file.h \
    ModelHeaders.h \
    vector3D.h \
    widgets/modelview.h \
    utilities/Utilities.h \
    wowfiles/WowChunkedFormat.h \
    wowfiles/Wdt.h \
    wowfiles/Wdl.h \
    wowfiles/Mphd.h \
    wowfiles/Mhdr.h \
    wowfiles/Mh2o.h \
    wowfiles/Mcnk.h \
    wowfiles/Mcin.h \
    wowfiles/Mcal.h \
    wowfiles/ChunkHeaders.h \
    wowfiles/Chunk.h \
    wowfiles/alpha/WdtAlpha.h \
    wowfiles/alpha/MphdAlpha.h \
    wowfiles/alpha/Monm.h \
    wowfiles/alpha/McvtAlpha.h \
    wowfiles/alpha/McnrAlpha.h \
    wowfiles/alpha/McnkAlpha.h \
    wowfiles/alpha/MainAlpha.h \
    wowfiles/alpha/AdtAlpha.h \
    wowfiles/cataclysm/McnrCata.h \
    wowfiles/cataclysm/McnkCataTextures.h \
    wowfiles/cataclysm/McnkCataObjects.h \
    wowfiles/cataclysm/McnkCata.h \
    wowfiles/cataclysm/AdtCataTextures.h \
    wowfiles/cataclysm/AdtCataObjects.h \
    wowfiles/cataclysm/AdtCata.h \
    wowfiles/lichking/McnrLk.h \
    wowfiles/lichking/McnkLk.h \
    wowfiles/lichking/AdtLk.h \
    file/blsfile.h \
    MiscHeaders.h \
    wowfiles/MainChunk.h

SOURCES += \
    widget.cpp \
    mainwindow.cpp \
    main.cpp \
    futa.cpp \
    file/wowfilemodel.cpp \
    file/wowfilemanager.cpp \
    file/wowfileitem.cpp \
    file/wowfile.cpp \
    file/wdlfile.cpp \
    file/mpqfile.cpp \
    file/dbcfile.cpp \
    file/adtfile.cpp \
    widgets/waterwidget.cpp \
    widgets/lowresheightwidget.cpp \
    widgets/layerwidget.cpp \
    widgets/heightwidget.cpp \
    widgets/heightview.cpp \
    widgets/filewizard.cpp \
    widgets/wowfilewidget.cpp \
    widgets/alphamapwidget.cpp \
    file/m2file.cpp \
    widgets/modelview.cpp \
    utilities/Utilities.cpp \
    wowfiles/WowChunkedFormat.cpp \
    wowfiles/Wdt.cpp \
    wowfiles/Wdl.cpp \
    wowfiles/Mphd.cpp \
    wowfiles/Mhdr.cpp \
    wowfiles/Mh2o.cpp \
    wowfiles/Mcnk.cpp \
    wowfiles/Mcin.cpp \
    wowfiles/Mcal.cpp \
    wowfiles/Chunk.cpp \
    wowfiles/alpha/WdtAlpha.cpp \
    wowfiles/alpha/MphdAlpha.cpp \
    wowfiles/alpha/Monm.cpp \
    wowfiles/alpha/McvtAlpha.cpp \
    wowfiles/alpha/McnrAlpha.cpp \
    wowfiles/alpha/McnkAlpha.cpp \
    wowfiles/alpha/MainAlpha.cpp \
    wowfiles/alpha/AdtAlpha.cpp \
    wowfiles/cataclysm/McnrCata.cpp \
    wowfiles/cataclysm/McnkCataTextures.cpp \
    wowfiles/cataclysm/McnkCataObjects.cpp \
    wowfiles/cataclysm/McnkCata.cpp \
    wowfiles/cataclysm/AdtCataTextures.cpp \
    wowfiles/cataclysm/AdtCataObjects.cpp \
    wowfiles/cataclysm/AdtCata.cpp \
    wowfiles/lichking/McnrLk.cpp \
    wowfiles/lichking/McnkLk.cpp \
    wowfiles/lichking/AdtLk.cpp \
    file/blsfile.cpp \
    wowfiles/MainChunk.cpp

RC_FILE = winres.rc

unix:{
LIBS += -L/usr/lib/ -lGLU -lStormLib

INCLUDEPATH += /usr/include
}

win32 {
        INCLUDEPATH += .
        INCLUDEPATH += win32/include
        LIBS += -L"../FuTa/win32/lib"

        # GLEW (windows only)
        #DEFINES += GLEW_STATIC INCL_GLEW
        #LIBS += -lglew32
        # Linking order correction (glew->glu->opegl)
        #LIBS += -lglu32 -lopengl32 -lgdi32 -luser32

        #LIBS += -lStormLibdll #shared linking on win
        LIBS += -lStormLib # static linking on win
}

RESOURCES += ressources.qrc









