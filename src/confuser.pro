QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = confuser
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp \
           confmattab.cpp \
           confmattabcmds.cpp \
           mainsettings.cpp \
    preferencesdialog.cpp

HEADERS  += mainwindow.h \
            confmattab.h \
            confmattabcmds.h \
            mainsettings.h \
    preferencesdialog.h

FORMS    += mainwindow.ui \
    preferencesdialog.ui

RESOURCES += \
    confuser.qrc
