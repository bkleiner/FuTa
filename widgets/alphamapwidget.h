#ifndef ALPHAMAPWIDGET_H
#define ALPHAMAPWIDGET_H

#include <QWidget>
#include <QtGui>
#include "widgets/wowfilewidget.h"

class MapTile;

class alphamapWidget : public WoWFileWidget
{
    Q_OBJECT
public:
    explicit alphamapWidget(MapTile* file, QWidget *parent = 0);

    virtual void reload();

private:
    MapTile* adtFile;
    QTabWidget *tabs;
    QList<QImage*> alphamaps;
    QPushButton *exportButton;
    QPushButton *importButton;
    QSettings settings;

    void renderAlpha(QImage *pic, int x, int y, int layer);
    void drawPicToAlpha(QImage *pic, int layer);
    void drawAlphaToPic(QImage *pic, int layer);

    QWidget* createLayerTab(int layer);

    
signals:
    
public slots:
    void exportPNG();
    void importPNG();
    
};

#endif // ALPHAMAPWIDGET_H
