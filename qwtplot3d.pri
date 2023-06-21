
#INCLUDEPATH += C:\Users\Limbo-s\Documents\GitHub\Evo-live-map\3rdparty\qwtplot3d\include
#INCLUDEPATH += D:\Users\Bimbo\Documents\GitHub\forELM\Evo-live-map\3rdparty\qwtplot3d\include

#CONFIG(release, release|debug){
#            win32-g++ {
##            LIBS += -LC:\Users\Limbo-s\Documents\GitHub\Evo-live-map\3rdparty\qwtplot3d\lib\x86\release -lqwtplot3d
#            LIBS += -LD:\Users\Bimbo\Documents\GitHub\forELM\Evo-live-map\3rdparty\qwtplot3d\lib\x86\release -lqwtplot3d

##            message("release mode")
#                        }
#                    }
#CONFIG(debug, release|debug){
#            win32-g++ {
##                LIBS += -LC:\Users\Limbo-s\Documents\GitHub\Evo-live-map\3rdparty\qwtplot3d\lib\x86\debug -lqwtplot3d
#                LIBS += -LD:\Users\Bimbo\Documents\GitHub\forELM\Evo-live-map\3rdparty\qwtplot3d\lib\x86\debug -lqwtplot3d
##            message("debug mode")
#                        }
#                }


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/3rdparty/qwtplot3d/lib/x86/release/ -lqwtplot3d
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/3rdparty/qwtplot3d/lib/x86/debug/ -lqwtplot3d

INCLUDEPATH += $$PWD/3rdparty/qwtplot3d/include
DEPENDPATH += $$PWD/3rdparty/qwtplot3d/include
