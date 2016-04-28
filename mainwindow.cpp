#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    unsavedChanges = true;
    this->setWindowTitle("Untitled*");

    ui->pausePlayButton->setCheckable(true);

    //songPro folder under users documents folder
    QIcon forward(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\resources\\forwardButton.png");
    QIcon back(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\resources\\backButton.png");
    ui->forwardButton->setIcon(forward);
    ui->backButton->setIcon(back);
    //ui->pausePlayButton->setIcon();

    QIcon* playPause = new QIcon();

    playPause->addPixmap(QPixmap(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\resources\\pauseButton.png"),QIcon::Normal,QIcon::On);
    playPause->addPixmap(QPixmap(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\resources\\playButton.png"),QIcon::Normal,QIcon::Off);
    ui->pausePlayButton->setIcon(*playPause);

    player = new QMediaPlayer;

    ui->volumeSlider->setMaximum(100);

    ui->lcdNumber->setPalette(Qt::black);

    isLoading = false;

    //Create new item model
    //songItemModel = new QStandardItemModel(3,4,this);
    songItemModel = new QStandardItemModel(0,0,this);
/*
    //Populate inital songs (temp)
    for(int row = 0; row < 3; row++)
    {
        for(int col = 0; col < 4; col++)
        {
            QStandardItem *item = new QStandardItem(QString("row %0, column %1").arg(row).arg(col));
            songItemModel->setItem(row, col, item);
        }
    }
*/
    //set model for tree view and labels
    ui->treeView->setModel(songItemModel);
    ui->treeView->setSortingEnabled(true);
    songItemModel->setHorizontalHeaderLabels(QStringList() << "Title" << "Artist" << "Album" << "Genre" << "Path");

   QPixmap pic(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\artwork\\default.jpg");
   ui->albumCoverLabel->setPixmap(pic.scaled(300,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    //ui->treeView->editTriggers()
   //Set up signal/slot to detect when selection changes
   QObject::connect(ui->treeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(slot_selectionChanged()));
   QObject::connect(songItemModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slot_itemChanged(QStandardItem*)));
   QObject::connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(updatePosition(qint64)));
   QObject::connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(updateDuration(qint64)));
   QObject::connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(slot_mediaStatusChanged(QMediaPlayer::MediaStatus)));

}

MainWindow::~MainWindow()
{
    delete ui;
}
//-----------------------------------------------------------

//Add song Button
void MainWindow::on_pushButtonAdd_clicked()
{
    //Get text from lineEdit txt boxes
    QString title = ui->lineEditTitle->text();
    QString artist = ui->lineEditArtist->text();
    QString album = ui->lineEditAlbum->text();
    QString genre = ui->lineEditGenre->text();

    //Check if title is empty
    if (!title.isEmpty())
    {
        qDebug() << "empty";
    }

    //Default strings
    if (title.isEmpty()){title = "untitled";}
    if (artist.isEmpty()){artist = "unknown";}
    if (album.isEmpty()){album = "unknown";}
    if (genre.isEmpty()){genre = "unknown";}

    //Create item for each
    QStandardItem *titleItem = new QStandardItem(title);
    QStandardItem *artistItem = new QStandardItem(artist);
    QStandardItem *albumItem = new QStandardItem(album);
    QStandardItem *genreItem = new QStandardItem(genre);
    QStandardItem *pathItem = new QStandardItem("");

    //Add items to a list (of 3 QStanItems)
    QList<QStandardItem*> list;
    list << titleItem << artistItem << albumItem << genreItem << pathItem;

    //Insert list of items as a row
    songItemModel->insertRow(0, list);

    //Clear text boxes
    ui->lineEditTitle->clear();
    ui->lineEditArtist->clear();
    ui->lineEditAlbum->clear();
    ui->lineEditGenre->clear();

    //Resize columns
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->resizeColumnToContents(1);
    ui->treeView->resizeColumnToContents(2);
    ui->treeView->resizeColumnToContents(3);

    changesMade();
}
//-----------------------------------------------------------
//Delete Button
void MainWindow::on_pushButtonDelete_clicked()
{
    //get index of selected row and remove
    int row = ui->treeView->currentIndex().row();
    songItemModel->removeRow(row);

    //Resize
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->resizeColumnToContents(1);
    ui->treeView->resizeColumnToContents(2);
    ui->treeView->resizeColumnToContents(3);

    //check if album art should be deleted

    changesMade();

}
//-----------------------------------------------------------
void MainWindow::on_pushButtonSearch_clicked()
{

    qDebug() << "clicked";
    ui->lineEditSearch->clear();

    //temp
    /*
    ui->treeView->setModel(songItemModel);
    qDebug() << "Unselected";
    ui->lineEditSearch->clear();
    QObject::connect(ui->treeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(slot_selectionChanged()));
    */
    //end-temp

    QString test = "he'l;lo my name is 'b.//'ob";

    qDebug() << test.remove(QRegExp(QString::fromUtf8("[ -`~!@#$%^&*()_—+=|:;<>«»,.?/{}\'\"\\\[\\\]\\\\]")));

}
//-----------------------------------------------------------
void MainWindow::on_pushButtonSort_clicked()
{
    ui->treeView->setModel(songItemModel);
}
//-----------------------------------------------------------
void MainWindow::on_lineEditSearch_textChanged(const QString &arg1)
{
    //Create a proxymodel that can be modified w/out changing og model
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(songItemModel);

    ui->treeView->setModel(proxyModel);

    //Filter
    proxyModel->setFilterRegExp(QRegExp("^"+(ui->lineEditSearch->text()),Qt::CaseInsensitive,QRegExp::RegExp));
    proxyModel->setFilterKeyColumn(0);
}
//-----------------------------------------------------------
void MainWindow::on_lineEditSearch_editingFinished()
{
    //When search finished (unfocused) go back to og model

    ui->treeView->setModel(songItemModel);
    qDebug() << "Unselected";
    ui->lineEditSearch->clear();
    QObject::connect(ui->treeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(slot_selectionChanged()));

}
//-----------------------------------------------------------
void MainWindow::on_actionSave_as_triggered()
{
    QString name = QFileDialog::getSaveFileName(this, tr("Save File"), QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\lists" ,tr("Text Files (*.txt)"));

    if (!name.isEmpty()){
        QFile file(name);
        if (!file.open(QIODevice::WriteOnly)){
            //Error
        }
        else//Write text
        {
            MainWindow::writeSaveFile(file);

            //Get name of file and setTitle to this
            currentUser = QFileInfo(name).baseName();
            this->setWindowTitle(currentUser);
            changesSaved();

        }

    }

    //Make artwork folder
    QDir artFolder(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\artwork\\" + QFileInfo(name).baseName());

    if (!artFolder.exists())
    {
        artFolder.mkpath(".");
    }
}
//-----------------------------------------------------------
void MainWindow::on_actionSave_triggered()
{
    //Check if file DNE, if so call saveAs func
    QFile file(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\lists\\" + currentUser + ".txt");

    if (!file.open(QIODevice::WriteOnly)){
        //Error
    }
    else//Write text
    {
        MainWindow::writeSaveFile(file);


    }

    changesSaved();

}

//-----------------------------------------------------------
void MainWindow::writeSaveFile(QFile &file)
{
    QTextStream output(&file);

    for (int row = 0; row < songItemModel->rowCount(); row++)
    {
        output << songItemModel->item(row,0)->text() << ";"
               << songItemModel->item(row,1)->text() << ";"
               << songItemModel->item(row,2)->text() << ";"
               << songItemModel->item(row,3)->text() << ";"
               << songItemModel->item(row,4)->text() << "\r\n"; //path
    }


    output.flush();
    file.close();

}
//-----------------------------------------------------------
void MainWindow::changesMade()
{
    if (!unsavedChanges)
    {
        unsavedChanges = true;
        this->setWindowTitle(currentUser + "*");
    }

}

void MainWindow::changesSaved()
{
    unsavedChanges = false;
    this->setWindowTitle(currentUser.remove("*"));

}
//-----------------------------------------------------------
void MainWindow::on_actionOpen_triggered()
{
    //Maybe save first
    QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int option = msgBox.exec();

    switch (option) {
      case QMessageBox::Save:
          // Save was clicked
            on_actionSave_triggered();
          break;
      case QMessageBox::Discard:
          // Don't Save was clicked
            //Cont to open dialog
          break;
      case QMessageBox::Cancel:
          // Cancel was clicked
            return;
          break;
      default:
          // should never be reached
          break;
    }

    //Contiue to open open dialog
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\lists", tr("Text Files (*.txt)"));

    if (!fileName.isEmpty()){

        //Get name of file and setTitle to this
        currentUser = QFileInfo(fileName).baseName();
        this->setWindowTitle(currentUser);

        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)){
            QMessageBox::critical(this, tr("Error"), tr("Could not open"));
            return;
        }

        //Read file

        QTextStream input(&file);

        QString line;
        QStringList list;
        int row = 0;

        //Clear current songItemModel and reset header
        songItemModel->clear();
        songItemModel->setHorizontalHeaderLabels(QStringList() << "Title" << "Artist" << "Album" << "Genre" << "Path");

        qDebug() << "here";

        isLoading = true;
        do {
            line = input.readLine();
            qDebug() << line;

            if(!line.isNull())
            {
                list = line.split(";");
                songItemModel->setItem(row, 0, new QStandardItem(list[0]));
                songItemModel->setItem(row, 1, new QStandardItem(list[1]));
                songItemModel->setItem(row, 2, new QStandardItem(list[2]));
                songItemModel->setItem(row, 3, new QStandardItem(list[3]));
                songItemModel->setItem(row, 4, new QStandardItem(list[4]));//path

                QFile file(list[4]);

                if (!file.exists())
                {
                    QBrush brush(Qt::gray);
                    songItemModel->item(row,0)->setForeground(brush);
                    songItemModel->item(row,1)->setForeground(brush);
                    songItemModel->item(row,2)->setForeground(brush);
                    songItemModel->item(row,3)->setForeground(brush);
                    songItemModel->item(row,4)->setForeground(brush);
                }


                row++;
            }
        }while(!line.isNull());

        file.close();
        //Resize Columns
        ui->treeView->resizeColumnToContents(0);
        ui->treeView->resizeColumnToContents(1);
        ui->treeView->resizeColumnToContents(2);
        ui->treeView->resizeColumnToContents(3);

        isLoading = false;

        changesSaved();

    }
}

void MainWindow::slot_itemChanged(QStandardItem* item)
{
    changesMade();
    if (item->text() == "" || item->text() == " ")
    {
        item->setText("Unknown");
    }

    if (!isLoading)
    {
        int row = item->row();
        QFile file(songItemModel->item(row,4)->text());

        if (!file.exists())
        {
            QBrush brush(Qt::gray);
            songItemModel->item(row,0)->setForeground(brush);
            songItemModel->item(row,1)->setForeground(brush);
            songItemModel->item(row,2)->setForeground(brush);
            songItemModel->item(row,3)->setForeground(brush);
            songItemModel->item(row,4)->setForeground(brush);
        }
        else
        {
            QBrush brush(Qt::black);
            songItemModel->item(row,0)->setForeground(brush);
            songItemModel->item(row,1)->setForeground(brush);
            songItemModel->item(row,2)->setForeground(brush);
            songItemModel->item(row,3)->setForeground(brush);
            songItemModel->item(row,4)->setForeground(brush);
        }
    }



    //free(brush);

}

void MainWindow::slot_selectionChanged()
{

    qDebug() << "selection changed";

    QString currentAlbum = songItemModel->item(ui->treeView->currentIndex().row(),2)->text();
    qDebug() << currentAlbum;

    QString plainAlbum = currentAlbum.toLower().remove(QRegExp(QString::fromUtf8("[^a-zA-Z0-9]")));

    qDebug() << plainAlbum;

    QDir dir(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\artwork\\" + currentUser);

    QStringList filter, fileList;
    filter << (plainAlbum + "*");
    fileList = dir.entryList(filter, QDir::Files);

    QString fileNameWExt;

    if (!fileList.isEmpty())
    {
        qDebug() << fileList[0];
        fileNameWExt = fileList[0];

    }
    else
    {
        qDebug() << "empty";
        fileNameWExt = "foo.jpg";
    }

    QPixmap pic(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\artwork\\" + currentUser + "\\" + fileNameWExt);

    if (pic.isNull())
    {
    pic.load(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\artwork\\default.jpg");
    }

    ui->albumCoverLabel->setPixmap(pic.scaled(300,300,Qt::KeepAspectRatio,Qt::SmoothTransformation));

}


void MainWindow::on_addArtButton_clicked()
{
    QString currentAlbum = songItemModel->item(ui->treeView->currentIndex().row(),2)->text();

    QString plainAlbum = currentAlbum.toLower().remove(QRegExp(QString::fromUtf8("[^a-zA-Z0-9]")));

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QStandardPaths::standardLocations(QStandardPaths::PicturesLocation)[0],
            tr("Image Files (*.png *.jpg *.jpeg)"));

    //Need to check to overwrite
    if (!fileName.isEmpty()){
        QFile file(fileName);
        file.copy(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\artwork\\" + currentUser + "\\" + plainAlbum + "." + QFileInfo(fileName).suffix());
    }
}

void MainWindow::on_linkMusicButton_clicked()
{
    int currentRow = ui->treeView->currentIndex().row();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QStandardPaths::standardLocations(QStandardPaths::MusicLocation)[0],
            tr("Music Files (*.mp3)"));

    songItemModel->setItem(currentRow, 4, new QStandardItem(fileName));

    //not grey
    //need to save bool (disp star)/ usaved changes made func changeMade()
    changesMade();



}

void MainWindow::on_actionNew_triggered()
{
    //Maybe save first
    QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    int option = msgBox.exec();

    switch (option) {
      case QMessageBox::Save:
          // Save was clicked
            on_actionSave_triggered();
          break;
      case QMessageBox::Discard:
          // Don't Save was clicked
          break;
      case QMessageBox::Cancel:
          // Cancel was clicked
            return;
          break;
      default:
          // should never be reached
          break;
    }

    //Dialog
    bool ok;
    QString user = QInputDialog::getText(this, tr("New List"),
                                             tr("Username:"), QLineEdit::Normal,
                                             QDir::home().dirName(), &ok);
    //Need to check if user already exists as well
    if (ok && !user.isEmpty())
    {

        //Clear current songItemModel and reset header
        songItemModel->clear();
        songItemModel->setHorizontalHeaderLabels(QStringList() << "Title" << "Artist" << "Album" << "Genre");

        currentUser = user;
        this->setWindowTitle(user);

        //Make text folder
        QFile listFile(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\lists\\" + currentUser + ".txt");
        /*
        if (!listFolder.exists())
        {
            listFolder.mkpath(".");
        }
        */

        if (!listFile.open(QIODevice::WriteOnly)){
            //Error could not open
        }
        listFile.close();

        //Make artwork folder
        QDir artFolder(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\artwork\\" + currentUser);

        if (!artFolder.exists())
        {
            artFolder.mkpath(".");
        }

        changesMade();

    }

}
//-----------MUSIC CONTROLS-----------------//-------------------------------------------------------------------------------------

void MainWindow::slot_mediaStatusChanged(QMediaPlayer::MediaStatus stat)
{
    qDebug() << "slot media stat";
    qDebug() << stat;
    if (stat == QMediaPlayer::LoadedMedia)
    {
    player->play();
    }

}

/*void MainWindow::on_pausePlayButton_toggled(bool checked)
{
        qDebug() << checked;

        if (!currentSongPath.isEmpty())
        {
            if (checked)
            {
                if (player->state() != QMediaPlayer::PausedState)
                {
                    player->setMedia(QUrl::fromLocalFile(currentSongPath));
                    player->setVolume(50);
                }
                player->play();
            }
            else
            {
                player->pause();
            }
        }
        else
        {
            qDebug() << "did not play";
            checked = !checked;
            ui->pausePlayButton->setChecked(false);
        }


}
*/

//When a song is double clicked play if possible
void MainWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
    player->stop();

    //Get path C:\\Users\\Shahrooz\\Music\\Foo_Fighters_-_Times_Like_These.mp3
    currentSongPath = songItemModel->item(ui->treeView->currentIndex().row(),4)->text();
    QFile listFile(currentSongPath);

    if (listFile.exists())
    {
        qDebug() << "EXISTS!!!!!!!!!!!!!!!!";
        ui->pausePlayButton->setChecked(true);
        on_pausePlayButton_clicked(true);

    }
    else
    {
        currentSongPath = "";
        ui->pausePlayButton->setChecked(false);

    }
}

//Play only if path is valid
void MainWindow::on_pausePlayButton_clicked(bool checked)
{
    qDebug() << checked;
    qDebug() << "on_pausePlayButton_clicked";

    if (!currentSongPath.isEmpty())
    {
        qDebug() << "Path is not Empty";
        if (checked)
        {
            if (player->state() != QMediaPlayer::PausedState)
            {
                //player->setMedia(QUrl::fromLocalFile(currentSongPath));
                QUrl url(currentSongPath);//This doesnt accept double slashes
                player->setMedia(url);
                player->setVolume(ui->volumeSlider->sliderPosition());

                switch ( player->error()) {
                  case QMediaPlayer::NoError:
                         qDebug() << "No Error******";
                      break;
                  case QMediaPlayer::ResourceError	:
                      qDebug() << "Resource Error******";
                      break;
                  case QMediaPlayer::FormatError:
                      qDebug() << "Format Error******";
                        return;
                      break;
                case QMediaPlayer::NetworkError:
                    qDebug() << "Network Error*******";
                      return;
                    break;
                case QMediaPlayer::AccessDeniedError:
                    qDebug() << "Access Denied Error*******";
                      return;
                    break;
                case QMediaPlayer::ServiceMissingError:
                    qDebug() << "Service Missing Error*******";
                      return;
                    break;
                  default:
                      qDebug() << "default********";
                      break;
                }


            }
            else
            {
                player->play();
            }
            //Maybe wait for loaded
            //player->play();
        }
        else
        {
            player->pause();
        }
    }
    else
    {
        qDebug() << "did not play";
        ui->pausePlayButton->setChecked(false);
    }

}

//Volume slider
void MainWindow::on_volumeSlider_sliderMoved(int position)
{
    player->setVolume(position);
}



void MainWindow::on_trackSlider_sliderMoved(int position)
{
    player->setPosition(position);
}

void MainWindow::updatePosition(qint64 position)
{
    ui->trackSlider->setValue(position);

    QTime duration(0, position / 60000, qRound((position % 60000) / 1000.0));
    //positionLabel->setText(duration.toString(tr("mm:ss")));
    ui->lcdNumber->display(duration.toString(tr("mm:ss")));
}

void MainWindow::updateDuration(qint64 duration)
{
        ui->trackSlider->setRange(0, duration);
        ui->trackSlider->setEnabled(duration > 0);
        ui->trackSlider->setPageStep(duration / 10);

}

void MainWindow::on_forwardButton_clicked()
{
    int increment = (player->duration())/10;

    int newPos = player->position() + increment;

    if (newPos > player->duration())
    {
        player->setPosition(player->duration());
    }
    else
    {
        player->setPosition(newPos);
    }

}

void MainWindow::on_backButton_clicked()
{
    int increment = (player->duration())/10;

    int newPos = player->position() - increment;

    if (newPos < 0)
    {
        player->setPosition(0);
    }
    else
    {
        player->setPosition(newPos);
    }
}


