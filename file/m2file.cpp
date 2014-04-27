#include "m2file.h"
#include <iostream>

m2File::m2File(QString fileName) : WoWFile(fileName)
{
    QFile m2(filename);
    QFile skin(filename.left(filename.lastIndexOf(".")).append("00.skin"));

    if (!m2.open(QIODevice::ReadOnly) || !skin.open(QIODevice::ReadOnly) )
             return;

    QByteArray m2Data = m2.readAll();
    QByteArray skinData = skin.readAll();

    m2Buffer.setBuffer(&m2Data);
    skinBuffer.setBuffer(&skinData);

    if(!m2Buffer.open(QBuffer::ReadOnly) || !skinBuffer.open(QBuffer::ReadOnly))
        return;

    m2Buffer.read((char*)&m2Header, sizeof(m2Header));
    skinBuffer.read((char*)&skinHeader, sizeof(skinHeader));

    vertexes = new ModelVertex[m2Header.nVertices];
    m2Buffer.seek(m2Header.ofsVertices);
    m2Buffer.read((char*)&vertexes[0],48*m2Header.nVertices);


    uint16_t *indexLookup = reinterpret_cast<uint16_t*>(skinBuffer.buffer().data() + skinHeader.ofsIndex);
    uint16_t *triangles = reinterpret_cast<uint16_t*>(skinBuffer.buffer().data() + skinHeader.ofsTris);
    nIndices = skinHeader.nTris;
    indices = new uint16_t[nIndices];
    for (size_t i = 0; i<nIndices; ++i) {
      indices[i] = indexLookup[triangles[i]];
    }

    ModelGeoset *ops = (ModelGeoset*)(skinData.data() + skinHeader.ofsSub);
    ModelTexUnit *tex = (ModelTexUnit*)(skinData.data() + skinHeader.ofsTex);

    for (size_t j = 0; j<skinHeader.nTex; j++)
    {
        testshit next;
        next.begin = ops[tex[j].op].istart;
        next.count = ops[tex[j].op].icount;
        array.insert(array.end(),next);
    }
}

m2File::testshit m2File::getOps(int index)
{
    return array.at(index);
}

ModelVertex m2File::getVertex(int index)
{
    return vertexes[indices[index]];
}
