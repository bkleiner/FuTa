#ifndef WOWFILEWIDGET_H
#define WOWFILEWIDGET_H

#include <QWidget>

class WoWFileWidget : public QWidget
{
    Q_OBJECT
public:
    WoWFileWidget(QWidget *parent = 0);

    virtual void reload() { }
    
signals:
    
public slots:
    
};

#endif // WOWFILEWIDGET_H
