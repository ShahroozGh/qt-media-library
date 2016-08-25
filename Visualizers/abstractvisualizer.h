#ifndef ABSTRACTVISUALIZER_H
#define ABSTRACTVISUALIZER_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QVector>
#include <QtMath>
#include <QDebug>
#include <QPainterPath>

class AbstractVisualizer : public QObject
{
    Q_OBJECT
public:
    AbstractVisualizer();
    AbstractVisualizer(QObject *_parent);

    QGraphicsScene *scene;

    virtual void initScene();
    QGraphicsScene* getScene(){return scene;}

    virtual void openOptionsDialog();
    virtual void plotSpectrum(std::vector<float> data);
};

#endif // ABSTRACTVISUALIZER_H
