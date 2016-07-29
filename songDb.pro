#-------------------------------------------------
#
# Project created by QtCreator 2015-05-18T15:48:02
#
#-------------------------------------------------

QT       += core gui
QT         += multimedia
QT += network


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = songDb
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    AudioSystem.cpp \
    SpectrumVisualizer.cpp \
    prefsdialog.cpp \
    addartworkdialog.cpp \
    QCategoryFilterProxyModel.cpp \
    QCardStyledItemDelegate.cpp \
    customtitlebar.cpp \
    customcentralwidget.cpp \
    songdatafetcher.cpp

HEADERS  += mainwindow.h \
    AudioSystem.h \
    SpectrumVisualizer.h \
    prefsdialog.h \
    addartworkdialog.h \
    QCategoryFilterProxyModel.h \
    QCardStyledItemDelegate.h \
    customtitlebar.h \
    customcentralwidget.h \
    songdatafetcher.h


FORMS    += mainwindow.ui \
    prefsdialog.ui \
    addartworkdialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/lib/' -lfmod_vc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/lib/' -lfmod_vc

INCLUDEPATH += $$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/inc'
DEPENDPATH += $$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/inc'

DISTFILES += \
    mainWindowStyleSheet.css \
    ../songPro/themes/lightTheme.css
#----new
#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/lib/' -lfmodL_vc
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/lib/' -lfmodL_vcd
#else:unix: LIBS += -L$$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/lib/' -lfmodL_vc

#INCLUDEPATH += $$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/inc'
#DEPENDPATH += $$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/inc'
