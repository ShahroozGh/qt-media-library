#include "addartworkdialog.h"
#include "ui_addartworkdialog.h"

AddArtworkDialog::AddArtworkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddArtworkDialog)
{
    ui->setupUi(this);

    ui->artworkListWidget->setViewMode(QListWidget::IconMode);
    ui->artworkListWidget->setIconSize(QSize(60,60));
    ui->artworkListWidget->setResizeMode(QListWidget::Adjust);
//    m_listeWidget->setViewMode(QListWidget::IconMode);

//    m_listeWidget->setIconSize(QSize(200,200));

//    m_listeWidget->setResizeMode(QListWidget::Adjust);

//    m_listeWidget->addItem(new QListWidgetItem(QIcon("../earth.jpg"),"Earth"));
//    m_listeWidget->addItem(new QListWidgetItem(QIcon("../tornado.jpg"),"Tornado"));
//    m_listeWidget->addItem(new QListWidgetItem(QIcon("../ics.jpg"),"Wallpaper"));
}

AddArtworkDialog::~AddArtworkDialog()
{
    delete ui;
}

void AddArtworkDialog::setQuery(QString query, QueryType type)
{
    this->query = query;
    this->qType = type;
    //albumName = albumTitle;
}


void AddArtworkDialog::on_artSearchPushButton_clicked()
{
    netManager = new QNetworkAccessManager(this);

    //QObject::connect(netManager, SIGNAL(finished(QNetworkReply*)),this, SLOT(requestFinished(QNetworkReply*)));
    QUrl url;
    if (qType == QueryType::Album)
        url = QUrl("https://api.spotify.com/v1/search?q=" + QUrl::toPercentEncoding(query) + "&type=album");
    else
        url = QUrl("https://api.spotify.com/v1/search?q=" + QUrl::toPercentEncoding(query) + "&type=artist");

    QNetworkReply* reply = netManager->get(QNetworkRequest(url));
    QObject::connect(reply, SIGNAL(finished()),this, SLOT(requestFinished()));
}

//void AddArtworkDialog::requestFinished(QNetworkReply *reply)
void AddArtworkDialog::requestFinished()
{
    QNetworkReply* reply = (QNetworkReply*)sender();
    std::cout << "requestFinished" << std::endl;

    if (reply->error() == QNetworkReply::NoError){
        qDebug() << "No Net Error";
        QString replyText = (QString)reply->readAll();
        //reply->deleteLater();
        //std::cout << replyText.toStdString() << std::endl;

        //Parse the JSON response
        QVector<QString> imgUrls = getImgURLsFromJSON(replyText);

        //Start getting images
        for (int i = 0; i < imgUrls.size(); i++){
            QUrl url(imgUrls[i]);

            QNetworkRequest req(url);
            req.setAttribute(QNetworkRequest::User, i);
            QNetworkReply* reply = netManager->get(req);
            QObject::connect(reply, SIGNAL(finished()),this, SLOT(imgDownloadRequestComplete()));

        }

    }
    else{
        std::cout << "Net error" << std::endl;
        qDebug() << reply->errorString();

    }

    reply->deleteLater();
}

void AddArtworkDialog::imgDownloadRequestComplete()
{
    QNetworkReply* reply = (QNetworkReply*)sender();
    if (reply->error() == QNetworkReply::NoError){
        qDebug() << "No IMG download Error";

        //Store img in pixmap
        QPixmap albumImg;
        albumImg.loadFromData(reply->readAll());
        int id = reply->request().attribute(QNetworkRequest::User).toInt();
        qDebug() << id;
        //Update ui list
        QListWidgetItem* item = new QListWidgetItem(QIcon(albumImg),QString::number(id));
        ui->artworkListWidget->addItem(item);
        //reply->url().toString()
        //albumImages.push_back(albumImg);

        AlbumData data;
        data.img = albumImg;
        data.listItem = item;

        albums.push_back(data);

    }
    else{
        qDebug() << "IMG DOWNLOAD ERROR";

    }

    reply->deleteLater();
}

QVector<QString> AddArtworkDialog::getImgURLsFromJSON(QString jsonData)
{
    QVector<QString> imgUrls;

    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject baseObj = doc.object();

    QString value;
    if (qType == QueryType::Album)
        value = "albums";
    else
        value = "artists";

    QJsonObject albumsObj = baseObj.value(value).toObject();
    qDebug() << "Limit: " << albumsObj["limit"].toInt();

    //Get album list
    QJsonArray albumArray = albumsObj["items"].toArray();
    qDebug() << "Array size: " << albumArray.size();

    for (int i = 0; i < albumArray.size(); i++){

        //Get imgURLs
        //QJsonObject singleAlbum = albumArray[0];
        qDebug() << albumArray[i].toObject()["name"].toString();

        //Get image url array jsonObject (contains three objects containing url to different album  sizes)
        QJsonArray images = albumArray[i].toObject()["images"].toArray();

        //Get the 300x300 picture url
        QString url = images[1].toObject()["url"].toString();
        qDebug() << url;

        imgUrls.push_back(url);

    }




    return imgUrls;
}

//Network functions

void AddArtworkDialog::on_buttonBox_accepted()
{
    for(int i = 0; i < albums.size(); i++){
        if (albums[i].listItem->isSelected()){
            selectedImg = albums[i].img;
            return;
        }

    }
}
