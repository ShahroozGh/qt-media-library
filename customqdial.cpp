#include "customqdial.h"



CustomQDial::CustomQDial(QWidget *parent) : QDial(parent)
{


}

void CustomQDial::paintEvent(QPaintEvent *pe)
{
    float percentProgress = this->value()/100.0f;

    //Relative to percent of full circle radius
    float progressBarRelWidth = 0.3f;
    float edgeBuffer = 0.3f;
    float bgCircleRelRad = 0.95f;
    float progressBarRelRad = 1.0f;
    float innerCircleRelRad = 0.2f;

    int bgCircleRad;
    if (pe->rect().width() > pe->rect().height())
        bgCircleRad = pe->rect().height()/2;
    else
        bgCircleRad = pe->rect().width()/2;

    //Brushes and pens
    QBrush bgCircle;
    bgCircle.setColor(QColor("#212121"));
    bgCircle.setStyle(Qt::SolidPattern);

    QPen bgCirclePen;
    bgCirclePen.setColor(QColor("#181818"));
    bgCirclePen.setWidth(1);

    QBrush progressBar;
    progressBar.setColor(QColor("#FC575E"));

    QPen progressBarPen;
    QColor startColor("#FC575E");
    startColor.setHsv(startColor.hsvHue(), startColor.hsvSaturation() - percentProgress*50, startColor.value());

    progressBarPen.setColor(startColor);
    progressBarPen.setWidth(bgCircleRad*progressBarRelWidth);
    progressBarPen.setCapStyle(Qt::RoundCap);

    QBrush progressBarGroove;
    progressBarGroove.setColor(QColor("#000000"));

    QPen progressBarGroovePen;
    progressBarGroovePen.setColor(QColor("#000000"));
    progressBarGroovePen.setWidth(bgCircleRad*progressBarRelWidth);
    progressBarGroovePen.setCapStyle(Qt::RoundCap);




    //Calcuate bounding rectangles for circles
    QRect bgRect(pe->rect().center().x() - bgCircleRad * bgCircleRelRad,
                 pe->rect().center().y() - bgCircleRad * bgCircleRelRad,
                 2*bgCircleRad*bgCircleRelRad,
                 2*bgCircleRad*bgCircleRelRad);

    QRect pbRect(pe->rect().center().x() - bgCircleRad * (progressBarRelRad - edgeBuffer),
                 pe->rect().center().y() - bgCircleRad * (progressBarRelRad - edgeBuffer),
                 2*bgCircleRad*(progressBarRelRad - edgeBuffer),
                 2*bgCircleRad*(progressBarRelRad - edgeBuffer));




    //Properties


    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    //Draw background
    painter.setBrush(bgCircle);
    painter.setPen(bgCirclePen);
    //painter.drawEllipse(pe->rect());



    //painter.drawEllipse(pe->rect().center(), bgCircleRad, bgCircleRad);
    painter.drawEllipse(bgRect);

    //Draw progress bar

    painter.setBrush(progressBarGroove);
    painter.setPen(progressBarGroovePen);
    painter.drawEllipse(pbRect);

    //Calculate angle


    int startAngle = 16 * 270;
    int spanAngle = -16 * percentProgress * 360;

    painter.setBrush(progressBar);
    painter.setPen(progressBarPen);
    //painter.drawArc(QRect(pe->rect().center().x() - bgCircleRad, pe->rect().center().y() - bgCircleRad, 2*bgCircleRad, 2*bgCircleRad), startAngle, spanAngle);
    painter.drawArc(pbRect, startAngle, spanAngle);


    drawGrooves(pe->rect().center(), 16, bgCircleRad*innerCircleRelRad, 8.0f, -360.0f * percentProgress, painter);



}

void CustomQDial::drawGrooves(QPoint center, int numberOfLines, float radius, float length, float angleOffset, QPainter &painter)
{

    QBrush lineB;
    lineB.setColor(QColor("#000000"));

    QPen lineP;
    lineP.setColor(QColor("#151515"));
    lineP.setWidth(2);
    lineP.setCapStyle(Qt::RoundCap);

    painter.setBrush(lineB);
    painter.setPen(lineP);

    float angleInterval = 360.0f / numberOfLines;

    for (int i = 0; i < numberOfLines; i++){
        QLineF guideLine, line;
        guideLine.setP1(center);
        guideLine.setAngle(i*angleInterval + angleOffset);
        guideLine.setLength(radius);

        line.setP1(guideLine.p2());
        line.setAngle(i*angleInterval + angleOffset);
        line.setLength(length);

        //Draw line
        painter.drawLine(line);

    }

}
