#ifndef QCARDSTYLEDITEMDELEGATE_H
#define QCARDSTYLEDITEMDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QDebug>

class QCardStyledItemDelegate : public QStyledItemDelegate
{
public:
    QCardStyledItemDelegate(QWidget *parent = 0) : QStyledItemDelegate(parent) {}

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const ;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const ;
    //QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const ;
    //void setEditorData(QWidget *editor, const QModelIndex &index) const ;
    //void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const ;

};

#endif // QCARDSTYLEDITEMDELEGATE_H
