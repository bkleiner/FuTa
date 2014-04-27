#ifndef WOWFILEMODEL_H
#define WOWFILEMODEL_H

#include <QAbstractItemModel>
#include "wowfile.h"
#include "wowfileitem.h"

class WoWFileModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit WoWFileModel(QObject *parent = 0);
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    QModelIndex parent(const QModelIndex &child) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    void addWoWFile(WoWFile *file);

private:
    WoWFileItem *firstItem;
signals:
    
public slots:
    
};

#endif // WOWFILEMODEL_H
