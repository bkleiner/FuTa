#include "alphamapwidget.h"

alphamapWidget::alphamapWidget(AdtFile* file, QWidget *parent) :
    WoWFileWidget(parent)
  ,adtFile(file)
{
    tabs = new QTabWidget(this);

    for(int layer = 1; layer < 4; ++layer)
        tabs->addTab(this->createLayerTab(layer),QString("Layer %1").arg(layer));

    tabs->setMinimumSize(500,500);
    saveButton = new QPushButton(tr("&Export"),this);
    importButton = new QPushButton(tr("&Import"),this);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabs);
    mainLayout->addWidget(saveButton,0,Qt::AlignRight);
    mainLayout->addWidget(importButton,0,Qt::AlignRight);

    connect(saveButton,SIGNAL(clicked()),this,SLOT(saveAlphamaps()));
    connect(importButton,SIGNAL(clicked()),this,SLOT(importPNG()));

    this->setLayout(mainLayout);
}

void alphamapWidget::reload()
{
    for(int i = 0; i < tabs->count(); ++i)
    {
        this->drawAlphaToPic(alphamaps.at(i),i+1);
        QLabel *tryOut = tabs->widget(i)->findChild<QLabel *>();
        tryOut->setPixmap(QPixmap::fromImage(*alphamaps.at(i)).scaled(500,500));
    }
}

void alphamapWidget::importPNG()
{
    QString filename = QFileDialog::getOpenFileName(this,"Select the alphamap png to open",QDir::current().absolutePath(),"*.PNG (*.png)");
    if(!filename.isEmpty())
    {
        this->drawPicToAlpha(new QImage(filename),this->tabs->currentIndex()+1);
        this->drawAlphaToPic(alphamaps.at(this->tabs->currentIndex()),this->tabs->currentIndex()+1);
        QLabel *tryOut = tabs->widget(this->tabs->currentIndex())->findChild<QLabel *>();
        tryOut->setPixmap(QPixmap::fromImage(*alphamaps.at(this->tabs->currentIndex())).scaled(500,500));
    }
}

void alphamapWidget::drawPicToAlpha(QImage *pic, int layer)
{
    QImage::Format format = (QImage::Format)settings.value("alphamap/format",QImage::Format_ARGB32).toInt();//.value<QImage::Format>();
    for(int x = 0; x < 16; ++x)
    {
        for(int y = 0; y < 16; ++y)
        {
            unsigned char data[64*64];
            memset(data,0,64*64);
            for(int cx = 0; cx<64; ++cx)
                for(int cy = 0; cy<64; ++cy)
                    data[64*cy+cx] = format == QImage::Format_ARGB32 ? (uchar)qAlpha(pic->pixel(64*x+cx, 64*y+cy)) : (uchar)qRed(pic->pixel(64*x+cx, 64*y+cy));
            adtFile->setAlphamap(data,x,y,layer);
        }
    }

}

void alphamapWidget::drawAlphaToPic(QImage *pic, int layer)
{
    for(int x = 0; x < 16; ++x)
        for(int y = 0; y < 16; ++y)
            this->renderAlpha(pic,x,y,layer);
}

void alphamapWidget::renderAlpha(QImage *pic, int x, int y, int layer)
{
    unsigned char amap[64*64];

    QColor rgb = settings.value("alphamap/color",Qt::green).value<QColor>();
    QImage::Format format = (QImage::Format)settings.value("alphamap/format",QImage::Format_ARGB32).toInt();

    adtFile->getAlphamap(amap,x,y,layer);
    for(int cx = 0; cx<64; ++cx)
    {
        for(int cy = 0; cy<64; cy++)
        {
            int alpha = (uchar)amap[64*cy+cx];
            if(format == QImage::Format_ARGB32)
              pic->setPixel(64*x+cx, 64*y+cy, qRgba(rgb.red(),rgb.green(),rgb.blue(),alpha));
            else
              pic->setPixel(64*x+cx, 64*y+cy, qRgb(alpha,alpha,alpha));
        }
    }
}

QWidget* alphamapWidget::createLayerTab(int layer)
{
    QWidget* layerTab = new QWidget();
    QImage *alphamap = new QImage(1024, 1024, (QImage::Format)settings.value("alphamap/format",QImage::Format_ARGB32).toInt());

    this->drawAlphaToPic(alphamap,layer);
    this->alphamaps.append(alphamap);

    QLabel *alpha = new QLabel(layerTab);
    alpha->setPixmap(QPixmap::fromImage(*alphamap).scaled(500,500));
    alpha->setMinimumSize(500,500);

    return layerTab;
}

void alphamapWidget::saveAlphamaps()
{
        for(int i = 0; i < alphamaps.count();++i)
            alphamaps.at(i)->save(adtFile->getFilename().left(adtFile->getFilename().lastIndexOf(".")).append("_layer_%1.png").arg(i+1),"PNG");
}
