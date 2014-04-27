#include "layerwidget.h"
#include <QDebug>

LayerWidget::LayerWidget(AdtFile* file, QWidget *parent) :
    WoWFileWidget(parent)
  ,adtFile(file)
{
    tabs = new QTabWidget(this);

    texTable = new QTableView(this);
    saveButton = new QPushButton(tr("&Apply"),this);
    texModel = new QStandardItemModel(this);
    texList = QString::fromStdString(adtFile->getTextures()).split(QChar::Null, QString::SkipEmptyParts);

    for (int row = 0; row < texList.size(); ++row)
        texModel->appendRow(new QStandardItem(texList.at(row)));


    for(int layer = 0; layer < 4; ++layer)
        tabs->addTab(this->createLayerTab(layer),QString("Layer %1").arg(layer));

    texModel->setHorizontalHeaderLabels(QStringList("Texture Path"));
    texTable->setModel(texModel);
    texTable->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    texTable->verticalHeader()->setVisible(false);
    texTable->verticalHeader()->setHighlightSections(false);
    texTable->setContextMenuPolicy(Qt::CustomContextMenu);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QLabel *warning = new QLabel("This affects ALL Mcnks...Handle with care!");
    mainLayout->addWidget(warning);
    mainLayout->addWidget(tabs);
    mainLayout->addWidget(texTable);
    mainLayout->addWidget(saveButton,0,Qt::AlignRight);

    connect(saveButton,SIGNAL(clicked()),this,SLOT(save()));

    connect(texTable,SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(contextMenu(QPoint)));
    connect(texModel,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(changeTexture(QStandardItem*)));

    this->setLayout(mainLayout);

}

void LayerWidget::save()
{
    QString textures;
    for(int i = 0; i < texModel->rowCount(); ++i)
        textures.append(texModel->item(i)->text()).append(QChar::Null);
    adtFile->setTextures(textures.toStdString());
    for(int i = 0; i < tabs->count(); ++i)
        //int i = tabs->currentIndex();
    {
        QComboBox *combo = tabs->widget(i)->findChild<QComboBox *>();
        QLineEdit *flagsLine = tabs->widget(i)->findChild<QLineEdit *>("flagsLine");
        QLineEdit *effectLine = tabs->widget(i)->findChild<QLineEdit *>("effectLine");

        for(int u = 0; u < 256; ++u)
        {
            if(adtFile->getMclyEntrys(u)->size() > i)
            {
                adtFile->getMclyEntrys(u)->at(i).textureID = combo->itemData(combo->currentIndex()).toUInt();
                if(!flagsLine->text().isEmpty())
                    adtFile->getMclyEntrys(u)->at(i).flags = flagsLine->text().toUInt();
                if(!effectLine->text().isEmpty())
                    adtFile->getMclyEntrys(u)->at(i).effectID = effectLine->text().toUInt();
            }
        }

        if(adtFile->getMclyEntrys(16*8)->size() > i)
        {
            combo->setCurrentIndex(adtFile->getMclyEntrys(128)->at(i).textureID);
            flagsLine->setText(QString("%1").arg((int)adtFile->getMclyEntrys(128)->at(i).flags));
            effectLine->setText(QString("%1").arg((int)adtFile->getMclyEntrys(128)->at(i).effectID));
        }
    }
}

void LayerWidget::changeTexture(QStandardItem *item)
{
    for(int i = 0; i < tabs->count(); ++i)
    {
        QComboBox *combo = tabs->widget(i)->findChild<QComboBox *>();
        if(combo->count() < item->row()+1)
            combo->addItem(item->text(),item->data(Qt::UserRole));
        combo->setItemText(item->row(),item->text());
    }
}

void LayerWidget::addTexture()
{
    texModel->appendRow(new QStandardItem(""));
}

void LayerWidget::contextMenu(QPoint loc)
{
    QMenu *menu = new QMenu;
    menu->addAction(tr("add Texture"),this,SLOT(addTexture()));
    menu->exec(texTable->mapToGlobal(loc));
}

QWidget *LayerWidget::createLayerTab(int layer)
{
    QWidget* layerTab = new QWidget();

    QGridLayout *secLayout = new QGridLayout;

    QLabel *textureLabel = new QLabel("Texture");
    QLabel *flagsLabel = new QLabel("Texture flags");
    QLabel *effectLabel = new QLabel("Ground effect ID");
    QComboBox *textureCombo = new QComboBox(layerTab);
    QLineEdit *flagsLine = new QLineEdit(layerTab);
    QLineEdit *effectLine = new QLineEdit(layerTab);

    flagsLine->setValidator(new QIntValidator(this));
    effectLine->setValidator(new QIntValidator(this));

    flagsLine->setObjectName("flagsLine");
    effectLine->setObjectName("effectLine");

    for (int row = 0; row < texList.size(); ++row)
        textureCombo->addItem(texList.at(row),row);

    if(adtFile->getMclyEntrys(16*8)->size() > layer)
    {
        textureCombo->setCurrentIndex(adtFile->getMclyEntrys(128)->at(layer).textureID);
        flagsLine->setText(QString("%1").arg((int)adtFile->getMclyEntrys(128)->at(layer).flags));
        effectLine->setText(QString("%1").arg((int)adtFile->getMclyEntrys(128)->at(layer).effectID));
    }
    else
        secLayout->addWidget(new QLabel("this Layer is not set in the adt\nu will have to create it with the alphamap tool first"),3,0,3,2);

    secLayout->addWidget(textureLabel,0,0);
    secLayout->addWidget(textureCombo,0,1);
    secLayout->addWidget(flagsLabel,1,0);
    secLayout->addWidget(flagsLine,1,1);
    secLayout->addWidget(effectLabel,2,0);
    secLayout->addWidget(effectLine,2,1);

    layerTab->setLayout(secLayout);

    return layerTab;
}
