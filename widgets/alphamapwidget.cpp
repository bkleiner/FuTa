#include "alphamapwidget.h"

#include "file/maptile.h"

alphamapWidget::alphamapWidget(MapTile* file, QWidget *parent)
    : WoWFileWidget(parent)
    , adtFile(file)
{
    tabs = new QTabWidget(this);

    for(int layer = 0; layer < 3; ++layer)
        tabs->addTab(createLayerTab(layer),QString("Layer %1").arg(layer + 1));

    tabs->setMinimumSize(500,500);
    exportButton = new QPushButton(tr("&Export"),this);
    importButton = new QPushButton(tr("&Import"),this);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(tabs);
    mainLayout->addWidget(exportButton,0,Qt::AlignRight);
    mainLayout->addWidget(importButton,0,Qt::AlignRight);

    connect(exportButton,SIGNAL(clicked()),this,SLOT(exportPNG()));
    connect(importButton,SIGNAL(clicked()),this,SLOT(importPNG()));

    setLayout(mainLayout);
}

void alphamapWidget::reload()
{
    for(int i = 0; i < tabs->count(); ++i)
    {
        drawAlphaToPic(alphamaps.at(i),i);
        QLabel *tryOut = tabs->widget(i)->findChild<QLabel *>();
        tryOut->setPixmap(QPixmap::fromImage(*alphamaps.at(i)).scaled(500,500));
    }
}

void alphamapWidget::importPNG()
{
    QString filename = QFileDialog::getOpenFileName(this,"Select the alphamap png to open",QDir::current().absolutePath(),"*.PNG (*.png)");
    if(!filename.isEmpty())
    {
        drawPicToAlpha(new QImage(filename),tabs->currentIndex());
        drawAlphaToPic(alphamaps.at(tabs->currentIndex()),tabs->currentIndex());
        QLabel *tryOut = tabs->widget(tabs->currentIndex())->findChild<QLabel *>();
        tryOut->setPixmap(QPixmap::fromImage(*alphamaps.at(tabs->currentIndex())).scaled(500,500));
    }
}

void alphamapWidget::drawPicToAlpha(QImage *pic, int layer)
{
    QImage::Format format = (QImage::Format)settings.value("alphamap/format",QImage::Format_ARGB32).toInt();

    for(int x = 0; x < 16; ++x)
    {
        for(int y = 0; y < 16; ++y)
        {
            Alphamap::Ptr amap = adtFile->getAlphamap(x, y, layer);

            for(int cx = 0; cx < 64; ++cx)
            {
                for(int cy = 0; cy < 64; ++cy)
                {
                    uint8_t value = format == QImage::Format_ARGB32 ? qAlpha(pic->pixel(64*x+cx, 64*y+cy)) : qRed(pic->pixel(64*x+cx, 64*y+cy));
                    amap->setValue(64*cy+cx, value);
                }
            }
        }
    }

}

void alphamapWidget::drawAlphaToPic(QImage *pic, int layer)
{
    for(int x = 0; x < 16; ++x)
        for(int y = 0; y < 16; ++y)
            renderAlpha(pic,x,y,layer);
}

void alphamapWidget::renderAlpha(QImage *pic, int x, int y, int layer)
{
    QVariant variant = QColor(Qt::green);
    QColor rgb = settings.value("alphamap/color",variant).value<QColor>();
    QImage::Format format = (QImage::Format)settings.value("alphamap/format",QImage::Format_ARGB32).toInt();
    Alphamap::Ptr amap = adtFile->getAlphamap(x, y, layer);

    for(int cx = 0; cx < 64; ++cx)
    {
        for(int cy = 0; cy < 64; cy++)
        {
            int alpha = amap->value(64*cy+cx);

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

    drawAlphaToPic(alphamap,layer);
    alphamaps.append(alphamap);

    QLabel *alpha = new QLabel(layerTab);
    alpha->setPixmap(QPixmap::fromImage(*alphamap).scaled(500,500));
    alpha->setMinimumSize(500,500);

    return layerTab;
}

void alphamapWidget::exportPNG()
{
    for(int i = 0; i < alphamaps.count();++i)
        alphamaps.at(i)->save(adtFile->getFilename().left(adtFile->getFilename().lastIndexOf(".")).append("_layer_%1.png").arg(i+1),"PNG");
}
