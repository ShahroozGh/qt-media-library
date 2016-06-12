#ifndef SPECTRUMVISUALIZER_H
#define SPECTRUMVISUALIZER_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QVector>

class SpectrumVisualizer
{
public:
    SpectrumVisualizer();
    ~SpectrumVisualizer();

    QGraphicsScene *scene;
    QVector<QGraphicsRectItem*> bars;

    void setParent();
    void initScene();

    void plotSpectrum(std::vector<float> data);

    QGraphicsScene* getScene();
};

#endif // SPECTRUMVISUALIZER_H
