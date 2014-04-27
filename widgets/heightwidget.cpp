#include "heightwidget.h"


HeightWidget::HeightWidget(AdtFile* file, QWidget *parent) :
    WoWFileWidget(parent)
  ,adtFile(file)
  ,label(NULL)
  ,heightview(NULL)
{
    saveButton = new QPushButton(tr("&Export"),this);
    importButton = new QPushButton(tr("&Import"),this);

    this->creatLayout();

    connect(saveButton,SIGNAL(clicked()),this,SLOT(exportFile()));
    connect(importButton,SIGNAL(clicked()),this,SLOT(importFile()));
}

void HeightWidget::reload()
{
    this->hide();
    this->creatLayout();
    this->show();
}

void HeightWidget::creatLayout()
{
    if(this->layout())
        delete this->layout();

    mainLayout = new QVBoxLayout();

    if(settings.value("heightmap/show3d",false).toBool())
    {
        if(label)
        {
            label->hide();
            delete label;
            label = NULL;
        }

        if(!heightview)
            heightview = new heightView(adtFile,this);
        mainLayout->addWidget(heightview);
    }
    else
    {
        if(heightview)
        {
            heightview->hide();
            delete heightview;
            heightview = NULL;
        }

        if(!label)
            label = new QLabel("Heightmap Import/Export more features will be added in future",this);
        mainLayout->addWidget(label);
    }

    mainLayout->addWidget(saveButton,0,Qt::AlignRight);
    mainLayout->addWidget(importButton,0,Qt::AlignRight);
    this->setLayout(mainLayout);

}

void HeightWidget::importFile()
{
#ifdef Q_WS_WIN
    QString filename = adtFile->getFilename().left(adtFile->getFilename().lastIndexOf("\\")+1);
#else
    QString filename = adtFile->getFilename().left(adtFile->getFilename().lastIndexOf("/")+1);
#endif
    adtFile->importHeight(QFileDialog::getOpenFileName(this,"Select the height files to open",filename,"Height Files (*.height)").toStdString());
}

void HeightWidget::exportFile()
{
    QString filename = adtFile->getFilename().left(adtFile->getFilename().lastIndexOf(".")+1).append("height");
    adtFile->exportHeight(QFileDialog::getSaveFileName(this,"Select path to save the height file",filename,"Height Files (*.height)").toStdString());
}
