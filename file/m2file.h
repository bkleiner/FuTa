#ifndef M2FILE_H
#define M2FILE_H

#include <QFile>
#include <QBuffer>
#include "ModelHeaders.h"
#include "wowfile.h"

class m2File : public WoWFile
{
public:

    struct testshit
    {
        int begin, count;
    };

    m2File(QString fileName);

    ModelVertex getVertex(int index);
    testshit getOps(int index);

    ModelViewStruct skinHeader;

private:
    QBuffer m2Buffer;
    QBuffer skinBuffer;

    ModelHeader m2Header;

    ModelVertex *vertexes;

    size_t nIndices;
    uint16_t *indices;


    std::vector<testshit> array;


};

#endif // M2FILE_H
