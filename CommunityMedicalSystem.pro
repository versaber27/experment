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
    database/entities/visit.cpp \
    database/entities/prescription.cpp \
    database/entities/prescriptionmedicine.cpp \
    database/models/patientmodel.cpp \
    database/models/visitmodel.cpp \
    database/models/prescriptionmodel.cpp \
    addpatientdialog.cpp

# 头文件
HEADERS += \
    mainwindow.h \
    database/databasemanager.h \
    database/entities/patient.h \
    database/entities/doctor.h \
    database/entities/visit.h \
    database/entities/prescription.h \
    database/entities/prescriptionmedicine.h \
    database/models/patientmodel.h \
    database/models/visitmodel.h \
    database/models/prescriptionmodel.h \
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
    QMAKE_POST_LINK += cmd /c if not exist \"$$DEPLOY_DIR\" mkdir \"$$DEPLOY_DIR\"
    QMAKE_POST_LINK += \"$$[QT_HOST_BINS]/windeployqt.exe\" --dir \"$$DEPLOY_DIR\" --compiler-runtime \"$(DESTDIR_TARGET)\"
    QMAKE_POST_LINK += copy /Y \"$(DESTDIR_TARGET)\" \"$$DEPLOY_DIR\\\"
}
