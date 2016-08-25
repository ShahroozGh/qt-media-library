#ifndef VISUALIZERDIALOG_H
#define VISUALIZERDIALOG_H

#include <QDialog>
#include <QVector>
#include "Visualizers/abstractvisualizer.h"
#include "Visualizers/polyvisualizer.h"

namespace Ui {
class VisualizerDialog;
}

class VisualizerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VisualizerDialog(QWidget *parent = 0);
    ~VisualizerDialog();

    void plotSpectrum(std::vector<float> data);
    void repaintGraphics();
private slots:
    void on_optionsPushButton_clicked();

private:
    Ui::VisualizerDialog *ui;
    AbstractVisualizer* vis;
};

#endif // VISUALIZERDIALOG_H
