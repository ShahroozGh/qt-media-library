#ifndef CUSTOMQDIAL_H
#define CUSTOMQDIAL_H

#include <QObject>
#include <QWidget>
#include <QDial>
#include <QPaintEvent>
#include <QPainter>
#include <QLine>

class CustomQDial : public QDial
{
    Q_OBJECT

public:
    CustomQDial(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent* pe);

private:
    void drawGrooves(QPoint center, int numberOfLines, float radius, float length, float angleOffset, QPainter &painter);
};

#endif // CUSTOMQDIAL_H
