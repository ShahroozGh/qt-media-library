#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QAbstractItemView>

#include <QStandardItemModel>
#include <QStandardItem>
#include <QSortFilterProxyModel>

#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>

#include <QStandardPaths>
#include <QFileInfo>

#include <QMenuBar>

#include <QPixmap>

#include <QInputDialog>

#include <QtMultimedia>

#include <QTimer>

#include <QButtonGroup>

#include <QGraphicsDropShadowEffect>

#include "QCardStyledItemDelegate.h"

#include "QCategoryFilterProxyModel.h"

#include "prefsdialog.h"

#include "addartworkdialog.h"

#include "AudioSystem.h"

#include "SpectrumVisualizer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void changeStyleSheet(QString styleSheet);

protected:
    //Overrided functions
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private slots:
    void updateLoop();

    void on_pushButtonAdd_clicked();

    void on_pushButtonDelete_clicked();

    void on_pushButtonSearch_clicked();

    void on_pushButtonSort_clicked();

    void on_lineEditSearch_textChanged(const QString &arg1);

    void on_lineEditSearch_editingFinished();

    void on_actionSave_as_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void slot_selectionChanged();

    void slot_itemChanged(QStandardItem* item);

    void on_addArtButton_clicked();

    void on_actionNew_triggered();

    //void on_pausePlayButton_toggled(bool checked);

    void on_treeView_doubleClicked(const QModelIndex &index);

    void on_pausePlayButton_clicked(bool checked);

    void on_volumeSlider_sliderMoved(int position);

    void on_trackSlider_sliderMoved(int position);

    //void updatePosition(qint64 position);

    //void updateDuration(qint64 duration);

    //void slot_mediaStatusChanged(QMediaPlayer::MediaStatus stat);

    void on_forwardButton_clicked();

    void on_backButton_clicked();

    void on_linkMusicButton_clicked();

    void on_trackSlider_actionTriggered(int action);

    void on_volumeSlider_actionTriggered(int action);

    void on_actionPreferences_triggered();

    void on_actionFind_album_art_triggered();

    void on_pushButtonPg1_clicked();

    void on_pushButtonPg2_clicked();

    void on_albumListView_doubleClicked(const QModelIndex &index);

    void cat_button_group_clicked(int id);

    void on_actionLink_Music_File_triggered();

    void on_actionAdd_art_form_file_triggered();

    void on_closeB_clicked();

    void on_minMaxB_clicked();

    void on_minimizeB_clicked();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *songItemModel;          //Data struct that holds song list data for display

    QString currentUser, currentSongPath;
    QMediaPlayer *player;
    QTimer *updateTimer;
    bool unsavedChanges, isLoading;
    QButtonGroup* categorySelectorGroup;

    QMenuBar* createMenuBar();

    //Audio System
    AudioSystem fmodSys;

    SpectrumVisualizer visualizer;

    // Returns stored album art if it exists
    QPixmap getAlbumArt(QString albumTitle);

    bool validateSongList(QString fileName);
    void addFile(std::string path);
    void writeSaveFile(QFile &file);
    void changesMade();
    void changesSaved();

    void setStyleSheets();
    void setDropShadowProperties(int blurRadius, QColor color);
    void enableDropShadows(bool enabled);


    void updateCurrentSongInfoDisplay();

    void useCustomFrame(bool use);




};

#endif // MAINWINDOW_H
