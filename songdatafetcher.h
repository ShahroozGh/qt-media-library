#ifndef SONGDATAFETCHER_H
#define SONGDATAFETCHER_H

#include <QObject>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QJsonArray>

#include <QStandardItemModel>

#include <iostream>


//enum QueryType {Album, Artist};

class SongDataFetcher : public QObject
{
    Q_OBJECT
public:
    explicit SongDataFetcher(QObject *parent = 0);
    void fetchAndSetAllArt(QStandardItemModel* model);


private slots:
    void imgLinkRequestComplete();
    void imgDownloadRequestComplete();

private:
    QNetworkAccessManager* netManager;

    QVector<QString> imgLinks;

    QStandardItemModel* model;

    QVector<QString> getImgURLFromJSON(QString jsonData);

signals:

public slots:


};

#endif // SONGDATAFETCHER_H
