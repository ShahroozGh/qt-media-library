#include "songdatafetcher.h"

SongDataFetcher::SongDataFetcher(QObject *parent) : QObject(parent)
{

}

void SongDataFetcher::fetchAndSetAllArt(QStandardItemModel *model)
{
    this->model = model;
    //bool pending = true;
    //completedCount = 0;

    for (int row = 0; row < model->rowCount(); row++)
    {
        /*
        output << model->item(row,0)->text() << ";"
               << model->item(row,1)->text() << ";"
               << model->item(row,2)->text() << ";"
               << model->item(row,3)->text() << ";"
               << model->item(row,4)->text() << "\r\n"; //path
               */

        bool hasArt = model->item(row, 5)->data(Qt::UserRole + 2).toBool();

        if (hasArt)
            continue;

        QString query = model->item(row, 2)->text();

        netManager = new QNetworkAccessManager(this);

        //Set up the URL
        QUrl url;
        url = QUrl("https://api.spotify.com/v1/search?q=" + QUrl::toPercentEncoding(query) + "&type=album");
        QNetworkRequest req(url);
        req.setAttribute(QNetworkRequest::User, row);

        QNetworkReply* reply = netManager->get(req);
        QObject::connect(reply, SIGNAL(finished()),this, SLOT(imgLinkRequestComplete()));

    }
}

void SongDataFetcher::imgLinkRequestComplete()
{
    QNetworkReply* reply = (QNetworkReply*)sender();
    //std::cout << "requestFinished" << std::endl;

    if (reply->error() == QNetworkReply::NoError){
        qDebug() << "No Net Error";
        QString replyText = (QString)reply->readAll();
        //reply->deleteLater();
        //std::cout << replyText.toStdString() << std::endl;

        //Parse the JSON response
        QVector<QString> imgUrls = getImgURLFromJSON(replyText);

        //Start getting images
        if (!imgUrls.empty()){
            QUrl url(imgUrls[0]);

            QNetworkRequest req(url);
            //Pass on row number
            req.setAttribute(QNetworkRequest::User, reply->request().attribute(QNetworkRequest::User).toInt());
            QNetworkReply* newReply = netManager->get(req);
            QObject::connect(newReply, SIGNAL(finished()),this, SLOT(imgDownloadRequestComplete()));

        }

    }
    else{
        std::cout << "Net error" << std::endl;
        qDebug() << reply->errorString();

    }

    reply->deleteLater();
}

void SongDataFetcher::imgDownloadRequestComplete()
{
    QNetworkReply* reply = (QNetworkReply*)sender();
    if (reply->error() == QNetworkReply::NoError){
        qDebug() << "No IMG download Error";

        //Store img in pixmap
        QPixmap albumImg;
        albumImg.loadFromData(reply->readAll());
        int row = reply->request().attribute(QNetworkRequest::User).toInt();
        qDebug() << row;
        //Update ui list
        QStandardItem* item = model->item(row,5);
        item->setIcon(QIcon(albumImg));
        item->setData(true, Qt::UserRole + 2);

        //completedCount++;
        //if(completedCount == total) pending = false;
    }
    else{
        qDebug() << "IMG DOWNLOAD ERROR";

    }

    reply->deleteLater();

}

QVector<QString> SongDataFetcher::getImgURLFromJSON(QString jsonData)
{
    QVector<QString> imgUrls;

    QJsonDocument doc = QJsonDocument::fromJson(jsonData.toUtf8());
    QJsonObject baseObj = doc.object();

    QString value;
    value = "albums";

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

