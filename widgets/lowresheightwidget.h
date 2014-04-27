#ifndef LOWRESHEIGHTWIDGET_H
#define LOWRESHEIGHTWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTableView>
#include <QPushButton>
#include <QStandardItemModel>
#include "wowfilewidget.h"
#include "file/wdlfile.h"

class lowResHeightWidget : public WoWFileWidget
{
    Q_OBJECT
public:
    lowResHeightWidget(WdlFile* file, QWidget *parent = 0);

signals:

public slots:
    void selectAdtFiles();
    void generateData();

private:
    WdlFile *wdlFile;
    QVBoxLayout *layout;
    QTableView *adtTable;
    QPushButton *adtButton;
    QPushButton *apply;
    QStandardItemModel *adtModel;
    QStringList fileNames;

};

#endif // LOWRESHEIGHTWIDGET_H
