#include "wowfilemodel.h"

WoWFileModel::WoWFileModel(QObject *parent) :
    QAbstractItemModel(parent)
{
    firstItem = new WoWFileItem("File");
}

int WoWFileModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

int WoWFileModel::rowCount(const QModelIndex &parent) const
{
    WoWFileItem *parentItem;

    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parentItem = firstItem;
    else
        parentItem = static_cast<WoWFileItem*>(parent.internalPointer());

    return parentItem->getChildCount();
}

QVariant WoWFileModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    WoWFileItem *item = static_cast<WoWFileItem*>(index.internalPointer());
    return item->getData();


}

Qt::ItemFlags WoWFileModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QModelIndex WoWFileModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    WoWFileItem *parentItem;

    if (!parent.isValid())
        parentItem = firstItem;
    else
        parentItem = static_cast<WoWFileItem*>(parent.internalPointer());

    WoWFileItem *childItem = parentItem->getChild(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex WoWFileModel::parent(const QModelIndex &child) const
{
    if (!child.isValid())
        return QModelIndex();

    WoWFileItem *childItem = static_cast<WoWFileItem*>(child.internalPointer());
    WoWFileItem *parentItem = childItem->getParent();

    if (parentItem == firstItem)
        return QModelIndex();

    return createIndex(parentItem->row(), 0, parentItem);

}

QVariant WoWFileModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole && orientation == Qt::Horizontal && section == 0)
        return firstItem->getData();
    return QVariant();
}

void WoWFileModel::addWoWFile(WoWFile *file)
{
    beginInsertRows(QModelIndex(),0,1);
    firstItem->addChild(new WoWFileItem(file,firstItem));
    endInsertRows();
}
