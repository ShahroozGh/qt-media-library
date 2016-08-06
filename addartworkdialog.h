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

#include <QFileDialog>



#include <iostream>

namespace Ui {
class AddArtworkDialog;
}

enum QueryType {Album, Artist};

class AddArtworkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddArtworkDialog(QWidget *parent = 0);
    ~AddArtworkDialog();
    QString currentUser;
    QPixmap selectedImg;
    QPixmap defaultImg;
    void setQuery(QString query, QueryType type);
    void setDefaultImg(QPixmap img);



private slots:
    void on_artSearchPushButton_clicked();
    //void requestFinished(QNetworkReply* reply);
    void requestFinished();
    void imgDownloadRequestComplete();

    void on_buttonBox_accepted();

    void on_artworkListWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_findExistingArtButton_clicked();

private:
    Ui::AddArtworkDialog *ui;
    QString query;
    QueryType qType;
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
