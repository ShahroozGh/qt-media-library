#ifndef QCATEGORYFILTERPROXYMODEL_H
#define QCATEGORYFILTERPROXYMODEL_H

#include <QStandardItem>
#include <QSortFilterProxyModel>
#include <QSet>
#include <QDebug>


class QCategoryFilterProxyModel : public QSortFilterProxyModel
{
public:
    QCategoryFilterProxyModel();
    QCategoryFilterProxyModel(QObject *parent);

protected:
    QVariant data(const QModelIndex& idx, int role) const;
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    bool filterAcceptsColumn(int sourceColumn, const QModelIndex &sourceParent) const;
    void invalidateFilter();

private:
    QSet<QString> uniqueCategories;
};

#endif // QCATEGORYFILTERPROXYMODEL_H
