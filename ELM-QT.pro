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
    libs/J2534.cpp \
    libs/ftdi.cpp \

HEADERS  += mainwindow.h \
    libs/J2534.h \
    libs/j2534_tactrix.h \
    libs/libusb/include/libusb.h \
    mathparser2.h \
    dynamicwindow.h \
    libs/ftdi.h \
    libs/ftdi_types.h \
    DMA.h \
    enumdev.h \
    xmldomparser.h \

LIBS += -Llibs/libusb/MinGW64/dll -lusb-1.0
INCLUDEPATH += libs/libusb/include
FORMS    += mainwindow.ui
#QMAKE_LFLAGS_RELEASE += -static -static-libgcc
DISTFILES +=
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT
#QMAKE_CXXFLAGS - опции компиляции.
QMAKE_CXXFLAGS  += -mfpmath=sse -Ofast -flto -march=native -funroll-loops
#QMAKE_LFLAGS - опции линковки.
№QMAKE_LFLAGS += -lusb-1.0
