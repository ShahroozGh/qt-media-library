#include "polyvisualizer.h"

PolyVisualizer::PolyVisualizer()
{

}

void PolyVisualizer::initScene()
{



    scene = new QGraphicsScene();
    //scene->setSceneRect(-500,-500,1000,1000);


    circle = new QGraphicsEllipseItem();
    circle2 = new QGraphicsEllipseItem();
    circle3 = new QGraphicsEllipseItem();
    polyItem = new QGraphicsPolygonItem();
    polyItem2 = new QGraphicsPolygonItem();
    polyItem3 = new QGraphicsPolygonItem();
    invPolyItem = new QGraphicsPolygonItem();



    circle->setRect(-circleRadius, -circleRadius, 2*circleRadius, 2*circleRadius);
    circle2->setRect(-circleRadius, -circleRadius, 2*circleRadius, 2*circleRadius);
    circle3->setRect(-circleRadius, -circleRadius, 2*circleRadius, 2*circleRadius);

    circle2->setScale(0.9);
    circle3->setScale(0.8);

    polyItem->setPolygon(poly);
    polyItem2->setPolygon(poly);
    polyItem3->setPolygon(poly);
    invPolyItem->setPolygon(invPoly);

    polyItem2->setScale(0.9);
    polyItem3->setScale(0.8);

    invPolyItem->setScale(0.5);

    std::vector<float> empty;
    plotSpectrum(empty);

    scene->addItem(polyItem);
    scene->addItem(polyItem2);
    scene->addItem(polyItem3);
    scene->addItem(invPolyItem);

    scene->addItem(circle);
    scene->addItem(circle2);
    scene->addItem(circle3);

    //QGraphicsBlurEffect *effect = new QGraphicsBlurEffect();
    //effect->setBlurRadius(10);

    //polyItem->setGraphicsEffect(effect);

    QGraphicsOpacityEffect* op1 = new QGraphicsOpacityEffect();
    op1->setOpacity(0.5f);

    QGraphicsOpacityEffect* op2 = new QGraphicsOpacityEffect();
    op2->setOpacity(0.25f);

    QGraphicsOpacityEffect* circleOp1 = new QGraphicsOpacityEffect();
    circleOp1->setOpacity(0.5f);

    QGraphicsOpacityEffect* circleOp2 = new QGraphicsOpacityEffect();
    circleOp2->setOpacity(0.25f);

    QGraphicsDropShadowEffect* ds1 = new QGraphicsDropShadowEffect();
    ds1->setOffset(0,0);
    ds1->setColor(QColor("#d11784"));
    ds1->setBlurRadius(10);

    polyItem->setGraphicsEffect(ds1);

    polyItem2->setGraphicsEffect(op1);
    polyItem3->setGraphicsEffect(op2);

    circle2->setGraphicsEffect(circleOp1);
    circle3->setGraphicsEffect(circleOp2);


    initStyles();
}

void PolyVisualizer::updateParameters()
{
    transferParametersFromDialog();
    initStyles();

}

void PolyVisualizer::openOptionsDialog()
{
    dialog = new PolyVisualizerOptionsDialog();


    transferParametersToDialog();
    //Apply above options to ui elements
    dialog->initOptions();

    QObject::connect(dialog, SIGNAL(changesMade()), this, SLOT(updateParameters()));

    dialog->show();
    /*
    if (dialog->exec() == QDialog::Accepted){
        qDebug() << "ACCEPT";

        transferParametersFromDialog();

    }
    delete dialog;
    */
}

void PolyVisualizer::initStyles()
{
    scene->setBackgroundBrush(QBrush("#191919"));

    polyItem->graphicsEffect()->setEnabled(glow);

    if (fillVisualizer)
        polyItem->setBrush(QBrush("#d11784"));
    else
        polyItem->setBrush(Qt::NoBrush);

    polyItem->setPen(QPen(QColor("#d11784")));
    polyItem2->setPen(QPen(QColor("#d11784")));
    polyItem3->setPen(QPen(QColor("#d11784")));
    //polyItem2->setPen(QPen(QColor("#b11370")));
    //polyItem3->setPen(QPen(QColor("#840e53")));

    invPolyItem->setPen(QPen(QColor("#4caf50")));

    //circle->setBrush(QBrush("#444341"));
    //circle->setPen(QPen(QColor("#444341")));

    //circle->setBrush(QBrush("#4caf50"));
    circle->setPen(QPen(QColor("#4caf50")));
    circle2->setPen(QPen(QColor("#4caf50")));
    circle3->setPen(QPen(QColor("#4caf50")));
}

void PolyVisualizer::transferParametersFromDialog()
{
    totalPoints = dialog->totalPoints;
    minRadius = dialog->minRadius;
    angleOffset = dialog->angleOffset;
    gain = dialog->gain;


    useLogScale = dialog->useLogScale;
    mirror = dialog->mirror; //Works best when factor of totalPoints
    alternateMirror = dialog->alternateMirror;
    rotate = dialog->rotate;
    showInvSpectrum = dialog->showInvSpectrum;
    glow = dialog->glow;

    fillVisualizer = dialog->fillVisualizer;
}

void PolyVisualizer::transferParametersToDialog()
{
    dialog->totalPoints = totalPoints;
    dialog->minRadius = minRadius;
    dialog->angleOffset = angleOffset;
    dialog->gain = gain;


    dialog->useLogScale = useLogScale;
    dialog->mirror = mirror; //Works best when factor of totalPoints
    dialog->alternateMirror = alternateMirror;
    dialog->rotate = rotate;
    dialog->showInvSpectrum = showInvSpectrum;
    dialog->glow = glow;

    dialog->fillVisualizer = fillVisualizer;
}

void PolyVisualizer::plotSpectrum(std::vector<float> data)
{
    poly.clear();

    //Offset to rotate entire visulaization
    if (rotate)
        angleOffset = qDegreesToRadians(0.1f) + angleOffset;
    else
        angleOffset = qDegreesToRadians(90.0f);


    //Calculate the angle between each point (angle increment size)
    float dAngle = qDegreesToRadians(360.0f / (float)totalPoints);

    //Ratio of the total number of data points given in the freq
    //spectrum data and the total number of points in the visualizer
    //If there are less points in the visualizer than in the data spectrum
    //we will use the average ofer a chunk as the value for the visual point
    int chunkSize = data.size() / totalPoints;

    for (int i = 0; i < totalPoints; i++)
    {
        //If mirrored index starts counting down after the halfway point
        int index;

        if (alternateMirror)
        {
            if ((i / (totalPoints/mirror)) % 2 == 0 && alternateMirror)//If even
                index = i % (totalPoints/mirror);
            else
                index = (totalPoints/mirror) - i % (totalPoints/mirror);
        }
        else
            index = index = i % (totalPoints/mirror);

        //Same as above in one line
        //index = ((i / (totalPoints/4)) % 2 == 0) * (totalPoints/4) - i % (totalPoints/4);

        /*
        if (i > totalPoints/2 && mirror == 1)
            index = totalPoints - i;
        else if (mirror == 3)
        {
            if ((i / (totalPoints/4)) % 2 == 0)//If even
                index = i % (totalPoints/4);
            else
                index = (totalPoints/4) - i % (totalPoints/4);

            //Same as above in one line
            //index = ((i / (totalPoints/4)) % 2 == 0) * (totalPoints/4) - i % (totalPoints/4);
        }
        else if (mirror == 4)
        {
            if(i > totalPoints/4.0f && i < totalPoints/2.0f )
                index = totalPoints/2 - i;
            else if(i >= totalPoints/2 && i < 0.75f*totalPoints )
                index = i - totalPoints/2;
            else if(i > 0.75f*totalPoints)
                index = totalPoints - i;
            else
                index = i;
        }
        else
            index = i;
        */

        //Calculate the current angle
        float angle = (dAngle * i) - angleOffset;

        //Take the average of all the data points within
        //one chunk of the spectrum
        float accumulator = 0;
        for (int j = 0; j < chunkSize; j++)
        {

            if (useLogScale)
                accumulator += data[50*log((index*chunkSize + j + 1))];
            else
                accumulator += data[index*chunkSize + j];

        }
        float avg = accumulator / chunkSize;
        //int newRadius = minRadius + 100 * data[i];
        //int newRadius = minRadius + 500 * avg * log(i + 1);
        int newRadius = minRadius + 100 * avg * gain;

        QPoint point(newRadius * cos(angle), newRadius * sin(angle));

        poly << point;
    }

    polyItem->setPolygon(poly);
    polyItem2->setPolygon(poly);
    polyItem3->setPolygon(poly);


    //Calculate inner circles scale based on the beat

    //To find beat take average of the lower frequencies
    float accumulator = 0;
    for (int i = 0; i < data.size()/10; i++)
    {

        accumulator += data[i];

    }
    float avg;
    if (!data.empty())
        avg = accumulator / (data.size()/10);
    else
        avg = 0;


    circle2->setScale(15 * log(avg * 0.9 + 1)); //0.9max
    circle3->setScale(10 * log(avg * 0.8 + 1)); //0.8max

    //Plot inverted poly
    plotInvPoly(data);
}

void PolyVisualizer::plotInvPoly(std::vector<float> data)
{
    if(!showInvSpectrum){
        invPolyItem->hide();
        return;
    }
    else
        invPolyItem->show();

    invPoly.clear();

    //Offset to rotate entire visulaization
    if (rotate)
        angleOffset = qDegreesToRadians(0.1f) + angleOffset;
    else
        angleOffset = qDegreesToRadians(90.0f);


    //Calculate the angle between each point (angle increment size)
    float dAngle = qDegreesToRadians(360.0f / (float)totalPoints);

    //Ratio of the total number of data points given in the freq
    //spectrum data and the total number of points in the visualizer
    //If there are less points in the visualizer than in the data spectrum
    //we will use the average ofer a chunk as the value for the visual point
    int chunkSize = data.size() / totalPoints;

    for (int i = 0; i < totalPoints; i++)
    {
        //If mirrored index starts counting down after the halfway point
        int index;

        if (alternateMirror)
        {
            if ((i / (totalPoints/mirror)) % 2 == 0 && alternateMirror)//If even
                index = i % (totalPoints/mirror);
            else
                index = (totalPoints/mirror) - i % (totalPoints/mirror);
        }
        else
            index = index = i % (totalPoints/mirror);



        //Calculate the current angle
        float angle = (dAngle * i) - angleOffset;

        //Take the average of all the data points within
        //one chunk of the spectrum
        float accumulator = 0;
        for (int j = 0; j < chunkSize; j++)
        {

            if (useLogScale)
                accumulator += data[50*log((index*chunkSize + j + 1))];
            else
                accumulator += data[index*chunkSize + j];

        }
        float avg = accumulator / chunkSize;
        //int newRadius = minRadius + 100 * data[i];
        //int newRadius = minRadius + 500 * avg * log(i + 1);
        int newRadius = minRadius - 50 * avg * gain;
        if (newRadius <= 0)
            newRadius = 5;

        QPoint point(newRadius * cos(angle), newRadius * sin(angle));

        invPoly << point;
    }

    invPolyItem->setPolygon(invPoly);


}


