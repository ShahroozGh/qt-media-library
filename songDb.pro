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
    songdatafetcher.cpp \
    equalizerdialog.cpp \
    customqdial.cpp \
    customqslider.cpp \
    Visualizers/abstractvisualizer.cpp \
    Visualizers/polyvisualizer.cpp \
    Visualizers/polyvisualizeroptionsdialog.cpp \
    Visualizers/visualizer.cpp \
    Visualizers/visualizerdialog.cpp

HEADERS  += mainwindow.h \
    AudioSystem.h \
    SpectrumVisualizer.h \
    prefsdialog.h \
    addartworkdialog.h \
    QCategoryFilterProxyModel.h \
    QCardStyledItemDelegate.h \
    customtitlebar.h \
    customcentralwidget.h \
    songdatafetcher.h \
    equalizerdialog.h \
    customqdial.h \
    customqslider.h \
    Visualizers/abstractvisualizer.h \
    Visualizers/polyvisualizer.h \
    Visualizers/polyvisualizeroptionsdialog.h \
    Visualizers/visualizer.h \
    Visualizers/visualizerdialog.h


FORMS    += mainwindow.ui \
    prefsdialog.ui \
    addartworkdialog.ui \
    equalizerdialog.ui \
    Visualizers/polyvisualizeroptionsdialog.ui \
    Visualizers/visualizerdialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/lib/' -lfmod_vc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/lib/' -lfmod_vc

INCLUDEPATH += $$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/inc'
DEPENDPATH += $$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/inc'

DISTFILES += \
    mainWindowStyleSheet.css \
    ../songPro/themes/lightTheme.css \
    Documents_Example/songPro/artwork/default.png \
    Documents_Example/songPro/resources/backButton.png \
    Documents_Example/songPro/resources/forwardButton.png \
    Documents_Example/songPro/resources/pauseButton.png \
    Documents_Example/songPro/resources/playButton.png \
    Images/BackIcon.png \
    Images/BackIcon50.png \
    Images/ForwardIcon.png \
    Images/ForwardIcon50.png \
    Images/PauseIcon.png \
    Images/PauseIcon50.png \
    Images/PlayIcon.png \
    Images/PlayIcon50.png \
    Documents_Example/songPro/lists/exampleSongList.txt \
    README.md
#----new
#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/lib/' -lfmodL_vc
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/lib/' -lfmodL_vcd
#else:unix: LIBS += -L$$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/lib/' -lfmodL_vc

#INCLUDEPATH += $$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/inc'
#DEPENDPATH += $$PWD/'../../../../Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/lowlevel/inc'
