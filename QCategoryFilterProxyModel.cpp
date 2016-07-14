#include "QCategoryFilterProxyModel.h"

QCategoryFilterProxyModel::QCategoryFilterProxyModel()
{

}

QCategoryFilterProxyModel::QCategoryFilterProxyModel(QObject *parent): QSortFilterProxyModel(parent)
{
}

QVariant QCategoryFilterProxyModel::data(const QModelIndex &idx, int role) const
{
    //Artist role
    if(role == Qt::UserRole + 1){
        //get artist index from source model
        QModelIndex artistIndex = sourceModel()->index(mapToSource(idx).row(), 1);
        //return artist name
        return artistIndex.data().toString();
    }
    else if(role == Qt::DecorationRole){
        //get art index from source model
        QModelIndex artIndex = sourceModel()->index(mapToSource(idx).row(), 5);
        //return icon from art column
        return artIndex.data(Qt::DecorationRole);
    }
    else{
        //Else return default values for other roles
        return QSortFilterProxyModel::data(idx, role);
    }
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
    //Change this for variable column choice
    if (sourceColumn != filterKeyColumn())
        return false;
    else
        return true;
}

void QCategoryFilterProxyModel::invalidateFilter()
{
    uniqueCategories.clear();
}
