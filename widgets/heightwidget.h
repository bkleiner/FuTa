#ifndef HEIGHTWIDGET_H
#define HEIGHTWIDGET_H

#include <QWidget>
#include <QtGui>
#include "file/adtfile.h"
#include "wowfilewidget.h"

class HeightWidget : public WoWFileWidget
{
    Q_OBJECT
public:
    explicit HeightWidget(AdtFile* file, QWidget *parent = 0);

    void creatLayout();

    virtual void reload();

private:
    QPushButton *saveButton;
    QPushButton *importButton;
    QVBoxLayout *mainLayout;
    AdtFile *adtFile;
    QSettings settings;
    QLabel *label;
    
signals:
    
public slots:
    void importFile();
    void exportFile();
    
};

#endif // HEIGHTWIDGET_H
