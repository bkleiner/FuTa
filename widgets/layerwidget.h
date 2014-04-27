#ifndef LAYERWIDGET_H
#define LAYERWIDGET_H

#include <QWidget>
#include <QtGui>
#include "file/adtfile.h"
#include "widgets/wowfilewidget.h"

class LayerWidget : public WoWFileWidget
{
    Q_OBJECT
public:
    explicit LayerWidget(AdtFile* file, QWidget *parent = 0);

    //virtual void reload();

private:
    QTableView *texTable;
    QStandardItemModel *texModel;
    QStringList texList;
    QPushButton *saveButton;
    QTabWidget *tabs;
    AdtFile* adtFile;

    QWidget* createLayerTab(int layer);
    
signals:
    
public slots:
    void contextMenu(QPoint loc);
    void save();
    void addTexture();
    void changeTexture(QStandardItem* item);
};

#endif // LAYERWIDGET_H
