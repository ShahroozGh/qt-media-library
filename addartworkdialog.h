#ifndef ADDARTWORKDIALOG_H
#define ADDARTWORKDIALOG_H

#include <QDialog>
#include <QtNetwork>
#include <QNetworkAccessManager>
#include <QNetworkReply>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QVector>
#include <QListWidgetItem>



#include <iostream>

namespace Ui {
class AddArtworkDialog;
}

class AddArtworkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddArtworkDialog(QWidget *parent = 0);
    ~AddArtworkDialog();

    QPixmap selectedImg;

    void setAlbumQuery(QString albumTitle);



private slots:
    void on_artSearchPushButton_clicked();
    //void requestFinished(QNetworkReply* reply);
    void requestFinished();
    void imgDownloadRequestComplete();

    void on_buttonBox_accepted();

private:
    Ui::AddArtworkDialog *ui;
    QString albumName;
    QNetworkAccessManager* netManager;
    QVector<QPixmap> albumImages;

    QVector<QString> getImgURLsFromJSON(QString jsonData);

    struct AlbumData {
        QPixmap img;
        QListWidgetItem* listItem;

    };

    QVector<AlbumData> albums;



};

#endif // ADDARTWORKDIALOG_H
