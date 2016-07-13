#ifndef CUSTOMTITLEBAR_H
#define CUSTOMTITLEBAR_H

#include <QWidget>
#include <QMouseEvent>
#include <QStyleOption>
#include <QPainter>

#include <QDebug>

class CustomTitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit CustomTitleBar(QWidget *parent = 0);

signals:

public slots:

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent* event);

private:
    QPoint clickedPos, globalClickedPos;
    QSize oldWindowSize;
    bool justMinimized = false;
    bool resizeClicked = false;
};

#endif // CUSTOMTITLEBAR_H
