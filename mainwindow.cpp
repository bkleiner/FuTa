#include "mainwindow.h"

#include "file/maptile.h"

#include <QDebug>
#include <Qt>
#include <QColor>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
  ,centerWidget(new widget(this))
{
    setCentralWidget(centerWidget);
    setWindowIcon(QIcon(":/res/FutaIcon.png"));
    lastFiles = settings.value("lastfiles",QStringList(QDir::currentPath().append("/no.adt"))).value<QStringList>();
    createMenus();
    progressBar = new QProgressBar;
    progressBar->hide();
    progressBar->setMaximum(100);
    statusBar()->showMessage(tr("Ready"));
    statusBar()->addWidget(progressBar);
}

void MainWindow::showProgress(int count, int sum)
{
    if(progressBar->isHidden())
        progressBar->show();

    progressBar->setValue((float)count/(float)sum*100);

    if((float)count/(float)sum*100 >= 100)
        progressBar->hide();
}

void MainWindow::showText(QString text)
{
    statusBar()->showMessage(text);
}

void MainWindow::openFile()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this,"Select the WoW files to open",lastFiles.first().left(lastFiles.lastIndexOf("/")),"WoWFiles (*.adt *.wdt *.wdl)");
    foreach(QString fileName, fileNames)
    {
        openFile(fileName);
    }
}

void MainWindow::openFile(QString fileName)
{
    MapTile *file = new MapTile(fileName);

    file->read();
    centerWidget->addFile(file);
    addLastfile(fileName);
}

void MainWindow::addLastfile(QString fileName)
{
    if(!lastFiles.contains(fileName))
        lastFiles.prepend(fileName);

    if(lastFiles.size() > 5)
        lastFiles.removeLast();

    redoLastFiles();
}

void MainWindow::createFile()
{
    QAction *fileAction = (QAction*)sender();
    FileWizard *wizard = new FileWizard((FileWizard::type)fileAction->data().toInt(),this);
    if(wizard->exec())
        centerWidget->addFile(wizard->getFile());
}

void MainWindow::openLastFile()
{
    QAction *lastFile = (QAction*)sender();
    openFile(lastFile->text());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    settings.setValue("lastfiles",lastFiles);
    event->accept();
}

void MainWindow::setWoWPath()
{
    settings.setValue("wowpath",QFileDialog::getExistingDirectory(this,"set WoW Path",QDir::currentPath()));
    emit settingsChanged();
}

void MainWindow::setAlphaColor()
{
    QColor current = settings.value("alphamap/color").value<QColor>();
    QColor color = QColorDialog::getColor(current, this, "Select Color", QColorDialog::DontUseNativeDialog);
    settings.setValue("alphamap/color",color);
    emit settingsChanged();
}

void MainWindow::setAlphaFormatToColored()
{
    settings.setValue("alphamap/format",QImage::Format_ARGB32);
    emit settingsChanged();
}

void MainWindow::setAlphaFormatToGrayscale()
{
    settings.setValue("alphamap/format",QImage::Format_RGB32);
    emit settingsChanged();
}

void MainWindow::setShowHeightIn3d(bool value)
{
    settings.setValue("heightmap/show3d",value);
    emit settingsChanged();
}

void MainWindow::redoLastFiles()
{
    int size = fileMenu->actions().size()-4;
    for(int i = 0; i < (lastFiles.size() > 5 ? 5 : lastFiles.size()); ++i)
    {
        if(size > i)
            fileMenu->removeAction(fileMenu->actions().at(4));

        QAction *aFile = new QAction(lastFiles.at(i),this);
        connect(aFile,SIGNAL(triggered()),this,SLOT(openLastFile()) );
        fileMenu->addAction(aFile);

    }
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(tr("&Open..."),this,SLOT(openFile()));
    fileMenu->addAction(tr("&Save"),centerWidget,SLOT(saveFile()));
    QMenu *newFileMenu = fileMenu->addMenu(tr("New"));

    fileMenu->addSeparator();

    redoLastFiles();

    QAction *adtAction = new QAction(tr("Adt File"),this);
    //QAction *wdtAction = new QAction(tr("Wdt File"),this);
    //QAction *wdlAction = new QAction(tr("Wdl File"),this);
    //QAction *projectAction = new QAction(tr("Project File"),this);

    adtAction->setData(FileWizard::ADT);
    //wdtAction->setData(FileWizard::WDT);
    //wdlAction->setData(FileWizard::WDL);
    //projectAction->setData(FileWizard::PROJECT);

    connect(adtAction,SIGNAL(triggered()),this,SLOT(createFile()) );
    //connect(wdtAction,SIGNAL(triggered()),this,SLOT(createFile()) );
    //connect(wdlAction,SIGNAL(triggered()),this,SLOT(createFile()) );
    //connect(projectAction,SIGNAL(triggered()),this,SLOT(createFile()) );

    newFileMenu->addAction(adtAction);
    //newFileMenu->addAction(wdtAction);
    //newFileMenu->addAction(wdlAction);
    //newFileMenu->addAction(projectAction);

    QMenu *settingsMenu = menuBar()->addMenu(tr("&Settings"));

    QMenu *alphaMenu = settingsMenu->addMenu(tr("Alphamap"));
    QMenu *alphaFormatMenu = alphaMenu->addMenu("Format");

    settingsMenu->addAction(tr("set WoW Path"),this,SLOT(setWoWPath()));

    alphaMenu->addAction(tr("set color"),this,SLOT(setAlphaColor()));

    QActionGroup *alphaFormat = new QActionGroup(this);

    QAction *grayscale = new QAction(tr("Grayscale"),this);
    grayscale->setCheckable(true);
    connect(grayscale,SIGNAL(triggered()),this,SLOT(setAlphaFormatToGrayscale()) );

    QAction *colored = new QAction(tr("Colored"),this);
    colored->setCheckable(true);
    connect(colored,SIGNAL(triggered()),this,SLOT(setAlphaFormatToColored()) );

    if((QImage::Format)settings.value("alphamap/format",QImage::Format_ARGB32).toInt() == QImage::Format_ARGB32)
        colored->setChecked(true);
    else
        grayscale->setChecked(true);

    alphaFormat->addAction(colored);
    alphaFormat->addAction(grayscale);

    alphaFormatMenu->addAction(colored);
    alphaFormatMenu->addAction(grayscale);

    QMenu *heightMenu = settingsMenu->addMenu(tr("Heightmap"));

    QAction *show3d = new QAction(tr("show in 3D"),this);
    show3d->setCheckable(true);
    show3d->setChecked(settings.value("heightmap/show3d",false).toBool());
    connect(show3d,SIGNAL(triggered(bool)),this,SLOT(setShowHeightIn3d(bool)) );

    heightMenu->addAction(show3d);
}

