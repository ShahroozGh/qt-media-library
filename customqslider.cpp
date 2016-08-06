#include "customqslider.h"



CustomQSlider::CustomQSlider(QWidget *parent) : QSlider(parent)
{

}

void CustomQSlider::paintEvent(QPaintEvent *pe)
{

    QStyleOptionSlider opt;
    this->initStyleOption(&opt);

    QStyle *styl=style();
    QRect rectHandle=styl->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderHandle, NULL);
    QRect rectGroove=styl->subControlRect(QStyle::CC_Slider, &opt, QStyle::SC_SliderGroove, NULL);

    float percentProgress = this->value()/100.0f;

    int width = 10;
    int height = pe->rect().height();

    QRect grooveRect(pe->rect().center().x() - width/2, pe->rect().top(), width, height-1);

    QBrush grooveB;
    grooveB.setColor(QColor("#757581"));
    grooveB.setStyle(Qt::SolidPattern);

    QPen grooveP;
    grooveP.setColor(QColor("#757581"));
    grooveP.setWidth(1);

    QBrush handleB;
    handleB.setColor(QColor("#222222"));
    handleB.setStyle(Qt::SolidPattern);

    QPen handleP;
    handleP.setColor(QColor("#222222"));
    handleP.setWidth(1);


    QColor startColor("#FC575E");
    startColor.setHsv(startColor.hsvHue(), startColor.hsvSaturation() - abs(percentProgress - 0.5)*2*50, startColor.value());

    QBrush progressB;
    progressB.setColor(startColor);
    progressB.setStyle(Qt::SolidPattern);

    QPen progressP;
    progressP.setColor(startColor);
    progressP.setWidth(1);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //Draw groove
    //painter.setBrush(grooveB);
    //painter.setPen(grooveP);
    //painter.drawRect(grooveRect);
    //painter.drawRect(pe->rect());

    painter.setBrush(grooveB);
    painter.setPen(grooveP);
    painter.drawRoundedRect(grooveRect, 4, 4);




    //Draw progress
    QRect progressBar(pe->rect().center().x() - width/2.0f,
                      grooveRect.center().y(),
                      width,
                      0.0f);
    if (percentProgress < 0.5f){
        progressBar.setBottom(grooveRect.center().y() + (0.5f - percentProgress) * grooveRect.height());

    }
    else{
        progressBar.setTop(grooveRect.center().y() - (percentProgress - 0.5f) * grooveRect.height());

    }


    painter.setBrush(progressB);
    painter.setPen(progressP);
    painter.drawRect(progressBar);



    //Handle
    painter.setPen(handleP);
    painter.setBrush(handleB);
    //painter.drawRect(rectHandle);

    painter.drawEllipse(rectHandle.center(), 10, 10);



}
