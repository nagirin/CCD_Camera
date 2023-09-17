#-------------------------------------------------
#
# Project created by QtCreator 2020-09-02T22:09:28
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

RC_ICONS = logo.ico

TARGET = Camera
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        widget.cpp \
    appinit.cpp \
    mymessagebox.cpp\
    ImageWidget.cpp\



HEADERS += \
        widget.h \
    appinit.h \
    mymessagebox.h\
    ImageWidget.h\


FORMS += \
        widget.ui \
    mymessagebox.ui

RESOURCES += \
    qss.qrc

include(tools/opencv.pri)# opencv 动态链接(新建项目加这段)


