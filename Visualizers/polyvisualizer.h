#ifndef POLYVISUALIZER_H
#define POLYVISUALIZER_H

#include "abstractvisualizer.h"
#include "polyvisualizeroptionsdialog.h"
#include <QGraphicsBlurEffect>

class PolyVisualizer : public AbstractVisualizer
{
    Q_OBJECT
public:
    PolyVisualizer();

    void initScene();

    PolyVisualizerOptionsDialog* dialog;

    //int totalPoints = 50;
    int totalPoints = 1024;
    int minRadius = 150;
    float angleOffset = 0.0f;
    float gain = 10.0f;

    int circleRadius = 100;

    bool useLogScale = false;
    int mirror = 8; //Works best when factor of totalPoints
    bool alternateMirror = true;
    bool fillVisualizer = false;
    bool rotate = false;
    bool showInvSpectrum = false;
    bool glow = false;

    //Color properties


private slots:
    void updateParameters();



private:
    void openOptionsDialog();
    void plotSpectrum(std::vector<float> data);
    void plotInvPoly(std::vector<float> data);
    void initStyles();

    void transferParametersFromDialog();
    void transferParametersToDialog();

    QGraphicsPolygonItem* polyItem, *polyItem2, *polyItem3, *invPolyItem;
    QPolygon poly, invPoly;

    QGraphicsEllipseItem* circle, *circle2, *circle3;
};

#endif // POLYVISUALIZER_H
