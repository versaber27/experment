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
