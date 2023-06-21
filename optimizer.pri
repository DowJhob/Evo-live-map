
CONFIG(release, release|debug){
            win32-g++ {
                            QMAKE_CXXFLAGS  += -flto -funroll-loops
                            CONFIG += -static
                            message("release mode")
                        }
                    }
win32-g++ {

                QMAKE_CXXFLAGS  += -fforce-addr
                QMAKE_CXXFLAGS  += -m32 -Ofast -march=core2 -mtune=core2
                QMAKE_CXXFLAGS  += -mfpmath=sse
                QMAKE_CXXFLAGS  += -msse4
##                CONFIG(release, debug|release):QMAKE_LFLAGS_RELEASE += -static -static-libgcc
            }
win32-msvc {
#                QMAKE_LFLAGS_RELEASE += /LTCG
                QMAKE_CXXFLAGS  += /O2
#                QMAKE_CXXFLAGS  += /arch:AVX
#                QMAKE_CXXFLAGS  += /Arch: SSE2
                QMAKE_CFLAGS  += /O2
#                QMAKE_CFLAGS  += /arch:AVX
#                QMAKE_CFLAGS  += /Arch: SSE2
            }
