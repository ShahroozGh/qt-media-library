#ifndef CUSTOMCENTRALWIDGET_H
#define CUSTOMCENTRALWIDGET_H

#include <QWidget>
#include <QMouseEvent>

enum ClickRegionState {LeftDrag, RightDrag, TopDrag, BottomDrag, TopLeftDrag, TopRightDrag, BottomLeftDrag, BottomRightDrag, None};

class CustomCentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CustomCentralWidget(QWidget *parent = 0);



signals:

public slots:


protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QPoint localClickedPos, globalClickedPos;
    QSize oldWindowSize;
    ClickRegionState regionClicked = ClickRegionState::None;
    bool resizeClicked = false;

    void makeChildrenTransparent(bool transparent);



};

#endif // CUSTOMCENTRALWIDGET_H
