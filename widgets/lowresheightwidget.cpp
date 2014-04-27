#include "lowresheightwidget.h"

#include "futa.h"
#include "file/wowfilemanager.h"
#include "mainwindow.h"

#include <QFileDialog>
#include <QHeaderView>

lowResHeightWidget::lowResHeightWidget(WdlFile* file, QWidget *parent) :
    WoWFileWidget(parent)
  ,wdlFile(file)
{
    layout = new QVBoxLayout;
    adtTable = new QTableView(this);
    adtButton = new QPushButton(tr("Select adt files"),this);
    apply = new QPushButton(tr("Generate data"),this);

    QHBoxLayout *adtselect = new QHBoxLayout;
    adtselect->addWidget(adtTable);
    adtselect->addWidget(adtButton);
    layout->addLayout(adtselect);
    layout->addWidget(apply);
    setLayout(layout);

    connect(adtButton,SIGNAL(clicked()),this,SLOT(selectAdtFiles()));
    connect(apply,SIGNAL(clicked()),this,SLOT(generateData()));
}

void lowResHeightWidget::generateData()
{
    wdlFile->clearHeight();
    for(int i = 0; i < fileNames.size(); ++i)
    {
        AdtFile *adt = WoWFileManager::openAdt(fileNames.at(i));
        wdlFile->setHeight(i*1098,adt);
        FuTa::mainwindow()->showProgress(i+1,fileNames.size());
    }
}

void lowResHeightWidget::selectAdtFiles()
{
    fileNames = QFileDialog::getOpenFileNames(this
                                             ,"Select the Adts files to load data from"
                                             ,FuTa::getLastFiles()->first().left(FuTa::getLastFiles()->first().lastIndexOf("/"))
                                             ,"Adt file (*.adt)");
    adtModel = new QStandardItemModel(this);

    for (int row = 0; row < fileNames.size(); ++row)
        adtModel->appendRow(new QStandardItem(fileNames.at(row)));

    adtModel->setHorizontalHeaderLabels(QStringList("Adt file"));
    adtTable->setModel(adtModel);
    adtTable->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    adtTable->verticalHeader()->setVisible(false);
    adtTable->verticalHeader()->setHighlightSections(false);
}
