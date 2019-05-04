#-------------------------------------------------
#
# Project created by QtCreator 2016-08-28T09:39:21
#
#-------------------------------------------------

LIBS += -lSetupapi

QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ELM-QT
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp \
    J2534.cpp \
    ftdi.cpp \

HEADERS  += mainwindow.h \
    J2534.h \
    j2534_tactrix.h \
    mathparser2.h \
    dynamicwindow.h \
    ftdi.h \
    ftdi_types.h \
    DMA.h \
    enumdev.h \
    xmldomparser.h \

FORMS    += mainwindow.ui
#QMAKE_LFLAGS_RELEASE += -static -static-libgcc
DISTFILES +=
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
#QMAKE_CXXFLAGS - опции компиляции.
QMAKE_CXXFLAGS  += -m32 -mfpmath=sse -Ofast -flto -march=native -funroll-loops
#QMAKE_LFLAGS - опции линковки.
