#-------------------------------------------------
#
# Project created by QtCreator 2021-03-27T20:50:50
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HealthyBed
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS



# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        widget.cpp \
    dialog.cpp \
    dialog2.cpp \
    form.cpp

HEADERS  += widget.h \
    dialog.h \
    dialog2.h \
    form.h \
    release/moc_predefs.h \
    dialog.h \
    dialog2.h \
    form.h \
    ui_dialog.h \
    ui_dialog2.h \
    ui_form.h \
    ui_widget.h \
    widget.h


FORMS    += widget.ui \
    dialog.ui \
    dialog2.ui \
    form.ui




DISTFILES += \
    resource/style.qss

RESOURCES += \
    resource/resource.qrc


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/lib/ -lQt5Mqtt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/lib/ -lQt5Mqttd

INCLUDEPATH += $$PWD/include
DEPENDPATH += $$PWD/include
