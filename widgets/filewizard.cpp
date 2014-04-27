#include "filewizard.h"
#include "futa.h"

FileWizard::FileWizard(type filetype, QWidget *parent) :
    QWizard(parent)
  ,fileType(filetype)
{
    switch(fileType)
    {

    case ADT:
        this->addPage(new FileWizardPages::AdtPage);
        break;

    default:
        this->addPage(inDev());
        break;
    }

    this->setWindowTitle("File creation wizard");
}

QWizardPage *FileWizard::inDev()
{
    QWizardPage *page = new QWizardPage;
    QLabel *label = new QLabel("sorry there went something wrong or this features isnt ready yet \n"
                               "or both");
    QVBoxLayout *layout = new QVBoxLayout;

    page->setTitle("nothing here");
    label->setWordWrap(true);
    layout->addWidget(label);
    page->setLayout(layout);

    return page;
}

WoWFile *FileWizard::getFile()
{
    //create file here
}


FileWizardPages::AdtPage::AdtPage() :
    QWizardPage(NULL)
{
    QGridLayout *layout = new QGridLayout;
    QHBoxLayout *cordLayout = new QHBoxLayout;

    QLabel *nameLabel = new QLabel("Instance name");
    QLabel *cordLabel = new QLabel("Adt cordinats");
    QLabel *versionLabel = new QLabel("WoW version");
    QLabel *texLabel = new QLabel("Base texture");
    QLabel *areaLabel = new QLabel("Area id (optional)");

    QLineEdit *pathEdit = new QLineEdit("file save path");
    QLineEdit *nameEdit = new QLineEdit;
    QLineEdit *mapxEdit = new QLineEdit;
    QLineEdit *mapyEdit = new QLineEdit;
    QLineEdit *texEdit = new QLineEdit("tileset\\elwynn\\elwynngrassbase.blp");
    QLineEdit *areaEdit = new QLineEdit;

    QComboBox *versionCombo = new QComboBox;

    QPushButton *getPath = new QPushButton("&Find");
    findDialog = new QFileDialog;

    connect(getPath,SIGNAL(clicked()),this,SLOT(findPath()));
    connect(this,SIGNAL(foundFile(QString)),pathEdit,SLOT(setText(QString)));

    mapxEdit->setFixedWidth(40);
    mapyEdit->setFixedWidth(40);

    cordLayout->addWidget(mapxEdit);
    cordLayout->addWidget(mapyEdit);

    versionCombo->addItem("WotLk",WoWFile::WOTLK);

    layout->addWidget(nameLabel,0,0);
    layout->addWidget(nameEdit,0,1);

    layout->addWidget(cordLabel,1,0);
    layout->addLayout(cordLayout,1,1);

    layout->addWidget(versionLabel,2,0);
    layout->addWidget(versionCombo,2,1);

    layout->addWidget(texLabel,3,0);
    layout->addWidget(texEdit,3,1);

    layout->addWidget(areaLabel,4,0);
    layout->addWidget(areaEdit,4,1);

    layout->addWidget(pathEdit,5,1);
    layout->addWidget(getPath,5,0);

    this->registerField("name*",nameEdit);
    this->registerField("mapx*",mapxEdit);
    this->registerField("mapy*",mapyEdit);
    this->registerField("version",versionCombo);
    this->registerField("texture",texEdit);
    this->registerField("areaid",areaEdit);
    this->registerField("path*",pathEdit);

    this->setLayout(layout);

}

void FileWizardPages::AdtPage::findPath()
{
    emit this->foundFile(findDialog->getSaveFileName(this
                                                     ,"save path"
                                                     ,QString(FuTa::getLastFiles()->first().left(FuTa::getLastFiles()->first().lastIndexOf("/")+1)).append(QString("%1_%2_%3.adt").arg(this->field("name").toString()).arg(this->field("mapx").toInt()).arg(this->field("mapy").toInt()))
                                                     ,"Adt file(*.adt)"));
}
