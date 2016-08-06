#ifndef CUSTOMQSLIDER_H
#define CUSTOMQSLIDER_H

#include <QObject>
#include <QWidget>
#include <QSlider>
#include <QPaintEvent>
#include <QPainter>
#include <QLine>
#include <QStyle>
#include <QStyleOptionSlider>


class CustomQSlider : public QSlider
{
public:
    CustomQSlider(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent* pe);
};

#endif // CUSTOMQSLIDER_H
