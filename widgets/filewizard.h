#ifndef FILEWIZARD_H
#define FILEWIZARD_H

#include <QWizard>
#include <QtGui>

#include "file/wowfile.h"

class MainWindow;

class FileWizard : public QWizard
{
    Q_OBJECT
public:

    enum type
    {
        ADT,
        WDT,
        WDL,
        FUTA
    };

    FileWizard(type filetype, QWidget *parent = 0);
    WoWFile* getFile();

private:
    type fileType;
    QWizardPage *inDev();
    QString filePath;
    MainWindow *parent;
    
signals:
    
public slots:
    //void findPath();
    
};

namespace FileWizardPages
{

class AdtPage : public QWizardPage
{
    Q_OBJECT

public:
    AdtPage();
public slots:
    void findPath();
signals:
    void foundFile(QString);
private:
    QFileDialog *findDialog;
};

}
#endif // FILEWIZARD_H
