#include "wowfilemanager.h"
#include <QMessageBox>

WoWFileManager::WoWFileManager()
{
    //openWoWMPQs();
}

QStringList WoWFileManager::getWoWListFile()
{
    FuTa::manager()->MPQListfiles;
}

void WoWFileManager::openWoWMPQs()
{
    WoWPath = settings.value("wowpath").toString();
    if(!WoWPath.isEmpty())
    {
        if (QFile::exists (WoWPath))
        {

            if(!WoWPath.endsWith("/"))
                WoWPath.append("/");

            WoWLoc = settings.value("locale", "****").toString();

            if (WoWLoc == "****")
            {
                QStringList locales;
                locales << "enGB" << "enUS" << "deDE" << "koKR" << "frFR"
                        << "zhCN" << "zhTW" << "esES" << "esMX" << "ruRU";

                foreach (QString locale, locales)
                {
                    if (QFile::exists (WoWPath + "Data/" + locale))
                    {
                        WoWLoc = locale;
                        break;
                    }
                }

                if (WoWLoc == "****")
                {
                    std::cout << "WoW Locale invaild" << std::endl;
                    return;
                }
            }

            QStringList archive_names;
            archive_names << "common.MPQ"
                          << "common-2.MPQ"
                          << "expansion.MPQ"
                          << "lichking.MPQ"
                          << "patch.MPQ"
                          << "patch-%1.MPQ"
                             << "{locale}/backup-{locale}.MPQ"
                             << "{locale}/base-{locale}.MPQ"
                          << "{locale}/locale-{locale}.MPQ"
                             << "{locale}/speech-{locale}.MPQ"
                          << "{locale}/expansion-locale-{locale}.MPQ"
                             << "{locale}/expansion-speech-{locale}.MPQ"
                          << "{locale}/lichking-locale-{locale}.MPQ"
                             << "{locale}/lichking-speech-{locale}.MPQ"
                          << "{locale}/patch-{locale}.MPQ"
                          << "{locale}/patch-{locale}-%1.MPQ"
                             << "development.MPQ"
                             ;

            foreach (QString archive, archive_names)
            {
                QString path(WoWPath + "Data/" + archive);
                path.replace("{locale}", WoWLoc);

                if (path.contains ("%1"))
                {
                    for (char i ('2'); i <= '9'; ++i)
                    {
                        QString replaced (path.arg (i));
                        if (QFile::exists (replaced))
                        {
                            loadMPQ (replaced);
                        }
                    }
                }
                else
                {
                    if (QFile::exists (path))
                    {
                        loadMPQ (path);
                    }
                }
            }
        }
        else
        {
            std::cout << "WoWPath invaild" << std::endl;
            return;
        }
    }
    else
        return;
}

void WoWFileManager::loadMPQ(QString fileName)
{
    MPQFile *file = this->openMPQ(fileName);
    MPQListfiles.append(file->getListfile());
    MPQListfiles.removeDuplicates();
    openMPQs.append(file);
}

bool WoWFileManager::openFileFromMPQ(QString path, int *size, char **data)
{
    QListIterator<MPQFile *> iterator (openMPQs);
    for (iterator.toBack(); iterator.hasPrevious(); iterator.previous())
    {
        if (iterator.peekPrevious()->openFile (path, size, data))
            return true;
    }
    return false;
}

MPQFile *WoWFileManager::openMPQ(QString fileName)
{
    return new MPQFile(fileName);
}

AdtFile *WoWFileManager::createAdtFile(QString fileName, WoWFile::Version version, QString textureString, int mapx, int mapy, int areaID = 0)
{
    switch(version)
    {
    case WoWFile::WOTLK:
        return new AdtFileLk(fileName,textureString,mapx,mapy,areaID);

    case WoWFile::CATA:
        //return new AdtFileTemplate<AdtCata>(fileName,textureString,mapx,mapy,areaID);
        QMessageBox::information(NULL, "Cata file selected", "Cata is not supported yet");

    default:
        return NULL;
    }
}

void WoWFileManager::extractAllFiles(QString path)
{
    int i = 1;
    for(QStringList::iterator it = MPQListfiles.begin(); it != MPQListfiles.end(); ++it)
    {
        QString savepath(path);
        int size;
        char *data;
        openFileFromMPQ(*it,&size,&data);

        savepath.append(*it);
        savepath.replace("\\","/");

        QDir dir;
        if(!dir.exists(savepath.left(savepath.lastIndexOf("/"))))
        {
            dir.mkpath(savepath.left(savepath.lastIndexOf("/")));
        }

        QFile file(savepath);

        if(file.exists())
            if(file.size() == size)
                break;

        if(!file.open(QIODevice::WriteOnly))
                break;

        file.write(data,size);

        file.close();
        delete [] data;
        std::cout << "file " << i << "/" << MPQListfiles.size() << ": " << it->toStdString() << std::endl;
        i++;
    }
}

AdtFile *WoWFileManager::openAdt(QString fileName)
{
    switch(Utilities::getAdtVersion(fileName.toStdString()))
    {
    case WoWFile::WOTLK:
        return new AdtFileLk(fileName);

    case WoWFile::CATA:
        QMessageBox::information(NULL, "Cata file selected", "Cata is not supported yet");
        //return new AdtFileTemplate<AdtCata>(fileName);

    default:
        return NULL;
    }
}

WdlFile *WoWFileManager::openWdl(QString fileName)
{
    return new WdlFile(fileName);
}
