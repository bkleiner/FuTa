#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtGui>
#include <QSettings>
#include "file/wowfile.h"
#include "file/wowfilemodel.h"
#include "widgets/wowfilewidget.h"

class widget : public QWidget
{
    Q_OBJECT
public:
    explicit widget(QWidget *parent = 0);
    bool addFile(WoWFile *wowfile);
    void loadWidget(WoWFileWidget *toLoad);

private:
    QGridLayout *layout;
    WoWFileWidget *dummyWidget;
    WoWFileModel *model;
    QTreeView *treeView;
    QVBoxLayout *dummylayout;
    WoWFileWidget *editWidget;
    
signals:
    
public slots:
    void changeWidget(QModelIndex index);
    void saveFile();
    void reloadWidgets();
    
};

#endif // WIDGET_H
