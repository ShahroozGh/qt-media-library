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

#include <QPixmap>

#include <QInputDialog>

#include <QtMultimedia>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
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

    void updatePosition(qint64 position);

    void updateDuration(qint64 duration);

    void slot_mediaStatusChanged(QMediaPlayer::MediaStatus stat);

    void on_forwardButton_clicked();

    void on_backButton_clicked();

    void on_linkMusicButton_clicked();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *songItemModel;          //Data struct that holds song list data for display
    QString currentUser, currentSongPath;
    QMediaPlayer *player;
    bool unsavedChanges, isLoading;

    void writeSaveFile(QFile &file);
    void changesMade();
    void changesSaved();


};

#endif // MAINWINDOW_H
