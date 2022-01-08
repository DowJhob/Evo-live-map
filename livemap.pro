#-------------------------------------------------
#
# Project created by QtCreator 2016-08-28T09:39:21
#
#-------------------------------------------------

CONFIG += c++11
#CONFIG += qhexedit4
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


SOURCES += src/main.cpp\
    src/abstract-memory.cpp \
    src/comm-device-interface/op13.cpp \
    src/ecu-proto/ECU-interface.cpp \
    src/comm-device-interface/comm-device-interface.cpp \
    src/comm-device-interface/j2534-interface.cpp \
    src/ecu-proto/evoX-DMA.cpp \
    src/ecu-proto/jcsbanksDMA.cpp \
    src/ecu-proto/stockDMA.cpp \
    src/controller.cpp \
    src/ecu/ecu-definition.cpp \
    src/ecu/rawstockmsg.cpp \
    src/fast-notation.cpp \
    src/enumdev.cpp \
    src/libs/j2534passthru.cpp \
    src/log-reader.cpp \
    src/logger.cpp \
    src/map-decl/map.cpp \
    src/widgets/hexEditor/qhexedit/chunks.cpp \
    src/widgets/hexEditor/qhexedit/commands.cpp \
    src/widgets/hexEditor/qhexedit/qhexedit.cpp \
    src/widgets/hexEditor/hexeditor.cpp \
    src/mainwindow.cpp \
#    src/libs/J2534.cpp \
    src/libs/ftdi.cpp \
    src/map-decl/scaling.cpp \
    src/serial/common-serial-interface.cpp \
    src/map-decl/submap.cpp \
    src/widgets/commParamWidget.cpp \
    src/widgets/custom_tablewidget.cpp \
    src/widgets/gauge_widget.cpp \
    src/widgets/maintoolbar.cpp \
    src/widgets/mapWidget/mapmodel.cpp \
    src/widgets/mapWidget/mapview.cpp \
    src/widgets/mapWidget/mapwidget.cpp

HEADERS  += src/mainwindow.h \
    src/abstract-memory.h \
    src/ecu-proto/ECU-interface.h \
    src/controller.h \
    src/ecu-proto/evoX-DMA.h \
    src/ecu-proto/jcsbanksDMA.h \
    src/ecu-proto/stockDMA.h \
    src/ecu/ecu-definition.h \
    src/ecu/evo7.h \
    src/ecu/rawstockmsg.h \
    src/fast-notation.h \
#    src/libs/J2534.h \
#    src/libs/j2534_tactrix.h \
    src/libs/ftdi.h \
    src/libs/ftdi_types.h \
    src/comm-device-interface/comm-device-interface.h \
    src/comm-device-interface/j2534-interface.h \
    src/comm-device-interface/op13.h \
    src/comm-device-interface/op20.h \
    src/libs/j2534passthru.h \
    src/log-reader.h \
    src/map-decl/map.h \
    src/map-decl/scaling.h \
    src/serial/common-serial-interface.h \
    src/serial/tactrix-serial.h \
    src/logger.h \
    src/enumdev.h \
    src/map-decl/submap.h \
    src/test-map.h \
    src/types.h \
    src/wideband/_wideband_input_device.h \
    src/wideband/tactrix_wb_input.h \
    src/widgets/commParamWidget.h \
    src/widgets/custom_tablewidget.h \
    src/widgets/gauge_widget.h \
    src/widgets/hexEditor/qhexedit/chunks.h \
    src/widgets/hexEditor/qhexedit/commands.h \
    src/widgets/hexEditor/qhexedit/qhexedit.h \
    src/widgets/hexEditor/hexeditor.h \
    src/widgets/maintoolbar.h \
    src/widgets/mapWidget/mapmodel.h \
    src/widgets/mapWidget/mapview.h \
    src/widgets/mapWidget/mapwidget.h
    #ecu/ecu_x_comm.h \
    #libs/libusb/include/libusb.h \
    #graph_logger.h \

FORMS    += mainwindow.ui

#DISTFILES +=

#CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT

#INCLUDEPATH += libs/libusb/include
#LIBS += -Llibs/libusb/MinGW64/dll -lusb-1.0
LIBS += -lSetupapi
LIBS += -ladvapi32
LIBS += -luser32
win32-g++ {
                QMAKE_CXXFLAGS  += -flto -funroll-loops
                QMAKE_CXXFLAGS  += -fforce-addr
                QMAKE_CXXFLAGS  += -m32 -Ofast -march=core2 -mtune=core2
                #QMAKE_CXXFLAGS  += -mfpmath=sse
                QMAKE_CXXFLAGS  += -msse4
#                LIBS += -L$$PWD/mingw-dll -lqwt
#                CONFIG(release, debug|release):QMAKE_LFLAGS_RELEASE += -static -static-libgcc
            }
win32-msvc {
                QMAKE_LFLAGS_RELEASE += /LTCG
                QMAKE_CXXFLAGS  += /O2
                QMAKE_CXXFLAGS  += /arch:AVX
                QMAKE_CXXFLAGS  += /Arch: SSE2
                QMAKE_CFLAGS  += /O2
                QMAKE_CFLAGS  += /arch:AVX
                QMAKE_CFLAGS  += /Arch: SSE2

#                LIBS += -L$$PWD/msvc-dll -lqwt
            }


DEFINES += QHEXEDIT_EXPORTS
#DEFINES += QHEXEDIT_IMPORTS

RESOURCES += \
    res.qrc
