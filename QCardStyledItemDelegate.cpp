#include "QCardStyledItemDelegate.h"

void QCardStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    //Set relevant geometry for convenience
    QSize infoBoxSize = option.rect.size() - option.decorationSize;
    QRect itemBounds = option.rect;
    QPoint primTextPos = QPoint(option.rect.topLeft().x() + 10, option.rect.topLeft().y() + option.decorationSize.height() + 0.5 * infoBoxSize.height());
    QPoint secTextPos = QPoint(option.rect.topLeft().x() + 10, option.rect.topLeft().y() + option.decorationSize.height() + 0.75 * infoBoxSize.height());
    QRect pixmapBounds = QRect(option.rect.topLeft().x(), option.rect.topLeft().y(), option.decorationSize.width(), option.decorationSize.height());
    QRect innerTextBounds = QRect(option.rect.left() + 10, option.rect.top(), option.rect.width() - 20, option.rect.height());

    QFont primFont = QFont("Roboto Lt");
    QFontMetrics fm(primFont);


    QString albumTitle = index.data(Qt::DisplayRole).toString();
    QString artist = index.data(Qt::UserRole + 1).toString();

    QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
    QPixmap albumCover = icon.pixmap(option.decorationSize.width(), option.decorationSize.height());
    //QPixmap albumCover = qvariant_cast<QPixmap>(index.data(Qt::DecorationRole));


    //QStyledItemDelegate::paint(painter, option, index);



    QPen pen(Qt::green, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    painter->setRenderHint(QPainter::TextAntialiasing, true);


    //Paint Background
    //QBrush bg = option.palette.background();
    QBrush bg;
    if (option.state & QStyle::State_MouseOver)
        bg = QBrush(QColor("#0000FF"));
    else
        bg = QBrush(QColor("#212121"));

    pen.setColor(QColor("#262321"));

    painter->setBrush(bg);
    painter->setPen(pen);

    painter->fillRect(itemBounds, bg);


    //Paint album pixmap
    painter->drawPixmap(pixmapBounds, albumCover);

    //Paint border
    painter->setBrush(QBrush(QColor(0,0,0,0)));
    painter->drawRect(itemBounds);
    //Paint text
    //QSize infoBoxSize = option.rect.size() - option.decorationSize;

    pen.setColor(QColor("#f6f6f6"));
    painter->setPen(pen);
    //Font properties
    //QFont primFont = option.font;

    primFont.setPointSize(11);
    primFont.setStyleStrategy(QFont::PreferAntialias);
    painter->setFont(primFont);
    //qDebug() << primFont.rawName();


    //Set clipping mask for text so that it fits in box
    painter->drawRect(innerTextBounds);
    painter->setClipRect(innerTextBounds, Qt::ReplaceClip);


    //If not hovered show elided text
    albumTitle = fm.elidedText(albumTitle, Qt::ElideRight, innerTextBounds.width());


    painter->drawText(primTextPos, albumTitle);

    painter->setClipRect(innerTextBounds, Qt::NoClip);
    //painter->setClipping(false);

    painter->drawText(secTextPos, artist);





    //Debug rect
    //pen.setColor(Qt::green);
    //painter->setPen(pen);
    //painter->drawRect(option.rect);

    /*
    QSize sz= QStyledItemDelegate::sizeHint(option, index);
    QPen pen2(Qt::red, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

    painter->setPen(pen2);
    painter->drawRect(option.rect.topLeft().x(), option.rect.topLeft().y(), sz.width(), sz.height());
    */
}

QSize QCardStyledItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize hint;
    int x = option.decorationSize.width();
    int y = option.decorationSize.height() + 75;

    hint.setWidth(x);
    hint.setHeight(y);

    return hint;
}
