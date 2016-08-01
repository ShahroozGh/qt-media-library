#include "equalizerdialog.h"
#include "ui_equalizerdialog.h"

EqualizerDialog::EqualizerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EqualizerDialog)
{
    ui->setupUi(this);

    //Set default eq positions
    eq1.center = 8000.0f;
    eq1.bandwidth = 1.0f;
    eq1.gain = 0.0f;

    eq2.center = 8000.0f;
    eq2.bandwidth = 1.0f;
    eq2.gain = 0.0f;

    eq3.center = 8000.0f;
    eq3.bandwidth = 1.0f;
    eq3.gain = 0.0f;

    ui->eqSlider1->setValue(100.0f*(eq1.gain + 30.0f)/60.0f);
    ui->eqCenterDial1->setValue(100.0f*(eq1.center - 20.0f)/21980.0f);
    ui->eqBandwidthDial1->setValue(100.0f*(eq1.bandwidth - 0.2f)/4.8f);

    ui->gainLabel1->setText(QString::number(eq1.gain));
    ui->eqCenterLabel1->setText(QString::number(eq1.center));
    ui->eqBandwidthLabel1->setText(QString::number(eq1.bandwidth));

    ui->eqSlider2->setValue(100.0f*(eq1.gain + 30.0f)/60.0f);
    ui->eqCenterDial2->setValue(100.0f*(eq1.center - 20.0f)/21980.0f);
    ui->eqBandwidthDial2->setValue(100.0f*(eq1.bandwidth - 0.2f)/4.8f);

    ui->gainLabel2->setText(QString::number(eq1.gain));
    ui->eqCenterLabel2->setText(QString::number(eq1.center));
    ui->eqBandwidthLabel2->setText(QString::number(eq1.bandwidth));

    ui->eqSlider3->setValue(100.0f*(eq1.gain + 30.0f)/60.0f);
    ui->eqCenterDial3->setValue(100.0f*(eq1.center - 20.0f)/21980.0f);
    ui->eqBandwidthDial3->setValue(100.0f*(eq1.bandwidth - 0.2f)/4.8f);

    ui->gainLabel3->setText(QString::number(eq1.gain));
    ui->eqCenterLabel3->setText(QString::number(eq1.center));
    ui->eqBandwidthLabel3->setText(QString::number(eq1.bandwidth));
}

EqualizerDialog::~EqualizerDialog()
{
    delete ui;
}

void EqualizerDialog::setAudioSystem(AudioSystem *audSys)
{
    audioSys = audSys;
}


void EqualizerDialog::on_eqSlider1_actionTriggered(int action)
{
    float normPos = ui->eqSlider1->sliderPosition()/100.0f; //0-99

    eq1.gain = (60.0f*normPos) - 30.0f;

    audioSys->setEqParams(eq1.gain, eq1.center, eq1.bandwidth, 0);

    ui->gainLabel1->setText(QString::number(eq1.gain));
}

void EqualizerDialog::on_eqCenterDial1_actionTriggered(int action)
{
    float normPos = ui->eqCenterDial1->sliderPosition()/100.0f;

    eq1.center = (21980.0f * normPos) + 20.0f;

    audioSys->setEqParams(eq1.gain, eq1.center, eq1.bandwidth, 0);

    ui->eqCenterLabel1->setText(QString::number(eq1.center));
}

void EqualizerDialog::on_eqBandwidthDial1_actionTriggered(int action)
{
    float normPos = ui->eqBandwidthDial1->sliderPosition()/100.0f;

    eq1.bandwidth = (4.8f * normPos) + 0.2f;

    audioSys->setEqParams(eq1.gain, eq1.center, eq1.bandwidth, 0);

    ui->eqBandwidthLabel1->setText(QString::number(eq1.bandwidth));
}

void EqualizerDialog::on_eqSlider2_actionTriggered(int action)
{
    float normPos = ui->eqSlider2->sliderPosition()/100.0f; //0-99

    eq2.gain = (60.0f*normPos) - 30.0f;

    audioSys->setEqParams(eq2.gain, eq2.center, eq2.bandwidth, 1);

    ui->gainLabel2->setText(QString::number(eq2.gain));
}


void EqualizerDialog::on_eqCenterDial2_actionTriggered(int action)
{
    float normPos = ui->eqCenterDial2->sliderPosition()/100.0f;

    eq2.center = (21980.0f * normPos) + 20.0f;

    audioSys->setEqParams(eq2.gain, eq2.center, eq2.bandwidth, 1);

    ui->eqCenterLabel2->setText(QString::number(eq2.center));
}

void EqualizerDialog::on_eqBandwidthDial2_actionTriggered(int action)
{
    float normPos = ui->eqBandwidthDial2->sliderPosition()/100.0f;

    eq2.bandwidth = (4.8f * normPos) + 0.2f;

    audioSys->setEqParams(eq3.gain, eq3.center, eq3.bandwidth, 1);

    ui->eqBandwidthLabel2->setText(QString::number(eq2.bandwidth));
}

void EqualizerDialog::on_eqSlider3_actionTriggered(int action)
{
    float normPos = ui->eqSlider3->sliderPosition()/100.0f; //0-99

    eq3.gain = (60.0f*normPos) - 30.0f;

    audioSys->setEqParams(eq3.gain, eq3.center, eq3.bandwidth, 2);

    ui->gainLabel3->setText(QString::number(eq3.gain));

}


void EqualizerDialog::on_eqCenterDial3_actionTriggered(int action)
{
    float normPos = ui->eqCenterDial3->sliderPosition()/100.0f;

    eq3.center = (21980.0f * normPos) + 20.0f;

    audioSys->setEqParams(eq3.gain, eq3.center, eq3.bandwidth, 2);

    ui->eqCenterLabel3->setText(QString::number(eq3.center));
}

void EqualizerDialog::on_eqBandwidthDial3_actionTriggered(int action)
{
    float normPos = ui->eqBandwidthDial3->sliderPosition()/100.0f;

    eq3.bandwidth = (4.8f * normPos) + 0.2f;

    audioSys->setEqParams(eq3.gain, eq3.center, eq3.bandwidth, 2);

    ui->eqBandwidthLabel3->setText(QString::number(eq3.bandwidth));
}
