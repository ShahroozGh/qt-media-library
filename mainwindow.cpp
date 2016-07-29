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
    this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint );

    //this->setStyleSheet("background-color: grey;");
    //this->setPalette( QPalette(QColor(55,55,55)) );

    //Set up button group
    categorySelectorGroup = new QButtonGroup(this);
    categorySelectorGroup->addButton(ui->displaySongsButton);
    categorySelectorGroup->addButton(ui->displayAlbumsButton);
    categorySelectorGroup->addButton(ui->displayArtistsButton);

    categorySelectorGroup->setId(ui->displaySongsButton, 0);
    categorySelectorGroup->setId(ui->displayAlbumsButton, 1);
    categorySelectorGroup->setId(ui->displayArtistsButton, 2);

    QObject::connect(categorySelectorGroup, SIGNAL(buttonClicked(int)), SLOT(cat_button_group_clicked(int)));

    ui->pausePlayButton->setCheckable(true);


    //Set up menu bar and add to layout
    ((QGridLayout*)ui->centralWidget->layout())->addWidget(createMenuBar(), 4, 1, 1, 4);

    //songPro folder under users documents folder
    QIcon forward(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\resources\\forwardIcon.png");
    QIcon back(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\resources\\backIcon.png");
    //ui->forwardButton->setIcon(forward);
    //ui->backButton->setIcon(back);


    //ui->pausePlayButton->setIcon();

    QIcon* playPause = new QIcon();

    playPause->addPixmap(QPixmap(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\resources\\pauseIcon.png"),QIcon::Normal,QIcon::On);
    playPause->addPixmap(QPixmap(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\resources\\playIcon.png"),QIcon::Normal,QIcon::Off);
    //ui->pausePlayButton->setIcon(*playPause);


    player = new QMediaPlayer;

    ui->volumeSlider->setMaximum(100);

    //True if file is currently being read
    isLoading = false;

    //Create new item model that will store all tracks
    //songItemModel = new QStandardItemModel(3,4,this);
    songItemModel = new QStandardItemModel(0,0,this);


    //set model for tree view and labels
    ui->treeView->setModel(songItemModel);
    ui->treeView->setSortingEnabled(true);
    songItemModel->setHorizontalHeaderLabels(QStringList() << "Title" << "Artist" << "Album" << "Genre" << "Path" << "Art");
    ui->treeView->header()->moveSection(5,0);
    ui->treeView->setIconSize(QSize(50,50));

   //Set album picture to default
   QPixmap pic(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\artwork\\default.jpg");
   ui->albumCoverLabel->setPixmap(pic.scaled(200,200,Qt::KeepAspectRatio,Qt::SmoothTransformation));

   defaultAlbumArt.load(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\artwork\\default.jpg");
    //ui->treeView->editTriggers()


   //Initialize audio system
   fmodSys.initFMOD();

   //Set up freq scene
   visualizer.initScene();
   ui->graphicsView->setStyleSheet("background: transparent");
   ui->graphicsView->setScene(visualizer.getScene());
   ui->graphicsView->setAlignment(Qt::AlignBottom|Qt::AlignLeft);
   ui->graphicsView->lower();


   //Set item delegates
    ui->albumListView->setItemDelegate(new QCardStyledItemDelegate(this));

   //Set up signal/slot to detect when selection changes
   QObject::connect(ui->treeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(slot_selectionChanged()));
   QObject::connect(songItemModel, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(slot_itemChanged(QStandardItem*)));

   setStyleSheets();

   //Timer to signal update loop for fft
   updateTimer = new QTimer(this);
   connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateLoop()));
   updateTimer->start(30);

   //Delete
   //QObject::connect(player, SIGNAL(positionChanged(qint64)), this, SLOT(updatePosition(qint64)));
   //QObject::connect(player, SIGNAL(durationChanged(qint64)), this, SLOT(updateDuration(qint64)));
   //QObject::connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(slot_mediaStatusChanged(QMediaPlayer::MediaStatus)));

}

MainWindow::~MainWindow()
{
    delete ui;
    //Free fmod resources
}


//UI Look and Feel
void MainWindow::setStyleSheets()
{
    //Load Custom Fonts
    int id = QFontDatabase::addApplicationFont("C:/Users/Shahrooz/Documents/songPro/resources/Fonts/calm/KeepCalm-Medium.ttf");
    qDebug() << "ID " << id;
    if (id == -1){
        std::cout << "Font Could Not Be loaded" << std::endl;

    }
    else
         QString family = QFontDatabase::applicationFontFamilies(id).at(0);


    id = QFontDatabase::addApplicationFont("C:/Users/Shahrooz/Documents/songPro/resources/Fonts/Roboto/Roboto-Light.ttf");
    qDebug() << "ID 2: " << id;
    if (id == -1){
        std::cout << "Font 2 Could Not Be loaded" << std::endl;

    }
    else
         QString family = QFontDatabase::applicationFontFamilies(id).at(0);

    id = QFontDatabase::addApplicationFont("C:/Users/Shahrooz/Documents/songPro/resources/Fonts/Roboto/Roboto-Thin.ttf");
    qDebug() << "ID 3: " << id;
    if (id == -1){
        std::cout << "Font 3 Could Not Be loaded" << std::endl;

    }
    else
         QString family = QFontDatabase::applicationFontFamilies(id).at(0);

    id = QFontDatabase::addApplicationFont("C:/Users/Shahrooz/Documents/songPro/resources/Fonts/Roboto/Roboto-Regular.ttf");
    qDebug() << "ID 4: " << id;
    if (id == -1){
        std::cout << "Font 4 Could Not Be loaded" << std::endl;

    }
    else
         QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    //QFont monospace(family);

    QString fileName = "C:\\Users\\Shahrooz\\Documents\\songDb\\mainWindowStyleSheet.css";
    //Read stylesheet file
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream in(&file);
    QString text;
    text = in.readAll();
    file.close();

    this->setStyleSheet(text);

    //Set Effects
    QGraphicsDropShadowEffect* ds = new QGraphicsDropShadowEffect(this);
    ds->setBlurRadius(20);
    ds->setOffset(0,0);
    ds->setColor(Qt::black);
    ui->treeView->setGraphicsEffect(ds);


    QGraphicsDropShadowEffect* ds2 = new QGraphicsDropShadowEffect(this);
    ds2->setBlurRadius(20);
    ds2->setOffset(0,0);
    ds2->setColor(Qt::black);

    ui->pushButtonAdd->setGraphicsEffect(ds2);

    ds = new QGraphicsDropShadowEffect(this);
    ds->setBlurRadius(20);
    ds->setOffset(0,0);
    ds->setColor(Qt::black);

    ui->pushButtonDelete->setGraphicsEffect(ds);



    ds = new QGraphicsDropShadowEffect(this);
    ds->setBlurRadius(20);
    ds->setOffset(0,0);
    ds->setColor(Qt::black);

    ui->pushButtonSearch->setGraphicsEffect(ds);


    ds = new QGraphicsDropShadowEffect(this);
    ds->setBlurRadius(20);
    ds->setOffset(0,0);
    ds->setColor(Qt::black);

    ui->albumCoverLabel->setGraphicsEffect(ds);

    ds = new QGraphicsDropShadowEffect(this);
    ds->setBlurRadius(30);
    ds->setOffset(0,0);
    ds->setColor(Qt::black);

    ui->lineEditSearch->setGraphicsEffect(ds);


    ds = new QGraphicsDropShadowEffect(this);
    ds->setBlurRadius(5);
    ds->setOffset(0,0);
    ds->setColor(Qt::black);

    ui->trackSlider->setGraphicsEffect(ds);

    ds = new QGraphicsDropShadowEffect(this);
    ds->setBlurRadius(5);
    ds->setOffset(0,0);
    ds->setColor(Qt::black);

    ui->volumeSlider->setGraphicsEffect(ds);

    ds = new QGraphicsDropShadowEffect(this);
    ds->setBlurRadius(20);
    ds->setOffset(0,0);
    ds->setColor(Qt::black);

    ui->pausePlayButton->setGraphicsEffect(ds);

    ds = new QGraphicsDropShadowEffect(this);
    ds->setBlurRadius(20);
    ds->setOffset(0,0);
    ds->setColor(Qt::black);

    ui->forwardButton->setGraphicsEffect(ds);

    ds = new QGraphicsDropShadowEffect(this);
    ds->setBlurRadius(20);
    ds->setOffset(0,0);
    ds->setColor(Qt::black);

    ui->backButton->setGraphicsEffect(ds);

    ds = new QGraphicsDropShadowEffect(this);
    ds->setBlurRadius(20);
    ds->setOffset(0,0);
    ds->setColor(Qt::black);


    ui->songInfoFrame->setGraphicsEffect(ds);








}

//Sets a global drop Shadow properties
void MainWindow::setDropShadowProperties(int blurRadius, QColor color)
{
    //Set Effects
    QGraphicsDropShadowEffect* ds = new QGraphicsDropShadowEffect(this);
    ds->setBlurRadius(blurRadius);
    ds->setOffset(0,0);
    ds->setColor(color);
    ui->treeView->setGraphicsEffect(ds);


    QGraphicsDropShadowEffect* ds2 = new QGraphicsDropShadowEffect(this);
    ds2->setBlurRadius(blurRadius);
    ds2->setOffset(0,0);
    ds2->setColor(color);

    ui->pushButtonAdd->setGraphicsEffect(ds2);

    ds = new QGraphicsDropShadowEffect(this);
    ds->setBlurRadius(blurRadius);
    ds->setOffset(0,0);
    ds->setColor(color);

    ui->pushButtonDelete->setGraphicsEffect(ds);



    ds = new QGraphicsDropShadowEffect(this);
    ds->setBlurRadius(blurRadius);
    ds->setOffset(0,0);
    ds->setColor(color);

    ui->pushButtonSearch->setGraphicsEffect(ds);


    ds = new QGraphicsDropShadowEffect(this);
    ds->setBlurRadius(blurRadius);
    ds->setOffset(0,0);
    ds->setColor(color);

    ui->albumCoverLabel->setGraphicsEffect(ds);

    ds = new QGraphicsDropShadowEffect(this);
    ds->setBlurRadius(blurRadius);
    ds->setOffset(0,0);
    ds->setColor(color);

    ui->lineEditSearch->setGraphicsEffect(ds);


    ds = new QGraphicsDropShadowEffect(this);
    ds->setBlurRadius(blurRadius);
    ds->setOffset(0,0);
    ds->setColor(color);

    ui->trackSlider->setGraphicsEffect(ds);

    ds = new QGraphicsDropShadowEffect(this);
    ds->setBlurRadius(blurRadius);
    ds->setOffset(0,0);
    ds->setColor(color);

    ui->volumeSlider->setGraphicsEffect(ds);

    ds = new QGraphicsDropShadowEffect(this);
    ds->setBlurRadius(blurRadius);
    ds->setOffset(0,0);
    ds->setColor(color);

    ui->pausePlayButton->setGraphicsEffect(ds);

    ds = new QGraphicsDropShadowEffect(this);
    ds->setBlurRadius(blurRadius);
    ds->setOffset(0,0);
    ds->setColor(color);

    ui->forwardButton->setGraphicsEffect(ds);

    ds = new QGraphicsDropShadowEffect(this);
    ds->setBlurRadius(blurRadius);
    ds->setOffset(0,0);
    ds->setColor(color);

    ui->backButton->setGraphicsEffect(ds);

    ds = new QGraphicsDropShadowEffect(this);
    ds->setBlurRadius(blurRadius);
    ds->setOffset(0,0);
    ds->setColor(color);


    ui->songInfoFrame->setGraphicsEffect(ds);
}

//Toggles dropShadows
void MainWindow::enableDropShadows(bool enabled)
{

    ui->treeView->graphicsEffect()->setEnabled(enabled);

    ui->pushButtonAdd->graphicsEffect()->setEnabled(enabled);

    ui->pushButtonDelete->graphicsEffect()->setEnabled(enabled);

    ui->pushButtonSearch->graphicsEffect()->setEnabled(enabled);

    ui->albumCoverLabel->graphicsEffect()->setEnabled(enabled);

    ui->lineEditSearch->graphicsEffect()->setEnabled(enabled);

    ui->trackSlider->graphicsEffect()->setEnabled(enabled);

    ui->volumeSlider->graphicsEffect()->setEnabled(enabled);

    ui->pausePlayButton->graphicsEffect()->setEnabled(enabled);

    ui->forwardButton->graphicsEffect()->setEnabled(enabled);

    ui->backButton->graphicsEffect()->setEnabled(enabled);

    ui->songInfoFrame->graphicsEffect()->setEnabled(enabled);

}

void MainWindow::changeStyleSheet(QString styleSheet)
{

    QString fileName = styleSheet;
    //Read stylesheet file
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) return;
    QTextStream in(&file);
    QString text;
    text = in.readAll();
    file.close();

    this->setStyleSheet(text);

}

void MainWindow::updateCurrentSongInfoDisplay()
{
    QString title = songItemModel->item(ui->treeView->currentIndex().row(),0)->text();
    QString artist = songItemModel->item(ui->treeView->currentIndex().row(),1)->text();
    QString album = songItemModel->item(ui->treeView->currentIndex().row(),2)->text();

    ui->titleLabel->setText(title);
    ui->artistLabel->setText(artist);
    ui->albumLabel->setText(album);


}

//Switches between using the custome native titlebar and the custom skinned one
void MainWindow::useCustomFrame(bool use)
{
    if (use){
        this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint );
        ui->titleBarWidget->show();
        this->show();
    }
    else{
        this->setWindowFlags(Qt::Window);
        ui->titleBarWidget->hide();
        this->show();

    }
}

//Drag and Drop events
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list"))
            event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    qDebug() << "DROP";
    QList<QUrl> urls = event->mimeData()->urls();
       if (urls.isEmpty()){
            qDebug() << "Empty Drop URL";
            return;
       }


       for (int i = 0; i < urls.size(); i++){
           QString fileName = urls[i].toLocalFile();

           //Check if file name exists
           if (fileName.isEmpty()){
               qDebug() << "File name is empty: " << i;

           }
           else {
                qDebug() << fileName << " DROPPED #" << i << " Suffix: " << QFileInfo(fileName).suffix() << " Base: " << QFileInfo(fileName).baseName();
                if (QFileInfo(fileName).suffix() == "mp3")
                    addFile(fileName.toStdString());

           }


       }


       //if (readFile(fileName))
       //    setWindowTitle(tr("%1 - %2").arg(fileName).arg(tr("Drag File")));
}
//-----------------------------------------------------------

//Add song to list given file name
//Attempt to fill in info given mp3 tag data
void MainWindow::addFile(std::string path)
{
    AudioSystem::SongInfo tagInfo = fmodSys.getTagData(path);


    //Get text from lineEdit txt boxes
    QString title = QFileInfo(QString::fromStdString(path)).baseName();
    QString artist = "unknown";
    QString album = "unknown";
    QString genre = "unknown";
    QString pathName = QString::fromStdString(path);

    //Replace unknown with tag info if found
    if (!tagInfo.title.empty())
        title = QString::fromStdString(tagInfo.title);

    if (!tagInfo.artist.empty())
        artist = QString::fromStdString(tagInfo.artist);

    if (!tagInfo.album.empty())
        album = QString::fromStdString(tagInfo.album);

    if (!tagInfo.genre.empty())
        genre = QString::fromStdString(tagInfo.genre);


    //Create item for each
    QStandardItem *titleItem = new QStandardItem(title);
    QStandardItem *artistItem = new QStandardItem(artist);
    QStandardItem *albumItem = new QStandardItem(album);
    QStandardItem *genreItem = new QStandardItem(genre);
    QStandardItem *pathItem = new QStandardItem(pathName);

    //Add items to a list (of 3 QStanItems)
    QList<QStandardItem*> list;
    list << titleItem << artistItem << albumItem << genreItem << pathItem;

    //Insert list of items as a row
    songItemModel->insertRow(0, list);



    //Resize columns
    ui->treeView->resizeColumnToContents(0);
    ui->treeView->resizeColumnToContents(1);
    ui->treeView->resizeColumnToContents(2);
    ui->treeView->resizeColumnToContents(3);

    changesMade();
}

//Add song Button
//This function will add a new item to the list based on data given in the text boxes
//For now used just for testing purposes
void MainWindow::on_pushButtonAdd_clicked()
{
    //Get text from lineEdit txt boxes
    QString title;
    QString artist;
    QString album;
    //QString genre = ui->lineEditGenre->text();
    QString genre;

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
    /*
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
    */

    //Temp test to dump tag info
    //Get path
    /*
    QString path = songItemModel->item(ui->treeView->currentIndex().row(),4)->text();
    QFile listFile(path);

    if (listFile.exists()) //Check if it is an mp3 as well
    {
        qDebug() << "EXISTS!!!!!!!!!!!!!!!!";
        //Attempt to load song and dump data
        fmodSys.getTagData(path.toStdString());


    }
    else
    {
        qDebug() << "NO FILE FOUND";


    }
    */

    //Temp comment out above, put back clater
    ui->treeView->setModel(songItemModel);

}
//-----------------------------------------------------------
void MainWindow::on_pushButtonSearch_clicked()
{

    qDebug() << "search clicked";
    //ui->lineEditSearch->clear();
    /*
    ui->treeView->setModel(songItemModel);
    qDebug() << "Unselected";
    ui->lineEditSearch->clear();
    QObject::connect(ui->treeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(slot_selectionChanged()));

    useCustomFrame(false);
*/
    SongDataFetcher* fetcher = new SongDataFetcher();
    fetcher->fetchAndSetAllArt(songItemModel);
}
//-----------------------------------------------------------
//Delete, sort button doesnt exist anymore
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
    QFile file(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\lists\\" + currentUser + ".json");

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
    /*
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

*/
    //Using json format

    //Create a json array of tracks
    QJsonObject topLevel;
    QJsonArray trackList;

    for (int row = 0; row < songItemModel->rowCount(); row++)
    {
        QJsonObject track;
        track.insert("title", songItemModel->item(row,0)->text());
        track.insert("artist", songItemModel->item(row,1)->text());
        track.insert("album", songItemModel->item(row,2)->text());
        track.insert("genre", songItemModel->item(row,3)->text());
        track.insert("audio_path", songItemModel->item(row,4)->text());

        trackList.append(track);
    }
    topLevel.insert("tracks", trackList);

    QJsonDocument saveFile;
    saveFile.setObject(topLevel);

    file.write(saveFile.toJson());

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
    /*
    //Use message box to give user opportunity to save changes
    //Should only do this if there are unsaved changes
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
          //call return so that we dont continue with the open operation
            return;
          break;
      default:
          // should never be reached
          break;
    }

    //Contiue to open open dialog
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\lists", tr("Text Files (*.txt)"));

    if (!fileName.isEmpty()){



        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)){
            QMessageBox::critical(this, tr("Error"), tr("Could not open"));
            return;
        }

        if (!validateSongList(fileName)){
            QMessageBox::critical(this, tr("Error"), tr("Could not open file. Invalid format."));
            std::cout << "Invalid file! Will not open." << std::endl;
            return;

        }
        //Get name of file and setTitle to this
        currentUser = QFileInfo(fileName).baseName();
        this->setWindowTitle(currentUser);

        //Read file

        QTextStream input(&file);

        QString line;
        QStringList list;
        int row = 0;


        //Clear current songItemModel and reset header
        songItemModel->clear();
        songItemModel->setHorizontalHeaderLabels(QStringList() << "Title" << "Artist" << "Album" << "Genre" << "Path" << "Art");
        ui->treeView->header()->moveSection(5,0);

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
                QPixmap albumArt = getAlbumArt(list[2]);
                if (albumArt.isNull()){
                    albumArt = defaultAlbumArt;
                    songItemModel->setItem(row, 5, new QStandardItem(QIcon(albumArt), ""));
                    //False if has no album art
                    songItemModel->item(row, 5)->setData(false, Qt::UserRole + 2);
                }
                else{
                    songItemModel->setItem(row, 5, new QStandardItem(QIcon(albumArt), ""));
                    songItemModel->item(row, 5)->setData(true, Qt::UserRole + 2);
                }

                songItemModel->setItem(row, 6, new QStandardItem(""));


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
        ui->treeView->resizeColumnToContents(5);

        isLoading = false;

        changesSaved();

    }
    */


    //Use message box to give user opportunity to save changes
    //Should only do this if there are unsaved changes
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
          //call return so that we dont continue with the open operation
            return;
          break;
      default:
          // should never be reached
          break;
    }

    //Contiue to open open dialog
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\lists", tr("JSON Files (*.json)"));

    if (!fileName.isEmpty()){



        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)){
            QMessageBox::critical(this, tr("Error"), tr("Could not open"));
            return;
        }

        //Clear current songItemModel and reset header
        songItemModel->clear();
        songItemModel->setHorizontalHeaderLabels(QStringList() << "Title" << "Artist" << "Album" << "Genre" << "Path" << "Art");
        ui->treeView->header()->moveSection(5,0);


        //Get name of file and setTitle to this
        currentUser = QFileInfo(fileName).baseName();
        this->setWindowTitle(currentUser);

        //Read file
        //Create JSON doc
        QJsonDocument saveFileDoc = QJsonDocument::fromJson(file.readAll());

        //Get main object and track array
        QJsonObject topLevelObj = saveFileDoc.object();

        QJsonArray tracks = topLevelObj["tracks"].toArray();
        qDebug() << "GOT TOP";

        //Make sure other parts of the program know data is still being loaded
        isLoading = true;

        for (int row = 0; row < tracks.size(); row++){

            QJsonObject track = tracks[row].toObject();
            qDebug() << "GOT TRACK";
            QString title = track["title"].toString();
            QString artist = track["artist"].toString();
            QString album = track["album"].toString();
            QString genre = track["genre"].toString();
            QString songPath = track["audio_path"].toString();

            qDebug() << "GOT STUFF"  << title << " " << artist << " " << album << " " << genre << " "  << songPath;


            songItemModel->setItem(row, 0, new QStandardItem(title));
            qDebug() << "TITLE STUFF";
            songItemModel->setItem(row, 1, new QStandardItem(artist));
            qDebug() << "ARTIST STUFF";
            songItemModel->setItem(row, 2, new QStandardItem(album));
            qDebug() << "ALBUM STUFF";
            songItemModel->setItem(row, 3, new QStandardItem(genre));
            qDebug() << "GENER STUFF";
            songItemModel->setItem(row, 4, new QStandardItem(songPath));

            qDebug() << "SET STUFF";

            QPixmap albumArt = getAlbumArt(album);
            if (albumArt.isNull()){
                albumArt = defaultAlbumArt;
                songItemModel->setItem(row, 5, new QStandardItem(QIcon(albumArt), ""));
                //False if has no album art
                songItemModel->item(row, 5)->setData(false, Qt::UserRole + 2);
            }
            else{
                songItemModel->setItem(row, 5, new QStandardItem(QIcon(albumArt), ""));
                songItemModel->item(row, 5)->setData(true, Qt::UserRole + 2);
            }

            songItemModel->setItem(row, 6, new QStandardItem(""));


            QFile file(songPath);

            if (!file.exists())
            {
                QBrush brush(Qt::gray);
                songItemModel->item(row,0)->setForeground(brush);
                songItemModel->item(row,1)->setForeground(brush);
                songItemModel->item(row,2)->setForeground(brush);
                songItemModel->item(row,3)->setForeground(brush);
                songItemModel->item(row,4)->setForeground(brush);
            }


        }



        file.close();
        //Resize Columns
        ui->treeView->resizeColumnToContents(0);
        ui->treeView->resizeColumnToContents(1);
        ui->treeView->resizeColumnToContents(2);
        ui->treeView->resizeColumnToContents(3);
        ui->treeView->resizeColumnToContents(5);

        isLoading = false;

        changesSaved();

    }
}

//Called if an item's data has been edited
void MainWindow::slot_itemChanged(QStandardItem* item)
{
    changesMade();
    //If new data is empty replace string with Unknown
    if ((item->text() == "" || item->text() == " ") && item->column() != 5)
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

    //Get selected album
    QString currentAlbum = songItemModel->item(ui->treeView->currentIndex().row(),2)->text();
    qDebug() << currentAlbum;

    //QPixmap pic = getAlbumArt(currentAlbum);
    /*

    //turn album name into simple string, all lowecase with no numbers (this is the filename of the artwork)
    QString plainAlbum = currentAlbum.toLower().remove(QRegExp(QString::fromUtf8("[^a-zA-Z0-9]")));

    qDebug() << plainAlbum;

    //Find file
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

    //If picture doesnt exist use default image
    if (pic.isNull())
    {
        pic.load(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\artwork\\default.jpg");
    }
    */

    QIcon picIcon = qvariant_cast<QIcon>(songItemModel->item(ui->treeView->currentIndex().row(),5)->data(Qt::DecorationRole));
    QPixmap pic = picIcon.pixmap(200,200);
    ui->albumCoverLabel->setPixmap(pic.scaled(200,200,Qt::KeepAspectRatio,Qt::SmoothTransformation));
    //songItemModel->item(ui->treeView->currentIndex().row(),2)->setIcon(QIcon(pic));
}

//REMOVE THIS
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

//When add artwork button is clicked this will be triggered
//Function allows user to select an image file to use for this album
void MainWindow::on_actionAdd_art_form_file_triggered()
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

QMenuBar *MainWindow::createMenuBar()
{

    //Create menubar
    QMenuBar* menuBar = new QMenuBar(this);
    menuBar->setObjectName("menuBar");
    menuBar->setGeometry(QRect(0,0,window()->width(),20));
    //menuBar->setNativeMenuBar(false);

    //Set up actions
    QAction* testAction = new QAction(this);
    testAction->setObjectName("test");
    testAction->setText("test");

    //Set up menus
    QMenu* fileMenu = new QMenu(menuBar);
    fileMenu->setTitle("File");

    QMenu* editMenu = new QMenu(menuBar);
    editMenu->setTitle("Edit");

    //Add actions to menuBar
    menuBar->addAction(fileMenu->menuAction());
    menuBar->addAction(editMenu->menuAction());

    //Add fileMenu actions
    fileMenu->addAction("New", this, SLOT(on_actionNew_triggered()));
    fileMenu->addAction("Open", this, SLOT(on_actionOpen_triggered()));
    fileMenu->addAction("Save", this, SLOT(on_actionSave_triggered()));
    fileMenu->addAction("Save as", this, SLOT(on_actionSave_as_triggered()));

    //Add Edit actions
    editMenu->addAction("Find Album Art...", this, SLOT(on_actionFind_album_art_triggered()));
    editMenu->addAction("Add art from file...", this, SLOT(on_actionAdd_art_form_file_triggered()));
    editMenu->addAction("Find Artist Art...", this, SLOT(on_actionFind_artist_art_triggered()));
    editMenu->addAction("Link Music file...", this, SLOT(on_actionLink_Music_File_triggered()));
    editMenu->addAction("Preferences...", this, SLOT(on_actionPreferences_triggered()));

    return menuBar;


}

//Returns pixmap of album art for a given album title
//If none found it will use the default image
QPixmap MainWindow::getAlbumArt(QString albumTitle)
{
    //turn album name into simple string, all lowecase with no numbers (this is the filename of the artwork)
    QString plainAlbum = albumTitle.toLower().remove(QRegExp(QString::fromUtf8("[^a-zA-Z0-9]")));

    qDebug() << plainAlbum;

    //Find file
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

    //If picture doesnt exist use default image
    if (pic.isNull())
    {
        //pic.load(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\artwork\\default.jpg");
    }

    return pic;

}


//REMOVE THIS
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

//Add mp3 file to track (from drop down)
void MainWindow::on_actionLink_Music_File_triggered()
{
    int currentRow = ui->treeView->currentIndex().row();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QStandardPaths::standardLocations(QStandardPaths::MusicLocation)[0],
            tr("Music Files (*.mp3)"));

    songItemModel->setItem(currentRow, 4, new QStandardItem(fileName));

    //not grey
    //need to save bool (disp star)/ usaved changes made func changeMade()
    changesMade();
}

bool MainWindow::validateSongList(QString fileName)
{
    bool isValid = true;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)){
        QMessageBox::critical(this, tr("Error"), tr("Could not open"));
        return false;
    }

    //Read file

    QTextStream input(&file);

    QString line;
    QStringList list;
    int row = 0;

    //Read data from text file into song item model
    isLoading = true;
    do {
        line = input.readLine();
        qDebug() << line;

        if(!line.isNull())
        {
            list = line.split(";");
            if (list.size() != 5){
                isValid = false;
                break;
            }
            row++;
        }
    }while(!line.isNull());


    file.close();
    if (isValid)
        return true;
    else
        return false;
}



//Fmod requires an update function to be called periodically for fft to work
//Also updates trackbar position
void MainWindow::updateLoop()
{
    //qDebug() << "Updatye";
    fmodSys.updateTick();
    fmodSys.updateFFT();

    visualizer.plotSpectrum(fmodSys.getRawSpectrumData());

    ui->graphicsView->repaint();

    //Update trackbar position

    //Get song position in percentage
    if (fmodSys.isPlaying())
    {
        float percent = (float)fmodSys.getPosition()/(float)fmodSys.getLength();

        int trackBarLength = ui->trackSlider->maximum() - ui->trackSlider->minimum();

        ui->trackSlider->setValue(trackBarLength * percent);

        QTime duration(0, fmodSys.getPosition() / 60000, (fmodSys.getPosition() % 60000 / 1000.0));
        QTime totalDuration(0, fmodSys.getLength() / 60000, (fmodSys.getLength() % 60000 / 1000.0));
        if (!duration.isValid()){
            std::cout << "Not Valid Time Min:" << fmodSys.getPosition() / 60000 << " Sec: " << (fmodSys.getPosition() % 60000 / 1000.0) << std::endl;
        }
        if (duration.isNull()){
            std::cout << "Null Time"  << std::endl;
        }
        //positionLabel->setText(duration.toString(tr("mm:ss")));
        //ui->lcdNumber->display(duration.toString(tr("mm:ss")));
        if (!duration.isNull() && duration.isValid())
            ui->timeLabel->setText(duration.toString(tr("m:ss")) + "/" + totalDuration.toString("m:ss"));

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
        songItemModel->setHorizontalHeaderLabels(QStringList() << "Title" << "Artist" << "Album" << "Genre" << "Path" << "Art");
        ui->treeView->header()->moveSection(5,0);

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

    if (listFile.exists()) //Check if it is an mp3 as well
    {
        qDebug() << "EXISTS!!!!!!!!!!!!!!!!";
        //Attempt to load song and play it
        fmodSys.setAudioFile(currentSongPath.toStdString());
        fmodSys.startPlayback();
        fmodSys.setVolume(ui->volumeSlider->sliderPosition()/100.0f);
        ui->pausePlayButton->setChecked(true);
        //on_pausePlayButton_clicked(true);

        updateCurrentSongInfoDisplay();

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
    //fmodSys.setVolume(position/100.0f);
}

void MainWindow::on_volumeSlider_actionTriggered(int action)
{
    int position = ui->volumeSlider->sliderPosition();
    fmodSys.setVolume(position/100.0f);
}


//Change to action triggered
void MainWindow::on_trackSlider_sliderMoved(int position)
{
    /*
    int max = ui->trackSlider->maximum();
    int min = ui->trackSlider->minimum();


    float percent = position/(float)(max-min);

    fmodSys.setPosition(percent * fmodSys.getLength());
    */
}

void MainWindow::on_trackSlider_actionTriggered(int action)
{
    int position = ui->trackSlider->sliderPosition();
    int max = ui->trackSlider->maximum();
    int min = ui->trackSlider->minimum();


    float percent = position/(float)(max-min);

    fmodSys.setPosition(percent * fmodSys.getLength());

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



//Menu triggers
void MainWindow::on_actionPreferences_triggered()
{
    PrefsDialog* prefsWin = new PrefsDialog(this);

    if (prefsWin->exec() == QDialog::Accepted){
        qDebug() << "ACCEPT";
        changeStyleSheet(prefsWin->stylePath);
        enableDropShadows(prefsWin->dropShadowsEnabled);
        setDropShadowProperties(prefsWin->blurRadius, Qt::black);
        useCustomFrame(prefsWin->useCustomFrame);

    }
    delete prefsWin;
}

void MainWindow::on_actionFind_album_art_triggered()
{
    //Get selected album
    if (!ui->treeView->currentIndex().isValid()){
        QMessageBox::critical(this, tr("Error"), tr("No Song Selected"));
        return;
    }


    QString currentAlbum = songItemModel->item(ui->treeView->currentIndex().row(),2)->text();




    AddArtworkDialog* artWin = new AddArtworkDialog(this);
    artWin->setQuery(currentAlbum, QueryType::Album);
    if (artWin->exec() == QDialog::Accepted){
        qDebug() << "ACCEPT";
        QPixmap img = artWin->selectedImg;

        //Store the image
        QString plainAlbum = currentAlbum.toLower().remove(QRegExp(QString::fromUtf8("[^a-zA-Z0-9]")));


        //Need to check to overwrite
        if (true){
            QFile file(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\artwork\\" + currentUser + "\\" + plainAlbum + "." + "png");
            file.open(QIODevice::WriteOnly);
            img.save(&file, "PNG");

            //Update pixmaps in the song model with new album covers
            qDebug() << songItemModel->rowCount();
            for (int i = 0; i < songItemModel->rowCount(); i++){
                QStandardItem* item = songItemModel->item(i,5);
                QStandardItem* albumItem = songItemModel->item(i,2);
                if (albumItem->text() == currentAlbum){
                    //item->setData(QIcon(img), Qt::DecorationRole);
                    item->setIcon(QIcon(img));
                qDebug() << "Icon set";
                    }

            }

        }
    }


}

void MainWindow::on_actionFind_artist_art_triggered()
{

    //Get selected artist
    if (!ui->treeView->currentIndex().isValid()){
        QMessageBox::critical(this, tr("Error"), tr("No Song Selected"));
        return;
    }


    QString currentArtist = songItemModel->item(ui->treeView->currentIndex().row(),1)->text();




    AddArtworkDialog* artWin = new AddArtworkDialog(this);
    artWin->setQuery(currentArtist, QueryType::Artist);
    if (artWin->exec() == QDialog::Accepted){
        qDebug() << "ACCEPT";
        QPixmap img = artWin->selectedImg;

        //Store the image
        QString plainArtist = currentArtist.toLower().remove(QRegExp(QString::fromUtf8("[^a-zA-Z0-9]")));


        //Need to check to overwrite
        if (true){
            QFile file(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\artwork\\" + currentUser + "\\artists\\" + plainArtist + "." + "png");
            file.open(QIODevice::WriteOnly);
            img.save(&file, "PNG");

            //Update pixmaps in the song model with new album covers
            qDebug() << songItemModel->rowCount();
            for (int i = 0; i < songItemModel->rowCount(); i++){
                QStandardItem* item = songItemModel->item(i,6);
                QStandardItem* albumItem = songItemModel->item(i,1);
                if (albumItem->text() == currentArtist){
                    //item->setData(QIcon(img), Qt::DecorationRole);
                    item->setIcon(QIcon(img));
                qDebug() << "Icon set";
                    }

            }

        }
    }

}

void MainWindow::on_pushButtonPg1_clicked()
{
    ui->songListsStackedWidget->setCurrentIndex(0);
}

void MainWindow::on_pushButtonPg2_clicked()
{
    ui->songListsStackedWidget->setCurrentIndex(1);
    QCategoryFilterProxyModel* albumProxyModel = new QCategoryFilterProxyModel(this);
    albumProxyModel->setSourceModel(songItemModel);
    albumProxyModel->setFilterKeyColumn(2);

    ui->albumListView->setModel(albumProxyModel);
}

//If icon view list is double clicked
void MainWindow::on_albumListView_doubleClicked(const QModelIndex &index)
{
    //Need to Check whether an artist or album was clicked
    int column = ((QCategoryFilterProxyModel*)ui->albumListView->model())->filterKeyColumn();
    //Change view to an album or artist
    QString album;
    if (column == 2)
        album = index.data(Qt::DisplayRole).toString();
    else if (column == 1)
        album = index.data(Qt::UserRole + 1).toString();

    //Set a filter proxy model to display items from chosen album
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(songItemModel);

    //Filter by album name
    proxyModel->setFilterRegExp(QRegExp(album,Qt::CaseSensitive,QRegExp::FixedString));
    proxyModel->setFilterKeyColumn(column);

    ui->treeView->setModel(proxyModel);

    //Reconnect selection signal/slot
    //QObject::connect(ui->treeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(slot_selectionChanged()));

    //Change label
    ui->IconListTypeLabel->setText(album);

    //Switch page
    ui->songListsStackedWidget->setCurrentIndex(0);


}

void MainWindow::cat_button_group_clicked(int id)
{
    qDebug() << "ID: " << id;
    QList<QAbstractButton*> buttons = categorySelectorGroup->buttons();


    //If trakc button clicked
    if (id == 0){
        //Switch to regular treeview and change its model to display all tracks
        ui->songListsStackedWidget->setCurrentIndex(0);
        ui->treeView->setModel(songItemModel);
        ui->IconListTypeLabel->setText("All Tracks");

        //Reconnect selection changed signal/slot now that model is changed
        QObject::connect(ui->treeView->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(slot_selectionChanged()));
    }
    else if(id == 1){
        ui->songListsStackedWidget->setCurrentIndex(1);
        //Maybe make this global
        QCategoryFilterProxyModel* albumProxyModel = new QCategoryFilterProxyModel(this);
        albumProxyModel->setSourceModel(songItemModel);
        albumProxyModel->setFilterKeyColumn(2);


        ui->albumListView->setModel(albumProxyModel);

        ui->IconListTypeLabel->setText("Albums");
    }
    else if(id == 2){
        ui->songListsStackedWidget->setCurrentIndex(1);
        //Maybe make this global
        QCategoryFilterProxyModel* albumProxyModel = new QCategoryFilterProxyModel(this);
        albumProxyModel->setSourceModel(songItemModel);
        albumProxyModel->setFilterKeyColumn(1);

        ui->albumListView->setModel(albumProxyModel);

        ui->IconListTypeLabel->setText("Artists");

    }
}




//For custom close buttons

void MainWindow::on_closeB_clicked()
{
    qApp->quit();
}

void MainWindow::on_minMaxB_clicked()
{
    if (window()->isMaximized())
        window()->showNormal();
    else
        window()->showMaximized();
}

void MainWindow::on_minimizeB_clicked()
{
    window()->showMinimized();
}
