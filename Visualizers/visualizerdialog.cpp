#include "visualizerdialog.h"
#include "ui_visualizerdialog.h"

VisualizerDialog::VisualizerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VisualizerDialog)
{
    ui->setupUi(this);

    vis = new PolyVisualizer();
    vis->initScene();

    ui->graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    ui->graphicsView->setScene(vis->getScene());
    ui->graphicsView->setSceneRect(vis->getScene()->sceneRect());
    ui->graphicsView->centerOn(0,0);
}

VisualizerDialog::~VisualizerDialog()
{
    delete ui;
    delete vis;//May not need if parenting works
}

void VisualizerDialog::plotSpectrum(std::vector<float> data)
{
    vis->plotSpectrum(data);
}

void VisualizerDialog::repaintGraphics()
{
    ui->graphicsView->centerOn(0,0);
    ui->graphicsView->repaint();
}

void VisualizerDialog::on_optionsPushButton_clicked()
{
    vis->openOptionsDialog();
}
