#ifndef WOWFILEMANAGER_H
#define WOWFILEMANAGER_H

#include "adtfile.h"
#include "wdlfile.h"
#include "mpqfile.h"
#include "futa.h"
#include <QSettings>
#include "QFile"
#include <QDir>

class WoWFileManager
{
public:
    WoWFileManager();

    static AdtFile *createAdtFile(QString fileName, WoWFile::Version version, QString textureString, int mapx, int mapy, int areaID);
    static WdlFile *createWdlFile(QString fileName);


    static AdtFile *openAdt(QString fileName);
    static MPQFile *openMPQ(QString fileName);
    static WdlFile *openWdl(QString fileName);

    template<typename T> static T *openWoWFile(QString path)
    {
        char *data;
        int size = 0;
        QString diskPath = FuTa::manager()->WoWPath+"Data/"+path;
        diskPath.replace("\\","/");

        if(QFile::exists(diskPath))
        {
            QFile file(diskPath);

            if(!file.open(QIODevice::ReadOnly))
            {
                std::cout<<"File not found: "<<diskPath.toStdString()<<std::endl;
                return false;
            }

            size = file.size();
            data = new char[size];

            file.read(data,size);

            file.close();
        }
        else
        {
            if(!FuTa::manager()->openFileFromMPQ(path,&size,&data))
            {
                std::cout<<"File not found: "<<path.toStdString()<<std::endl;
                return false;
            }
        }

        return new T(path,data,size);
    }

    static QStringList getWoWListFile();

    void extractAllFiles(QString path);

    void openWoWMPQs();

    QString WoWPath;

private:
    void loadMPQ(QString fileName);
    bool openFileFromMPQ(QString path, int *size, char **data);


    QString WoWLoc;
    QSettings settings;
    QList<MPQFile *> openMPQs;
    QStringList MPQListfiles;
};

#endif // WOWFILEMANAGER_H
