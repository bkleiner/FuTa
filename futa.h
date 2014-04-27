#ifndef FUTA_H
#define FUTA_H

#include <QApplication>

//#include "file/wowfilemanager.h"

class WoWFileManager;
class MainWindow;

class FuTa : public QApplication
{
public:
    FuTa(int &argc, char **argv);
    virtual ~FuTa() { }
    virtual bool notify(QObject * receiver, QEvent * event);

    static FuTa* instance() { return (FuTa* )qApp; }
    static QStringList* getLastFiles();
    static WoWFileManager* manager() { return instance()->fileManager; }
    static MainWindow *mainwindow() { return instance()->mainWindow; }

private:
    MainWindow *mainWindow;
    WoWFileManager *fileManager;
};

#endif // FUTA_H
