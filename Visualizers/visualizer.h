#ifndef VISUALIZER_H
#define VISUALIZER_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QVector>
#include <QtMath>
#include <QDebug>
#include <QPainterPath>




class Visualizer
{


public:
    Visualizer();

    QGraphicsScene *scene;
    QGraphicsPolygonItem* polyItem;
    QGraphicsPolygonItem* smoothPolyItem;
    QGraphicsPathItem* splinePathItem;

    void initScene();
    QGraphicsScene* getScene();
    void plotSpectrum(std::vector<float> data, QString type);

private:
    void initPolyVis();

    void initSmoothPoly();

    void initSplinePath();

    QPoint calculateControlPoints(QPointF point, float ctrlLength);

    QPolygon poly;

    QPolygon smoothPoly;

    QPainterPath splinePath;


};

#endif // VISUALIZER_H
