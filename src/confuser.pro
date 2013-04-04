QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = confuser
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp \
           confmattab.cpp \
           mainsettings.cpp \
    preferencesdialog.cpp \
    confmatmodel.cpp \
    confmatcmds.cpp \
    confmat.cpp \
    latexcode.cpp \
    texcodedialog.cpp

HEADERS  += mainwindow.h \
            confmattab.h \
            mainsettings.h \
    preferencesdialog.h \
    confmatmodel.h \
    confmatcmds.h \
    confmat.h \
    latexcode.h \
    texcodedialog.h

FORMS    += mainwindow.ui \
    preferencesdialog.ui \
    texcodedialog.ui

RESOURCES += \
    confuser.qrc
