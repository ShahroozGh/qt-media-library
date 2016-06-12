#include "SpectrumVisualizer.h"

SpectrumVisualizer::SpectrumVisualizer()
{

}

SpectrumVisualizer::~SpectrumVisualizer()
{
    delete scene;

}

void SpectrumVisualizer::initScene()
{
    scene = new QGraphicsScene();

    QBrush greenBrush(Qt::green);
    QBrush blueBrush(Qt::blue);
    QBrush brush(QColor(255,0,0,0));
    QPen outlinePen(Qt::black);
    outlinePen.setWidth(0);


    for(int i = 0; i < 256; i++){
        bars.push_back(scene->addRect(4*i, 0, 1, -i*0.1, outlinePen, greenBrush));
    }

}

void SpectrumVisualizer::plotSpectrum(std::vector<float> data)
{

    if (data.empty())
        return;

    for (int i = 0; i < data.size() / 4; i++) {
            float volVal = data[i];
            //volVal = 20 * log10(1 + volVal);
            volVal = 10 * volVal;
            //volVal = log10(10 + i)*volVal;

            float height = -100 * log10(volVal + 1);
            bars[i]->setRect(4*i, 0, 1, height);
        }
    scene->update(scene->sceneRect());
}

QGraphicsScene *SpectrumVisualizer::getScene()
{
    return scene;

}
