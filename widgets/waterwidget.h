#ifndef WATERWIDGET_H
#define WATERWIDGET_H

#include <QWidget>
#include <QtGui>
#include "file/maptile.h"
#include "wowfilewidget.h"

class WaterWidget : public WoWFileWidget
{
    Q_OBJECT
public:
    explicit WaterWidget(MapTile* file, QWidget *parent = 0);

private:
    QPushButton *saveButton;
    QPushButton *importButton;
    MapTile* adtFile;
    
signals:
    
public slots:
    void importFile();
    void exportFile();
    
};

#endif // WATERWIDGET_H
