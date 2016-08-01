#include "customtitlebar.h"
#define EDGE_WIDTH 5

CustomTitleBar::CustomTitleBar(QWidget *parent) : QWidget(parent)
{
    this->setMouseTracking(false);
}

void CustomTitleBar::mousePressEvent(QMouseEvent *event)
{
    clickedPos = event->pos();
    oldWindowSize = window()->size();
    globalClickedPos = event->globalPos();

    if (event->pos().y() <= EDGE_WIDTH){
        event->ignore();
        resizeClicked = true;
        //setCursor(Qt::SizeVerCursor);
        return;

    }
}

void CustomTitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->globalPos().y() == 0)
        window()->showMaximized();

    //setCursor(Qt::ArrowCursor);

    resizeClicked = false;

    event->ignore();
}

void CustomTitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() == Qt::NoButton){
        event->ignore();
        return;
    }
    if (resizeClicked){
        event->ignore();
        return;

    }
    if (event->pos().y() <= EDGE_WIDTH || clickedPos.y() <= EDGE_WIDTH){
        event->ignore();

        //setCursor(Qt::SizeVerCursor);
        return;

    }


    QPoint newPos = event->globalPos();


    //Note this gets messed up because the clicked pos becomes incorrect after resizing
    if (window()->isMaximized()){
        //qDebug() << clickedPos.x() << "," << clickedPos.y();
        window()->showNormal();
        //QPoint translatedPos = window()->pos();
        //translatedPos.setY(0);
        //window()->move(translatedPos);
        //Update clicked pos
        //clickedPos = event->pos();
        //qDebug() << clickedPos.x() << "," << clickedPos.y();
        //qDebug() << "MINIMIZED";
        justMinimized = true;
    }
    else{
        if (justMinimized){
            //Calculate new relitave mouse pos with newly resized window
            //Also after calling show normal window will probably be above the screen so we shift it down to y = 0 (i.e shift it to wherever the mouse clicked in the y direction)
            QPoint translatedPos = window()->pos();
            translatedPos.setX(0);

            //Calculate x shift
            //First get where along the window the mouse was located (in percentage)
            float percentSize = (float)clickedPos.x() / oldWindowSize.width();

            int shiftX = percentSize * window()->size().width();

            translatedPos.setX(shiftX - event->pos().x());

            clickedPos = event->pos() + translatedPos; //Calculate new relitave mouse pos with newly resized window
        }
        //Also want to shift to meet mouse at correct spot
        justMinimized = false;
        //qDebug() << "New:" << newPos.x() << "," << newPos.y();
       // qDebug() << "Clicked: " << clickedPos.x() << "," << clickedPos.y();
        window()->move(newPos - clickedPos);//Without clickedPos mouse will follow in top left corner always
    }


}


void CustomTitleBar::paintEvent(QPaintEvent *event)
{

     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}

