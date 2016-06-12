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

    //True if file is currently being read
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


   //Initialize audio system
   fmodSys.initFMOD();


   //Set up signal/slot to detect when selection changes
   QObject::connect(ui->treeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(slot_selectionChanged()));
   QObject::connect(songItemModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slot_itemChanged(QStandardItem*)));

   //Timer to signal update loop for fft
   updateTimer = new QTimer(this);
   connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateLoop()));
   updateTimer->start(30);

   //Delete
   QObject::connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(updatePosition(qint64)));
   QObject::connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(updateDuration(qint64)));
   QObject::connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(slot_mediaStatusChanged(QMediaPlayer::MediaStatus)));

}

MainWindow::~MainWindow()
{
    delete ui;
    //Free fmod resources
}
//-----------------------------------------------------------

//Add song Button
//This function will add a new item to the list based on data given in the text boxes
//For now used just for testing purposes
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

    //need to check if album art should be deleted from folder

    changesMade();

}
//-----------------------------------------------------------
void MainWindow::on_pushButtonSearch_clicked()
{

    qDebug() << "search clicked";
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
//Triggered when search query text changes
//Updates proxyModel to display matching items in the list
//Without modifying main model
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
//If sa
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
//Write updated data in model (song list) to correct text file for current library
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
//Used to display asterix to notify user of unsaved changes
//Call whenever changes are made that need to be saved
void MainWindow::changesMade()
{
    if (!unsavedChanges)
    {
        unsavedChanges = true;
        this->setWindowTitle(currentUser + "*");
    }

}

//Used to remove asterix if there are no unsaved changes
//Call whenever changes are saved
void MainWindow::changesSaved()
{
    unsavedChanges = false;
    this->setWindowTitle(currentUser.remove("*"));

}
//-----------------------------------------------------------
void MainWindow::on_actionOpen_triggered()
{
    //Use message box to give user opportunity to save changes
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

        //Read data from text file into song item model
        isLoading = true;
        do {
            line = input.readLine();
            qDebug() << line;

            if(!line.isNull())
            {
                list = line.split(";");
                //Check if list.size != 5, if so it is invalid
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

//Called if an item's data has been edited
void MainWindow::slot_itemChanged(QStandardItem* item)
{
    changesMade();
    //If new data is empty replace string with Unknown
    if (item->text() == "" || item->text() == " ")
    {
        //Maybe block signal first so that this next statement doesnt trigger slot_temChanged
        //thus avoiding an infinite loop (not sure if setText will trigger it)
        //As of now it doesnt seem to trigger it, only editing form the GUI seems too
        qDebug() << "item setText()\n";
        item->setText("Unknown");
    }

    if (!isLoading)
    {
        //Get items row
        int row = item->row();
        //Get audio file from given path
        QFile file(songItemModel->item(row,4)->text());

        //Check if the audio file exists
        //If not change its text color to gray to notify user
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


//Triggered when a row in the song list is selected
//This function will then update the album image currently displayed to match the selected song
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

//When add artwork button is clicked this will be triggered
//Function allows user to select an image file to use for this album
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

//Allows user to add an mp3 file for the currently selected song
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

//Fmod requires an update function to be called periodically for fft to work
//Also updates trackbar position
void MainWindow::updateLoop()
{
    //qDebug() << "Updatye";
    fmodSys.updateTick();
    fmodSys.updateFFT();

    //Update trackbar position

    //Get song position in percentage
    if (fmodSys.isPlaying())
    {
        float percent = (float)fmodSys.getPosition()/(float)fmodSys.getLength();

        int trackBarLength = ui->trackSlider->maximum() - ui->trackSlider->minimum();

        ui->trackSlider->setValue(trackBarLength * percent);

        QTime duration(0, fmodSys.getPosition() / 60000, qRound((fmodSys.getPosition() % 60000) / 1000.0));
        //positionLabel->setText(duration.toString(tr("mm:ss")));
        ui->lcdNumber->display(duration.toString(tr("mm:ss")));
    }
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

    //Get path
    currentSongPath = songItemModel->item(ui->treeView->currentIndex().row(),4)->text();
    QFile listFile(currentSongPath);

    if (listFile.exists())
    {
        qDebug() << "EXISTS!!!!!!!!!!!!!!!!";
        //Attempt to load song and play it
        fmodSys.setAudioFile(currentSongPath.toStdString());
        fmodSys.startPlayback();
        fmodSys.setVolume(ui->volumeSlider->sliderPosition()/100.0f);
        ui->pausePlayButton->setChecked(true);
        //on_pausePlayButton_clicked(true);

    }
    else
    {
        currentSongPath = "";
        ui->pausePlayButton->setChecked(false);

    }
}

//Play only if path is valid, checked == true means playing
void MainWindow::on_pausePlayButton_clicked(bool checked)
{
    qDebug() << checked;
    qDebug() << "on_pausePlayButton_clicked";

    if (!currentSongPath.isEmpty())
    {
        //There is a song in the selection
        qDebug() << "Path is not Empty";
        //Check if selected song has changed i.e fmodSys songPath != currentSongPath

        fmodSys.setPaused(!checked);

    }
    else
    {
        //Song path empty
        qDebug() << "did not play";
        ui->pausePlayButton->setChecked(false);
    }

}

//Volume slider
void MainWindow::on_volumeSlider_sliderMoved(int position)
{
    fmodSys.setVolume(position/100.0f);
}


//Change to action triggered
void MainWindow::on_trackSlider_sliderMoved(int position)
{
    int max = ui->trackSlider->maximum();
    int min = ui->trackSlider->minimum();


    float percent = position/(float)(max-min);

    fmodSys.setPosition(percent * fmodSys.getLength());
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
    float increment = (fmodSys.getLength())/10.0f;

    float newPos = fmodSys.getPosition() + increment;

    if (newPos > fmodSys.getLength())
    {
        //Dont seek if no space
    }
    else
    {
        fmodSys.seek((int)increment);
    }

}

void MainWindow::on_backButton_clicked()
{
    float increment = (fmodSys.getLength())/10.0f;

    float newPos = fmodSys.getPosition() + increment;

    if (newPos < 0)
    {
        //Dont seek if no space
    }
    else
    {
        fmodSys.seek((int)increment * -1);
    }
}


