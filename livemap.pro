#-------------------------------------------------
#
# Project created by QtCreator 2016-08-28T09:39:21
#
#-------------------------------------------------

CONFIG += c++11
#CONFIG += qwt
#CONFIG   += rtti
QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
DEFINES += QT_DEPRECATED_WARNINGS

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TARGET = livemap
TEMPLATE = app


SOURCES += main.cpp\
    qhexedit/chunks.cpp \
    qhexedit/commands.cpp \
    qhexedit/qhexedit.cpp \
    mainwindow.cpp \
    libs/J2534.cpp \
    libs/ftdi.cpp

HEADERS  += mainwindow.h \
    custom_tablewidget.h \
    ecu.h \
    #ecu_x_comm.h \
    ecu_interface.h \
    gauge_widget.h \
    inno_interface.h \
    op13.h \
    op20.h \
    qhexedit/chunks.h \
    qhexedit/commands.h \
    qhexedit/qhexedit.h \
    libs/J2534.h \
    libs/j2534_tactrix.h \
    #libs/libusb/include/libusb.h \
    mathparser2.h \
    libs/ftdi.h \
    libs/ftdi_types.h \
    enumdev.h \
    tactrix_inno.h \
    xmlparser.h

FORMS    += mainwindow.ui

#DISTFILES +=

CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

#INCLUDEPATH += libs/libusb/include
#LIBS += -Llibs/libusb/MinGW64/dll -lusb-1.0
LIBS += -lSetupapi
LIBS += -ladvapi32
LIBS += -luser32

CONFIG(release, debug|release):QMAKE_LFLAGS_RELEASE += -static -static-libgcc

QMAKE_LFLAGS_RELEASE += /LTCG
QMAKE_CXXFLAGS  += /Ox

QMAKE_CXXFLAGS  += -flto -funroll-loops
QMAKE_CXXFLAGS  += -fforce-addr
QMAKE_CXXFLAGS  += -m32 -Ofast -march=core2 -mtune=core2
#QMAKE_CXXFLAGS  += -mfpmath=sse
QMAKE_CXXFLAGS  += -msse4

DEFINES += QHEXEDIT_EXPORTS

DISTFILES +=

RESOURCES += \
    res.qrc
