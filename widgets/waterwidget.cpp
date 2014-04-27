#include "waterwidget.h"

WaterWidget::WaterWidget(AdtFile *file, QWidget *parent) :
    WoWFileWidget(parent)
  ,adtFile(file)
{
    saveButton = new QPushButton(tr("&Export"),this);
    importButton = new QPushButton(tr("&Import"),this);
    QLabel *label = new QLabel("Water Import/Export more features will be added in future",this);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(label);
    mainLayout->addWidget(saveButton,0,Qt::AlignRight);
    mainLayout->addWidget(importButton,0,Qt::AlignRight);

    connect(saveButton,SIGNAL(clicked()),this,SLOT(exportFile()));
    connect(importButton,SIGNAL(clicked()),this,SLOT(importFile()));

    this->setLayout(mainLayout);
}

void WaterWidget::importFile()
{
#ifdef Q_WS_WIN
    QString filename = adtFile->getFilename().left(adtFile->getFilename().lastIndexOf("\\")+1);
#else
    QString filename = adtFile->getFilename().left(adtFile->getFilename().lastIndexOf("/")+1);
#endif
    adtFile->importWater(QFileDialog::getOpenFileName(this,"Select the water files to open",filename,"Water Files (*.mh2o)").toStdString());
}

void WaterWidget::exportFile()
{
    QString filename = adtFile->getFilename().left(adtFile->getFilename().lastIndexOf(".")+1).append("mh2o");
    adtFile->exportWater(QFileDialog::getSaveFileName(this,"Select path to save the water file",filename,"Water Files (*.mh2o)").toStdString());
}
