#ifndef POLYVISUALIZEROPTIONSDIALOG_H
#define POLYVISUALIZEROPTIONSDIALOG_H

#include <QDialog>

namespace Ui {
class PolyVisualizerOptionsDialog;
}

class PolyVisualizerOptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PolyVisualizerOptionsDialog(QWidget *parent = 0);
    ~PolyVisualizerOptionsDialog();

    //Options
    int totalPoints;
    int minRadius;
    float angleOffset;
    float gain;


    bool useLogScale;
    int mirror; //Works best when factor of totalPoints
    bool alternateMirror;
    bool fillVisualizer;
    bool rotate;
    bool showInvSpectrum;
    bool glow;

    void initOptions();

signals:
    void changesMade();

private slots:
    void on_buttonBox_accepted();

    void optionModified();

private:
    Ui::PolyVisualizerOptionsDialog *ui;
};

#endif // POLYVISUALIZEROPTIONSDIALOG_H
