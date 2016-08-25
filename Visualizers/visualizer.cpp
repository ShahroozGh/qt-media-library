#include "visualizer.h"

Visualizer::Visualizer()
{

}

void Visualizer::initScene()
{
    scene = new QGraphicsScene();

    QGraphicsEllipseItem* circle = new QGraphicsEllipseItem();
    polyItem = new QGraphicsPolygonItem();

    smoothPolyItem = new QGraphicsPolygonItem();

    splinePathItem = new QGraphicsPathItem();



    circle->setRect(-50, -50, 100, 100);

    polyItem->setPolygon(poly);

    splinePathItem->setPath(splinePath);

    initPolyVis();
    initSmoothPoly();
    initSplinePath();

    smoothPolyItem->setPolygon(smoothPoly);

    scene->addItem(polyItem);

    scene->addItem(splinePathItem);

    //scene->addItem(smoothPolyItem);

    scene->addItem(circle);


}

QGraphicsScene *Visualizer::getScene()
{
    return scene;
}

void Visualizer::initPolyVis()
{
    int totalPoints = 1024;
    int radius = 150;

    float dAngle = qDegreesToRadians(360.0f / (float)totalPoints);

    for (int i = 0; i < totalPoints; i++)
    {
        float angle = (dAngle * i);

        QPoint point(radius * cos(angle), radius * sin(angle));

        poly << point;

    }
}


//r = C + cos(k*theta) polar
//=> x = r*cos(theta => x = (c+ cos(angle))*cos(theta)
void Visualizer::initSmoothPoly()
{
    int numberOfPetals = 6;
    int minRad = 150;
    int range = 20;
    int totalPoints = 1024;
    int radius = 150;

    float dAngle = qDegreesToRadians(360.0f / (float)totalPoints);

    for (int i = 0; i < totalPoints; i++)
    {
        float angle = (dAngle * i);
        float interRad = minRad + range * cos(numberOfPetals * angle);

        QPoint point(interRad * cos(angle), interRad * sin(angle));

        smoothPoly << point;

    }
}

void Visualizer::initSplinePath()
{
    float controlLength = 25.0f;
    //Number of peaks
    int totalPeaks = 8;
    float troughRad = 150.0f;
    float peakRad = 200.0f;

    float dAngle = qDegreesToRadians(360.0f / (float)(totalPeaks * 2.0f));

    splinePath.moveTo(QPoint(troughRad * cos(-dAngle), troughRad * sin(-dAngle)));

    for (int i = 0; i < totalPeaks; i++)
    {
        float peakAngle = 2 * dAngle * i;
        float troughAnglePrev = peakAngle - dAngle;
        float troughAngleNext = peakAngle + dAngle;

        //Calculate control points (4 in total)
        QPoint c1, c2, c3, c4;

        QPointF firstPoint(troughRad * cos(troughAnglePrev), troughRad * sin(troughAnglePrev));
        c1 = calculateControlPoints(firstPoint, -controlLength);

        QPointF peakPoint(peakRad * cos(peakAngle), peakRad * sin(peakAngle));
        c2 = calculateControlPoints(peakPoint, controlLength);

        QPointF secondPeakPoint(peakRad * cos(peakAngle), peakRad * sin(peakAngle));
        c3 = calculateControlPoints(secondPeakPoint, -controlLength);

        QPointF lastPoint(troughRad * cos(troughAngleNext), troughRad * sin(troughAngleNext));
        c4 = calculateControlPoints(lastPoint, controlLength);




        //Add to path
        splinePath.cubicTo(c1, c2, peakPoint);
        splinePath.cubicTo(c3, c4, lastPoint);

    }

    splinePathItem->setPath(splinePath);
}

QPoint Visualizer::calculateControlPoints(QPointF point, float ctrlLength)
{
    float pointLength = sqrt(pow(point.x(), 2.0f) + pow(point.y(), 2.0f));

    float cx = ctrlLength * point.y()/pointLength + point.x();
    float cy = -ctrlLength * point.x()/pointLength + point.y();

    return QPoint(cx, cy);
}


void Visualizer::plotSpectrum(std::vector<float> data, QString type = "geom")
{
    if (data.size() == 0){
        qDebug() << "EMPTY";
        return;
    }

    if (type == "geom")
    {
        qDebug() << "Not empty";
        poly.clear();

        int totalPoints = 50;
        int minRadius = 150;
        float angleOffest = qDegreesToRadians(90.0f);


        float dAngle = qDegreesToRadians(360.0f / (float)totalPoints);

        for (int i = 0; i < totalPoints; i++)
        {
            float angle = (dAngle * i) - angleOffest;

            //take avg
            int chunkSize = data.size() / totalPoints;
            float accumulator = 0;
            for (int j = 0; j < chunkSize; j++)
            {
                //accumulator += data[i*chunkSize + j];
                accumulator += data[log(i*chunkSize + j + 1)];

            }
            float avg = accumulator / chunkSize;
            //int newRadius = minRadius + 100 * data[i];
            //int newRadius = minRadius + 500 * avg * log(i + 1);
            int newRadius = minRadius + 100 * avg;

            QPoint point(newRadius * cos(angle), newRadius * sin(angle));

            poly << point;

        }

        polyItem->setPolygon(poly);
    }
    else if (type == "spline")
    {
        qDebug() << "Spline";
        splinePath = QPainterPath();

        float controlLength = 25.0f;
        //Number of peaks
        int totalPeaks = 8;
        float troughRad = 150.0f;
        float peakRad = 200.0f;

        float dAngle = qDegreesToRadians(360.0f / (float)(totalPeaks * 2.0f));

        splinePath.moveTo(QPoint(troughRad * cos(-dAngle), troughRad * sin(-dAngle)));

        for (int i = 0; i < totalPeaks; i++)
        {
            float peakAngle = 2 * dAngle * i;
            float troughAnglePrev = peakAngle - dAngle;
            float troughAngleNext = peakAngle + dAngle;

            //take avg
            int chunkSize = data.size() / totalPeaks;
            float accumulator = 0;
            for (int j = 0; j < chunkSize; j++)
            {
                accumulator += data[(int)log(i*chunkSize + j + 1)];

            }
            float avg = accumulator / chunkSize;
            peakRad = troughRad + avg * 100;

            //Calculate control points (4 in total)
            QPoint c1, c2, c3, c4;

            QPointF firstPoint(troughRad * cos(troughAnglePrev), troughRad * sin(troughAnglePrev));
            c1 = calculateControlPoints(firstPoint, -controlLength);

            QPointF peakPoint(peakRad * cos(peakAngle), peakRad * sin(peakAngle));
            c2 = calculateControlPoints(peakPoint, controlLength);

            QPointF secondPeakPoint(peakRad * cos(peakAngle), peakRad * sin(peakAngle));
            c3 = calculateControlPoints(secondPeakPoint, -controlLength);

            QPointF lastPoint(troughRad * cos(troughAngleNext), troughRad * sin(troughAngleNext));
            c4 = calculateControlPoints(lastPoint, controlLength);




            //Add to path
            splinePath.cubicTo(c1, c2, peakPoint);
            splinePath.cubicTo(c3, c4, lastPoint);

        }

        splinePathItem->setPath(splinePath);

    }
}
