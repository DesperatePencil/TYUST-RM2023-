TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        Armor/ArmorBag.cpp \
        Armor/ArmorDetector.cpp \
        Armor/LightBag.cpp \
        CameraDriver/Open_camera.cpp \
        Communication/CRC.cpp \
        Communication/serial.cpp \
        ImgProcessing/imgprocessing.cpp \
        Solver/AngleSolver.cpp \
        Start/start.cpp \
        Tool/SVM.cpp \
        main.cpp

HEADERS += \
    Armor/Armor.h \
    CameraDriver/DVPcamera.h \
    CameraDriver/Open_camera.h \
    Communication/serial.h \
    ImgProcessing/imgprocessing.h \
    Info/Info.h \
    Solver/Solver.h \
    Start/start.h \
    fuhuo.h \
    fuhuo.h
INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv
                /usr/local/include/opencv2

LIBS += /usr/local/lib/*.so.*

LIBS +=-lpthread

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../相机驱动/DVP2-Linux64_2018.11.2.tar/DVP2-Linux64/release/ -lhzd
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../相机驱动/DVP2-Linux64_2018.11.2.tar/DVP2-Linux64/debug/ -lhzd
else:unix: LIBS += -L$$PWD/../相机驱动/DVP2-Linux64_2018.11.2.tar/DVP2-Linux64/ -lhzd

INCLUDEPATH += $$PWD/../相机驱动/DVP2-Linux64_2018.11.2.tar/DVP2-Linux64
DEPENDPATH += $$PWD/../相机驱动/DVP2-Linux64_2018.11.2.tar/DVP2-Linux64

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../相机驱动/DVP2-Linux64_2018.11.2.tar/DVP2-Linux64/release/ -ldvp
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../相机驱动/DVP2-Linux64_2018.11.2.tar/DVP2-Linux64/debug/ -ldvp
else:unix: LIBS += -L$$PWD/../相机驱动/DVP2-Linux64_2018.11.2.tar/DVP2-Linux64/ -ldvp

INCLUDEPATH += $$PWD/../相机驱动/DVP2-Linux64_2018.11.2.tar/DVP2-Linux64
DEPENDPATH += $$PWD/../相机驱动/DVP2-Linux64_2018.11.2.tar/DVP2-Linux64

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../相机驱动/DVP2-Linux64_2018.11.2.tar/DVP2-Linux64/release/ -lD3tKit
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../相机驱动/DVP2-Linux64_2018.11.2.tar/DVP2-Linux64/debug/ -lD3tKit
else:unix: LIBS += -L$$PWD/../相机驱动/DVP2-Linux64_2018.11.2.tar/DVP2-Linux64/ -lD3tKit

INCLUDEPATH += $$PWD/../相机驱动/DVP2-Linux64_2018.11.2.tar/DVP2-Linux64
DEPENDPATH += $$PWD/../相机驱动/DVP2-Linux64_2018.11.2.tar/DVP2-Linux64
