#-------------------------------------------------
#
# Project created by QtCreator 2016-08-28T09:39:21
#
#-------------------------------------------------

CONFIG += c++11
#CONFIG += qwt
QT     += core gui xml serialport #datavisualization
#widgets

CONFIG(debug, debug|release) {
    VARIANT = debug
} else {
    VARIANT = release
}

#include(qwtplot3d.pri)
include(versionCtrl.pri)
# копирует заданные файлы в каталог назначения
include(copyXML.pri)
include(optimizer.pri)
#include(qwtplot3d.pri)
#include(qwtplot3d.pri)

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
DEFINES += QT_DEPRECATED_WARNINGS

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

TARGET = livemap
TEMPLATE = app

SOURCES += src/main.cpp\
    3rdparty/qhexedit2/src/chunks.cpp \
    3rdparty/qhexedit2/src/commands.cpp \
    3rdparty/qhexedit2/src/qhexedit.cpp \
    src/DMA-proto/DMA-proto.cpp \
    src/DMA-proto/proto-manager.cpp \
#    src/Flasher.cpp \
    src/ECU-model/ecumodel.cpp \
    src/ECU-model/evo7-ecumodel.cpp \
    src/abstract-memory.cpp \
    src/comm-device-interface/devicemanager.cpp \
    src/comm-device-interface/comm-device-interface.cpp \
    src/comm-device-interface/ftdi-comm.cpp \
    src/DMA-proto/evoX-DMA.cpp \
    src/DMA-proto/jcsbanksDMA.cpp \
    src/DMA-proto/stockDMA.cpp \
    src/comm-device-interface/j2534-comm.cpp \
    src/comm-device-interface/op20.cpp \
    src/comm-device-interface/serial-comm.cpp \
    src/ecu/ecu.cpp \
    src/ecu/mutparam.cpp \
    src/ecu/rammut.cpp \
#    src/widgets/3D/SurfaceGraph.cpp \
#    src/widgets/3D/s3dplot.cpp \
#    src/widgets/3D/s3dscatter.cpp \
#    src/widgets/3D/scatterdatamodifier.cpp \
    src/widgets/Patcher/patcher.cpp \
#    src/widgets/analizer.cpp \
    src/widgets/ecuManager.cpp \
    src/wideband/op20wb.cpp \
    src/DMA-proto/pollhelper.cpp \
    src/deviceNativeFilter.cpp \
    src/ecu/ecu-definition.cpp \
    src/fast-notation.cpp \
    src/libs/j2534passthru.cpp \
    src/map-decl/map.cpp \
    src/wideband/aemProto.cpp \
    src/wideband/commdevicewb-interface.cpp \
    src/wideband/innoProto.cpp \
    src/wideband/plxProto.cpp \
    src/wideband/serialwb.cpp \
    src/wideband/wb-manager.cpp \
    src/wideband/wb-proto.cpp \
    src/widgets/hexEditor/hexeditor.cpp \
    src/mainwindow.cpp \
#    src/libs/J2534.cpp \
    src/libs/ftdi.cpp \
    src/map-decl/scaling.cpp \
    src/map-decl/submap.cpp \
    src/widgets/commParamWidget.cpp \
    src/widgets/gauge_widget.cpp \
    src/widgets/loggermanager.cpp \
    src/widgets/mapManager/mapmanager.cpp \
    src/widgets/mapWidget/mapmodel.cpp \
    src/widgets/mapWidget/mapview.cpp \
    src/widgets/mapWidget/mapwidget.cpp \
    src/xmlmanager.cpp

HEADERS  += src/mainwindow.h \
    3rdparty/qhexedit2/src/chunks.h \
    3rdparty/qhexedit2/src/commands.h \
    3rdparty/qhexedit2/src/qhexedit.h \
    src/DMA-proto/DMA-proto.h \
    src/DMA-proto/proto-manager.h \
#    src/Flasher.h \
    src/ECU-model/ecumodel.h \
    src/ECU-model/evo7-ecumodel.h \
    src/abstract-memory.h \
    src/comm-device-interface/devicemanager.h \
    src/comm-device-interface/ftdi-comm.h \
    src/comm-device-interface/j2534-comm.h \
    src/comm-device-interface/serial-comm.h \
    src/ecu/ecu.h \
    src/ecu/mapDefinition.h \
    src/ecu/mutparam.h \
    src/ecu/rammut.h \
#    src/widgets/3D/SurfaceGraph.h \
#    src/widgets/3D/s3dplot.h \
#    src/widgets/3D/s3dscatter.h \
#    src/widgets/3D/scatterdatamodifier.h \
    src/widgets/Patcher/patcher.h \
#    src/widgets/analizer.h \
    src/widgets/ecuManager.h \
    src/wideband/op20wb.h \
    src/DMA-proto/pollhelper.h \
    src/DMA-proto/evoX-DMA.h \
    src/DMA-proto/jcsbanksDMA.h \
    src/DMA-proto/stockDMA.h \
    src/deviceNativeFilter.h \
    src/ecu/ecu-definition.h \
    src/fast-notation.h \
#    src/libs/J2534.h \
#    src/libs/j2534_tactrix.h \
    src/libs/ftdi.h \
    src/libs/ftdi_types.h \
    src/comm-device-interface/comm-device-interface.h \
    src/comm-device-interface/op20.h \
    src/libs/j2534passthru.h \
    src/map-decl/map.h \
    src/map-decl/scaling.h \
    src/map-decl/submap.h \
    src/test-map.h \
    src/types.h \
    src/wideband/aemProto.h \
    src/wideband/commdevicewb-interface.h \
    src/wideband/innoProto.h \
    src/wideband/plxProto.h \
    src/wideband/serialwb.h \
    src/wideband/wb-manager.h \
    src/wideband/wb-proto.h \
    src/widgets/commParamWidget.h \
    src/widgets/gauge_widget.h \
    src/widgets/hexEditor/hexeditor.h \
    src/widgets/loggermanager.h \
    src/widgets/mapManager/mapmanager.h \
    src/widgets/mapWidget/mapmodel.h \
    src/widgets/mapWidget/mapview.h \
    src/widgets/mapWidget/mapwidget.h \
    src/xmlmanager.h
    #ecu/ecu_x_comm.h \

FORMS    += mainwindow.ui \
    src/widgets/Patcher/patcher.ui \
    src/widgets/mapManager/mapmanager.ui

LIBS += -lSetupapi
LIBS += -ladvapi32
LIBS += -luser32

DEFINES += QHEXEDIT_EXPORTS
#DEFINES += QHEXEDIT_IMPORTS

RESOURCES += \
    res.qrc

DISTFILES += \
    xdf/80700010-jcsbanks.xdf \
    xdf/80700010_DMA-stockDMA-byNanner55.xdf \
    xdf/88590015-stockDMA-byNanner55.xdf \
    xdf/90550001-jcsbanks.xdf \
    xdf/90550001-stockDMA-byNanner55.xdf \
    xml/80700010_elm.xml \
    xml/88570008.xml \
    xml/88570008_elm.xml \
    xml/88582714_elm.xml \
    xml/88592714_elm.xml \
    xml/88592715_elm.xml \
    xml/90550001.txt \
    xml/90550001_elm.xml \
    xml/90552701_elm.xml \
    xml/96262709-TephraMod-dma.xml \
    xml/96532706-TephraMod-l2r99gst-dma.xml \
    xml/96533706-TephraMod-l2r99gst-dma.xml

#CONFIG(release, debug|release): LIBS += -L$$PWD/3rdparty/qwtplot3d/lib/x86/release/libqwtplot3d.a
#CONFIG(debug, debug|release): LIBS += -L$$PWD/3rdparty/qwtplot3d/lib/x86/debug/libqwtplot3d.a

#CONFIG(release, debug|release): LIBS += -L$$PWD/3rdparty/qwtplot3d/lib/x86/release/qwtplot3d.lib
#CONFIG(debug, debug|release): LIBS += -L$$PWD/3rdparty/qwtplot3d/lib/x86/debug/qwtplot3d.lib

#INCLUDEPATH += $$PWD/3rdparty/qwtplot3d/include
##DEPENDPATH += $$PWD/3rdparty/qwtplot3d/include

