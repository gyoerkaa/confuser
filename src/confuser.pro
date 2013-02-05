QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = confuser
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp \
           exportoptionsdialog.cpp \
           confmatwidget.cpp

HEADERS  += mainwindow.h \
            exportoptionsdialog.h \
            confmatwidget.h

FORMS    += mainwindow.ui \
            exportoptionsdialog.ui

RESOURCES += \
    confuser.qrc
