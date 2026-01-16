QT += core gui sql network widgets

CONFIG += c++17

TARGET = CommunityMedicalSystem
TEMPLATE = app

# 源文件
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    database/databasemanager.cpp \
    database/entities/patient.cpp \
    database/entities/doctor.cpp \
    database/models/patientmodel.cpp \
    addpatientdialog.cpp

# 头文件
HEADERS += \
    mainwindow.h \
    database/databasemanager.h \
    database/entities/patient.h \
    database/entities/doctor.h \
    database/models/patientmodel.h \
    addpatientdialog.h

FORMS += \
    mainwindow.ui

win32: CONFIG += console

# 包含路径
INCLUDEPATH += $$PWD
INCLUDEPATH += $$PWD/database
INCLUDEPATH += $$PWD/database/entities
INCLUDEPATH += $$PWD/database/models

# 调试/发布配置
CONFIG(debug, debug|release) {
    DEFINES += DEBUG_MODE
} else {
    DEFINES += QT_NO_DEBUG_OUTPUT
}

# 编译器选项
QMAKE_CXXFLAGS += -Wno-unused-parameter

win32 {
    DEPLOY_DIR = c:/Users/vergil/Desktop/CommunityMedicalSystem/可执行程序
    CONFIG(debug, debug|release) {
        EXE_PATH = $$OUT_PWD/debug/$$TARGET.exe
    } else {
        EXE_PATH = $$OUT_PWD/release/$$TARGET.exe
    }
    deploy.commands = $$quote(cmd /c if not exist \"$$DEPLOY_DIR\" mkdir \"$$DEPLOY_DIR\")
    deploy.commands += $$quote(\"$$[QT_HOST_BINS]/windeployqt.exe\" --dir \"$$DEPLOY_DIR\" --compiler-runtime \"$$EXE_PATH\")
    deploy.commands += $$quote(cmd /c copy /Y \"$$EXE_PATH\" \"$$DEPLOY_DIR\\\")
    deploy.depends = $$TARGET
    QMAKE_EXTRA_TARGETS += deploy
    POST_TARGETDEPS += deploy
}
