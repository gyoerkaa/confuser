QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = confuser
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp \
           exportoptionsdialog.cpp \
           confmattab.cpp \
           confmattabcmds.cpp \
           mainsettings.cpp

HEADERS  += mainwindow.h \
            exportoptionsdialog.h \
            confmattab.h \
            confmattabcmds.h \
            mainsettings.h

FORMS    += mainwindow.ui \
            exportoptionsdialog.ui

RESOURCES += \
    confuser.qrc
