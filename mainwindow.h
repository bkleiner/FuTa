#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include "widget.h"
#include "widgets/filewizard.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    void closeEvent(QCloseEvent *event);

    QStringList *getLastFiles() { return &lastFiles; }

private:
    void createMenus();
    void addLastfile(QString fileName);

    widget *centerWidget;
    QSettings settings;
    QStringList lastFiles;
    QMenu *fileMenu;
    QProgressBar *progressBar;

signals:
    void settingsChanged();

public slots:
    void openFile();
    void openFile(QString fileName);
    void openLastFile();

    void createFile();

    void redoLastFiles();

    void setAlphaColor();
    void setWoWPath();
    void setAlphaFormatToGrayscale();
    void setAlphaFormatToColored();
    void setShowHeightIn3d(bool);

    void showProgress(int count, int sum);
    void showText(QString text);
};


#endif // MAINWINDOW_H
