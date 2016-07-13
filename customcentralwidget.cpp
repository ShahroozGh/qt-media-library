#include "customcentralwidget.h"
#define EDGE_WIDTH 5

CustomCentralWidget::CustomCentralWidget(QWidget *parent) : QWidget(parent)
{
    this->setMouseTracking(true);


}

void CustomCentralWidget::mousePressEvent(QMouseEvent *event)
{
    //Store local and global clicked coordinates
    localClickedPos = event->pos();
    globalClickedPos = event->globalPos();

    //Determine if mouse is along one of the edges or corners
    //If so change the cursor to the apropriate size cursor
    if (event->pos().y() <= EDGE_WIDTH && event->pos().x() <= EDGE_WIDTH){
        regionClicked = ClickRegionState::TopLeftDrag;
        setCursor(Qt::SizeFDiagCursor);

    }
    else if(event->pos().y() <= EDGE_WIDTH && event->pos().x() >= window()->rect().right() - EDGE_WIDTH){
        regionClicked = ClickRegionState::TopRightDrag;
        setCursor(Qt::SizeBDiagCursor);
    }
    else if(event->pos().y() >= window()->rect().bottom() - EDGE_WIDTH && event->pos().x() <= EDGE_WIDTH){
        regionClicked = ClickRegionState::BottomLeftDrag;
        setCursor(Qt::SizeBDiagCursor);
    }
    else if(event->pos().y() >= window()->rect().bottom() - EDGE_WIDTH && event->pos().x() >= window()->rect().right() - EDGE_WIDTH) {
        regionClicked = ClickRegionState::BottomRightDrag;
        setCursor(Qt::SizeFDiagCursor);
    }
    else if (event->pos().y() <= EDGE_WIDTH){
        regionClicked = ClickRegionState::TopDrag;
        setCursor(Qt::SizeVerCursor);
    }
    else if(event->pos().y() >= window()->rect().bottom() - EDGE_WIDTH){
        regionClicked = ClickRegionState::BottomDrag;
        setCursor(Qt::SizeVerCursor);
    }
    else if(event->pos().x() <= EDGE_WIDTH){
        regionClicked = ClickRegionState::LeftDrag;
        setCursor(Qt::SizeHorCursor);
    }
    else if(event->pos().x() >= window()->rect().right() - EDGE_WIDTH){
        regionClicked = ClickRegionState::RightDrag;
        setCursor(Qt::SizeHorCursor);
    }


}

void CustomCentralWidget::mouseReleaseEvent(QMouseEvent *event)
{
    //Reset states
     setCursor(Qt::ArrowCursor);

     regionClicked = ClickRegionState::None;
}

void CustomCentralWidget::mouseMoveEvent(QMouseEvent *event)
{
    //Determine if mouse is along one of the edges or corners
    //If so change the cursor to the apropriate size cursor
    //While in this region make all children transparent to mouse events
    //so that this widget can still recieve the hover events needed to
    //reset the cursor to default
    if (event->buttons() == Qt::NoButton){
        //Hover event
        if (event->pos().y() <= EDGE_WIDTH && event->pos().x() <= EDGE_WIDTH){
            regionClicked = ClickRegionState::TopLeftDrag;
            setCursor(Qt::SizeFDiagCursor);
            makeChildrenTransparent(true);
        }
        else if(event->pos().y() <= EDGE_WIDTH && event->pos().x() >= window()->rect().right() - EDGE_WIDTH){
            regionClicked = ClickRegionState::TopRightDrag;
            setCursor(Qt::SizeBDiagCursor);
            makeChildrenTransparent(true);
        }
        else if(event->pos().y() >= window()->rect().bottom() - EDGE_WIDTH && event->pos().x() <= EDGE_WIDTH){
            regionClicked = ClickRegionState::BottomLeftDrag;
            setCursor(Qt::SizeBDiagCursor);
            makeChildrenTransparent(true);
        }
        else if(event->pos().y() >= window()->rect().bottom() - EDGE_WIDTH && event->pos().x() >= window()->rect().right() - EDGE_WIDTH) {
            regionClicked = ClickRegionState::BottomRightDrag;
            setCursor(Qt::SizeFDiagCursor);
            makeChildrenTransparent(true);
        }
        else if (event->pos().y() <= EDGE_WIDTH){
            regionClicked = ClickRegionState::TopDrag;
            setCursor(Qt::SizeVerCursor);
            makeChildrenTransparent(true);
        }
        else if(event->pos().y() >= window()->rect().bottom() - EDGE_WIDTH){
            regionClicked = ClickRegionState::BottomDrag;
            setCursor(Qt::SizeVerCursor);
            makeChildrenTransparent(true);
        }
        else if(event->pos().x() <= EDGE_WIDTH){
            regionClicked = ClickRegionState::LeftDrag;
            setCursor(Qt::SizeHorCursor);
            makeChildrenTransparent(true);
        }
        else if(event->pos().x() >= window()->rect().right() - EDGE_WIDTH){
            regionClicked = ClickRegionState::RightDrag;
            setCursor(Qt::SizeHorCursor);
            makeChildrenTransparent(true);
        }
        else{
            //If we are not in the border region reset cursor and make children
            //opaque to mouse events so that they can be interacted with again
            setCursor(Qt::ArrowCursor);
            makeChildrenTransparent(false);
        }

    }
    else{
        //Perform window resizing
        if (regionClicked == ClickRegionState::TopDrag){
            QSize newSize;
            QPoint startPoint = globalClickedPos;
            QPoint endPoint = event->globalPos();

            QPoint diff = startPoint - endPoint;

            newSize.setHeight(window()->size().height() + diff.y());
            newSize.setWidth(window()->width());

            if (newSize.height() < window()->minimumHeight())
                return;

            //Move the window to where the mouse moved
            window()->move(window()->pos().x(), endPoint.y());

            //Resize window
            window()->resize(newSize);

            //Store next position
            globalClickedPos = event->globalPos();

        }
        else if(regionClicked == ClickRegionState::BottomDrag){
            QSize newSize;
            QPoint startPoint = globalClickedPos;
            QPoint endPoint = event->globalPos();

            QPoint diff = endPoint - startPoint;

            newSize.setHeight(window()->size().height() + diff.y());
            newSize.setWidth(window()->width());


            if (newSize.height() < window()->minimumHeight())
                return;

            //Move the window to where the mouse moved NOT NEEDED
            //window()->move(window()->pos().x(), endPoint.y());

            //Resize window
            window()->resize(newSize);

            //Store next position
            globalClickedPos = event->globalPos();
        }
        else if (regionClicked == ClickRegionState::LeftDrag){
            QSize newSize;
            QPoint startPoint = globalClickedPos;
            QPoint endPoint = event->globalPos();

            QPoint diff = startPoint - endPoint;

            newSize.setWidth(window()->size().width() + diff.x());
            newSize.setHeight(window()->height());

            if (newSize.width() < window()->minimumWidth())
                return;

            //Move the window to where the mouse moved
            window()->move(endPoint.x(), window()->pos().y());

            //Resize window
            window()->resize(newSize);

            //Store next position
            globalClickedPos = event->globalPos();

        }
        else if(regionClicked == ClickRegionState::RightDrag){
            QSize newSize;
            QPoint startPoint = globalClickedPos;
            QPoint endPoint = event->globalPos();

            QPoint diff = endPoint - startPoint;

            newSize.setWidth(window()->size().width() + diff.x());
            newSize.setHeight(window()->height());


            if (newSize.width() < window()->minimumWidth())
                return;

            //Move the window to where the mouse moved NOT NEEDED
            //window()->move(window()->pos().x(), endPoint.y());

            //Resize window
            window()->resize(newSize);

            //Store next position
            globalClickedPos = event->globalPos();
        }
        else if(regionClicked == ClickRegionState::BottomRightDrag){
            QSize newSize;
            QPoint startPoint = globalClickedPos;
            QPoint endPoint = event->globalPos();

            QPoint diff = endPoint - startPoint;

            newSize = QSize(window()->width() + diff.x(), window()->height() + diff.y());


            //Dont resize unless mouse position causes it to grow past minimumWidth/height
            if (newSize.width() < window()->minimumWidth()){
                newSize.setWidth(window()->width());

            }
            else{
                globalClickedPos.setX(event->globalPos().x());

            }
            if (newSize.height() < window()->minimumHeight()){
                newSize.setHeight(window()->height());

            }
            else{
                globalClickedPos.setY(event->globalPos().y());

            }

            //Move the window to where the mouse moved NOT NEEDED
            //window()->move(window()->pos().x(), endPoint.y());

            //Resize window
            window()->resize(newSize);

            //Store next position
            //globalClickedPos = event->globalPos();
        }
        else if(regionClicked == ClickRegionState::BottomLeftDrag){
            QSize newSize;
            QPoint startPoint = globalClickedPos;
            QPoint endPoint = event->globalPos();

            QPoint diff = endPoint - startPoint;

            newSize = QSize(window()->width() - diff.x(), window()->height() + diff.y());


            QPoint newWindowPos;
            //Dont resize unless mouse position causes it to grow past minimumWidth/height
            if (newSize.width() < window()->minimumWidth()){
                newSize.setWidth(window()->width());
                newWindowPos.setX(window()->pos().x());
            }
            else{
                globalClickedPos.setX(event->globalPos().x());
                newWindowPos.setX(endPoint.x());

            }
            if (newSize.height() < window()->minimumHeight()){
                newSize.setHeight(window()->height());
                newWindowPos.setY(window()->pos().y());
            }
            else{
                globalClickedPos.setY(event->globalPos().y());
                newWindowPos.setY(window()->pos().y());
            }

            //Move the window to where the mouse moved NOT NEEDED
            window()->move(newWindowPos);

            //Resize window
            window()->resize(newSize);

            //Store next position
            //globalClickedPos = event->globalPos();
        }
        else if(regionClicked == ClickRegionState::TopRightDrag){
            QSize newSize;
            QPoint startPoint = globalClickedPos;
            QPoint endPoint = event->globalPos();

            QPoint diff = endPoint - startPoint;

            newSize = QSize(window()->width() + diff.x(), window()->height() - diff.y());


            QPoint newWindowPos;
            //Dont resize unless mouse position causes it to grow past minimumWidth/height
            if (newSize.width() < window()->minimumWidth()){
                newSize.setWidth(window()->width());
                newWindowPos.setX(window()->pos().x());
            }
            else{
                globalClickedPos.setX(event->globalPos().x());
                newWindowPos.setX(window()->pos().x());

            }
            if (newSize.height() < window()->minimumHeight()){
                newSize.setHeight(window()->height());
                newWindowPos.setY(window()->pos().y());
            }
            else{
                globalClickedPos.setY(event->globalPos().y());
                newWindowPos.setY(endPoint.y());
            }

            //Move the window to where the mouse moved NOT NEEDED
            window()->move(newWindowPos);

            //Resize window
            window()->resize(newSize);

            //Store next position
            //globalClickedPos = event->globalPos();
        }
        else if(regionClicked == ClickRegionState::TopLeftDrag){
            QSize newSize;
            QPoint startPoint = globalClickedPos;
            QPoint endPoint = event->globalPos();

            QPoint diff = endPoint - startPoint;

            newSize = QSize(window()->width() - diff.x(), window()->height() - diff.y());


            QPoint newWindowPos;
            //Dont resize unless mouse position causes it to grow past minimumWidth/height
            if (newSize.width() < window()->minimumWidth()){
                newSize.setWidth(window()->width());
                newWindowPos.setX(window()->pos().x());
            }
            else{
                globalClickedPos.setX(event->globalPos().x());
                newWindowPos.setX(endPoint.x());

            }
            if (newSize.height() < window()->minimumHeight()){
                newSize.setHeight(window()->height());
                newWindowPos.setY(window()->pos().y());
            }
            else{
                globalClickedPos.setY(event->globalPos().y());
                newWindowPos.setY(endPoint.y());
            }

            //Move the window to where the mouse moved NOT NEEDED
            window()->move(newWindowPos);

            //Resize window
            window()->resize(newSize);

            //Store next position
            //globalClickedPos = event->globalPos();
        }
        else{

            return;
        }

    }


}

//Set all child widgets to be transparent to mouse events
//This will allow this widget to recieve mouse hover events when needed
void CustomCentralWidget::makeChildrenTransparent(bool transparent)
{
    QObjectList children = this->children();

    foreach (QObject* child, children){
        ((QWidget*)child)->setAttribute(Qt::WA_TransparentForMouseEvents, transparent);

    }
}
