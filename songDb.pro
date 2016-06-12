#-------------------------------------------------
#
# Project created by QtCreator 2015-05-18T15:48:02
#
#-------------------------------------------------

QT       += core gui
QT         += multimedia


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = songDb
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    AudioSystem.cpp

HEADERS  += mainwindow.h \
    AudioSystem.h


FORMS    += mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/lib/' -lfmod_vc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/lib/' -lfmod_vcd

INCLUDEPATH += $$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/inc'
DEPENDPATH += $$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/inc'
