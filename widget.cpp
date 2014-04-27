#include "widget.h"
#include <QDebug>

widget::widget(QWidget *parent) :
    QWidget(parent)
    ,layout(new QGridLayout(this))
    ,dummyWidget(new WoWFileWidget(this))
    ,treeView(new QTreeView(this))
    ,model(new WoWFileModel(this))
{
    treeView->setModel(model);

    dummyWidget->setMinimumSize(600,600);
    dummylayout = new QVBoxLayout(dummyWidget);

    layout->addWidget(treeView,0,0);
    layout->addWidget(dummyWidget,0,1);
    layout->setColumnStretch(1,10);
    layout->setColumnStretch(1,30);

    connect(treeView,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(changeWidget(QModelIndex)));
    editWidget = new WoWFileWidget(dummyWidget);
    dummylayout->addWidget(editWidget);
    connect(parent,SIGNAL(settingsChanged()),this,SLOT(reloadWidgets()));
}

void widget::reloadWidgets()
{
    editWidget->reload();
}

void widget::saveFile()
{
    WoWFileItem *item = static_cast<WoWFileItem*>(treeView->currentIndex().internalPointer());
    item->getWoWFile()->save();
}

bool widget::addFile(WoWFile *wowfile)
{
    if(wowfile)
    {
        model->addWoWFile(wowfile);
        return true;
    }
    else
        return false;
}

void widget::loadWidget(WoWFileWidget *toLoad)
{
    toLoad->setParent(dummyWidget);

    if(editWidget)
        dummylayout->removeWidget(editWidget);

    dummylayout->addWidget(toLoad);
    editWidget->hide();
    toLoad->reload();
    toLoad->show();
    editWidget = toLoad;
}

void widget::changeWidget(QModelIndex index)
{
    if(index.isValid())
    {
        WoWFileItem *item = static_cast<WoWFileItem*>(index.internalPointer());
        if(item->getWidget())
            this->loadWidget(item->getWidget());
    }
}
