#ifndef WATERWIDGET_H
#define WATERWIDGET_H

#include <QWidget>
#include <QtGui>
#include "file/adtfile.h"
#include "wowfilewidget.h"

class WaterWidget : public WoWFileWidget
{
    Q_OBJECT
public:
    explicit WaterWidget(AdtFile* file, QWidget *parent = 0);

private:
    QPushButton *saveButton;
    QPushButton *importButton;
    AdtFile* adtFile;
    
signals:
    
public slots:
    void importFile();
    void exportFile();
    
};

#endif // WATERWIDGET_H
