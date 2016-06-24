#include "QCategoryFilterProxyModel.h"

QCategoryFilterProxyModel::QCategoryFilterProxyModel()
{

}

QCategoryFilterProxyModel::QCategoryFilterProxyModel(QObject *parent): QSortFilterProxyModel(parent)
{
}

bool QCategoryFilterProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex index = sourceModel()->index(sourceRow, filterKeyColumn(), sourceParent);
    //qDebug() << "QFILTER: " << filterKeyColumn() << " " << index.data().toString();
    if(!(uniqueCategories.contains(index.data().toString())))
    {

        const_cast<QCategoryFilterProxyModel *>(this)->uniqueCategories.insert(index.data().toString());
        return true;
    }
    else
    {
        return false;
    }


}

bool QCategoryFilterProxyModel::filterAcceptsColumn(int sourceColumn, const QModelIndex &sourceParent) const
{
    if (sourceColumn != 2)
        return false;
    else
        return true;
}

void QCategoryFilterProxyModel::invalidateFilter()
{
    uniqueCategories.clear();
}
