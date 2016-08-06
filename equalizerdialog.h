#ifndef EQUALIZERDIALOG_H
#define EQUALIZERDIALOG_H

#include <QDialog>
#include "AudioSystem.h"

namespace Ui {
class EqualizerDialog;
}

class EqualizerDialog : public QDialog
{

    Q_OBJECT


public:
    explicit EqualizerDialog(QWidget *parent = 0);
    ~EqualizerDialog();

    void setAudioSystem(AudioSystem* audSys);

private slots:
    void on_eqSlider1_actionTriggered(int action);

    void on_eqSlider2_actionTriggered(int action);

    void on_eqSlider3_actionTriggered(int action);

    void on_eqCenterDial1_actionTriggered(int action);

    void on_eqBandwidthDial1_actionTriggered(int action);

    void on_eqCenterDial2_actionTriggered(int action);

    void on_eqBandwidthDial2_actionTriggered(int action);

    void on_eqCenterDial3_actionTriggered(int action);

    void on_eqBandwidthDial3_actionTriggered(int action);

private:
    Ui::EqualizerDialog *ui;
    AudioSystem* audioSys;

    struct EQSettings {
        float gain;
        float center;
        float bandwidth;
    };

    EQSettings eq1, eq2, eq3;
};

#endif // EQUALIZERDIALOG_H
