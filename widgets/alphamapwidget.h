#ifndef ALPHAMAPWIDGET_H
#define ALPHAMAPWIDGET_H

#include <QWidget>
#include <QtGui>
#include "file/adtfile.h"
#include "widgets/wowfilewidget.h"


class alphamapWidget : public WoWFileWidget
{
    Q_OBJECT
public:
    explicit alphamapWidget(AdtFile* file, QWidget *parent = 0);

    virtual void reload();

private:
    AdtFile* adtFile;
    QTabWidget *tabs;
    QList<QImage*> alphamaps;
    QPushButton *saveButton;
    QPushButton *importButton;
    QSettings settings;

    void renderAlpha(QImage *pic, int x, int y, int layer);
    void drawPicToAlpha(QImage *pic, int layer);
    void drawAlphaToPic(QImage *pic, int layer);

    QWidget* createLayerTab(int layer);

    
signals:
    
public slots:
    void saveAlphamaps();
    void importPNG();
    
};

#endif // ALPHAMAPWIDGET_H
