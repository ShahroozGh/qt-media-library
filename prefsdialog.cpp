#include "prefsdialog.h"
#include "ui_prefsdialog.h"

PrefsDialog::PrefsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrefsDialog)
{
    ui->setupUi(this);
}

PrefsDialog::~PrefsDialog()
{
    delete ui;
}

void PrefsDialog::on_chooseStyleSheetPushButton_clicked()
{
    //Contiue to open open dialog
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open StyleSheet"), QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)[0] + "\\songPro\\themes", tr("StyleSheet Files (*.css)"));

    if (!fileName.isEmpty()){
        ui->styleSheetPathLineEdit->setText(fileName);

    }
}

void PrefsDialog::on_buttonBox_accepted()
{
    stylePath = ui->styleSheetPathLineEdit->text();
    dropShadowsEnabled = ui->dropShadowCheckBox->isChecked();
    blurRadius = ui->blurRadiusSpinBox->value();

}
