QT          += core gui widgets opengl

TEMPLATE    = app
CONFIG      += c++17
INCLUDEPATH += ./include
INCLUDEPATH += ./Matrix4x4/
INCLUDEPATH += .
TARGET      = GameEngine

FORMS += \
    mainwindow.ui

HEADERS += \
    PhysXClasses.h \
    barycentriccalc.h \
    camera.h \
    data.h \
    dialoguecontroller.h \
    gameobject.h \
    graphicscomponent.h \
    input.h \
    inputcomponent.h \
    light.h \
    logger.h \
    mainwindow.h \
    objectmesh.h \
    physicsball.h \
    physicsdebuglines.h \
    physicsmanager.h \
    renderwindow.h \
    shader.h \
    soundcomponent.h \
    texture.h \
    trianglesurface.h \
    vertex.h \
    visualobject.h \
    xyz.h

SOURCES += \
    barycentriccalc.cpp \
    camera.cpp \
    data.cpp \
    dialoguecontroller.cpp \
    gameobject.cpp \
    graphicscomponent.cpp \
    inputcomponent.cpp \
    light.cpp \
    logger.cpp \
    main.cpp \
    mainwindow.cpp \
    objectmesh.cpp \
    physicsball.cpp \
    physicsdebuglines.cpp \
    physicsmanager.cpp \
    renderwindow.cpp \
    shader.cpp \
    soundcomponent.cpp \
    texture.cpp \
    trianglesurface.cpp \
    vertex.cpp \
    visualobject.cpp \
    xyz.cpp

DISTFILES += \
    init.lua \
    phongshader.frag \
    phongshader.vert \
    plainshader.frag \
    plainshader.vert \
    textureshader.frag \
    textureshader.vert


CONFIG(debug, debug|release){
mac {
    LIBS += -framework OpenAL
}

unix|win32: LIBS *= -L$$PWD/libs/AL/bin/Win64/ -lOpenAL32 \
                    *= -L$$PWD/libs/DialogueDLL/x64/Debug/ -lDialogueDLL \
                    *= -L$$PWD/libs/lua/ liblua54.a

INCLUDEPATH += $$PWD/libs/AL/bin/Win64 \
            += $$PWD/libs/DialogueDLL/x64/Debug \
            += $$PWD/libs/lua
DEPENDPATH += $$PWD/libs/AL/bin/Win64 \
            += $$PWD/libs/DialogueDLL/x64/Debug \
            += $$PWD/libs/lua

INCLUDEPATH += $$PWD/include/AL \
            += $$PWD/include/DialogueDLL \
            += $$PWD/include/lua

}else{
mac {
    LIBS += -framework OpenAL
}

unix|win32: LIBS *= -L$$PWD/libs/AL/bin/Win64/ -lOpenAL32 \
                    *= -L$$PWD/libs/DialogueDLL/x64/Debug/ -lDialogueDLL \
                    *= -L$$PWD/libs/lua/ liblua54.a

INCLUDEPATH += $$PWD/libs/AL/bin/Win64 \
            += $$PWD/libs/DialogueDLL/x64/Debug \
            += $$PWD/libs/lua
DEPENDPATH += $$PWD/libs/AL/bin/Win64 \
            += $$PWD/libs/DialogueDLL/x64/Debug \
            += $$PWD/libs/lua

INCLUDEPATH += $$PWD/include/AL \
            += $$PWD/include/DialogueDLL \
            += $$PWD/include/lua
}


win32{

INCLUDEPATH += $$PWD/Libs/PhysX4/include \
               $$PWD/Libs/PhysX4/pxshared/include \
               $$PWD/Libs/PhysX4/source/foundation/include


DEPENDPATH += $$PWD/Libs/PhysX4/include


        CONFIG(debug, debug|release){
         LIBS += -L$$PWD/Libs/PhysX4/bin/debug -lPhysX_64
         LIBS += -L$$PWD/Libs/PhysX4/bin/debug -lPhysXCommon_64
         LIBS += -L$$PWD/Libs/PhysX4/bin/debug -lPhysXFoundation_64
         LIBS += -L$$PWD/Libs/PhysX4/bin/debug -lPhysXPvdSDK_static_64
         LIBS += -L$$PWD/Libs/PhysX4/bin/debug -lSimulationController_static_64
         LIBS += -L$$PWD/Libs/PhysX4/bin/debug -lPhysXTask_static_64
         LIBS += -L$$PWD/Libs/PhysX4/bin/debug -lSceneQuery_static_64
         LIBS += -L$$PWD/Libs/PhysX4/bin/debug -lPhysXExtensions_static_64
         LIBS += -L$$PWD/Libs/PhysX4/bin/debug -lPhysXCooking_64
         LIBS += -L$$PWD/Libs/PhysX4/bin/debug -lPhysXCharacterKinematic_static_64
         LIBS += -L$$PWD/Libs/PhysX4/bin/debug -lPhysXVehicle_static_64
        }else{
         LIBS += -L$$PWD/Libs/PhysX4/bin/release -lPhysX_64
         LIBS += -L$$PWD/Libs/PhysX4/bin/release -lPhysXCommon_64
         LIBS += -L$$PWD/Libs/PhysX4/bin/release -lPhysXFoundation_64
         LIBS += -L$$PWD/Libs/PhysX4/bin/release -lPhysXPvdSDK_static_64
         LIBS += -L$$PWD/Libs/PhysX4/bin/release -lSimulationController_static_64
         LIBS += -L$$PWD/Libs/PhysX4/bin/release -lPhysXTask_static_64
         LIBS += -L$$PWD/Libs/PhysX4/bin/release -lSceneQuery_static_64
         LIBS += -L$$PWD/Libs/PhysX4/bin/release -lPhysXExtensions_static_64
         LIBS += -L$$PWD/Libs/PhysX4/bin/release -lPhysXCooking_64
         LIBS += -L$$PWD/Libs/PhysX4/bin/release -lPhysXCharacterKinematic_static_64
         LIBS += -L$$PWD/Libs/PhysX4/bin/release -lPhysXVehicle_static_64
        }

}
