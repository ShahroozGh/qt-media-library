#ifndef PREFSDIALOG_H
#define PREFSDIALOG_H

#include <QDialog>

#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>

#include <QStandardPaths>
#include <QFileInfo>

#include "mainwindow.h"

namespace Ui {
class PrefsDialog;
}

class PrefsDialog : public QDialog
{
   Q_OBJECT

public:
    explicit PrefsDialog(QWidget *parent = 0);
    ~PrefsDialog();

    QString stylePath;
    bool dropShadowsEnabled;
    int blurRadius;
    bool useCustomFrame;

private slots:
    void on_chooseStyleSheetPushButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::PrefsDialog *ui;
};

#endif // PREFSDIALOG_H
