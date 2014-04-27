#include "heightview.h"
#include <QtOpenGL/QGLWidget>
#include <QFile>


heightView::heightView(AdtFile *adtfile, QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
  ,adtFile(adtfile)
{
    startTimer(25);
    distance = -250.0f;
    xRot = -90.0f * 16.0f;
    yRot = 0;
}

void heightView::initializeGL()
{

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glShadeModel(GL_SMOOTH);
}

void heightView::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 5000.0);
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
}


void heightView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f, distance); //glTranslatef(-16*8/2.0, -16*8/2.0, distance);

    GLfloat lightPosition[4] = { 0.0, 0.0, distance, 1.0 };
    glLightfv (GL_LIGHT0, GL_POSITION, lightPosition);

    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);


    glColor3f(0.0f,1.0f,0.0f);
    for(int cy = 0; cy<16; ++cy)
        for(int cx = 0; cx<16; ++cx)
        {
            glPushMatrix();
            glTranslatef(-16*4 + cx*8, -adtFile->getMcnkHeader(cy*16+cx)->posZ, -16*4 + cy*8);

            this->drawPass(cx,cy);

            glPopMatrix();
        }
}

void heightView::drawPass(int cx, int cy)
{
    AdtFile::NoLoDArray NoLoD;
    AdtFile::LoDArray LoD;
    MCNR mcnrStruct = adtFile->getMcnrStruct(cy*16+cx);
    adtFile->getValNoLOD(&NoLoD,&LoD,cy*16+cx);
    int mcnrIndex = 9;

    for(int x=0; x<8; ++x, mcnrIndex += 9)
    {
        for(int y=0; y<8; ++y, ++mcnrIndex)
        {
            float nL1=NoLoD[x][y]; //bottom right
            float nL2=NoLoD[x][y+1]; //top right
            float nL3=NoLoD[x+1][y+1]; //bottom left
            float nL4=NoLoD[x+1][y]; //top left
            float L=LoD[x][y]; // middle

            glBegin(GL_TRIANGLE_FAN);

            glNormal3f(-(float)mcnrStruct.entries[mcnrIndex].normal[1]/(float)127.0f
                       ,(float)mcnrStruct.entries[mcnrIndex].normal[2]/(float)127.0f
                       ,-(float)mcnrStruct.entries[mcnrIndex].normal[0]/(float)127.0f);
            glVertex3f( y, -L, x);


            glNormal3f(-(float)mcnrStruct.entries[mcnrIndex-9].normal[1]/(float)127.0f
                       ,(float)mcnrStruct.entries[mcnrIndex-9].normal[2]/(float)127.0f
                       ,-(float)mcnrStruct.entries[mcnrIndex-9].normal[0]/(float)127.0f);
            glVertex3f(y-0.5f, -nL1,  x-0.5f);


            glNormal3f(-(float)mcnrStruct.entries[mcnrIndex-8].normal[1]/(float)127.0f
                       ,(float)mcnrStruct.entries[mcnrIndex-8].normal[2]/(float)127.0f
                       ,-(float)mcnrStruct.entries[mcnrIndex-8].normal[0]/(float)127.0f);
            glVertex3f(y+0.5f, -nL2, x-0.5f);


            glNormal3f(-(float)mcnrStruct.entries[mcnrIndex+9].normal[1]/(float)127.0f
                       ,(float)mcnrStruct.entries[mcnrIndex+9].normal[2]/(float)127.0f
                       ,-(float)mcnrStruct.entries[mcnrIndex+9].normal[0]/(float)127.0f);
            glVertex3f(y+0.5f, -nL3, x+0.5f);


            glNormal3f(-(float)mcnrStruct.entries[mcnrIndex+8].normal[1]/(float)127.0f
                       ,(float)mcnrStruct.entries[mcnrIndex+8].normal[2]/(float)127.0f
                       ,-(float)mcnrStruct.entries[mcnrIndex+8].normal[0]/(float)127.0f);
            glVertex3f(y-0.5f, -nL4, x+0.5f);


            glNormal3f(-(float)mcnrStruct.entries[mcnrIndex-9].normal[1]/(float)127.0f
                       ,(float)mcnrStruct.entries[mcnrIndex-9].normal[2]/(float)127.0f
                       ,-(float)mcnrStruct.entries[mcnrIndex-9].normal[0]/(float)127.0f);
            glVertex3f(y-0.5f, -nL1, x-0.5f);


            glEnd();
        }
    }
}

void heightView::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void heightView::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        xRot = (xRot + 8 * dy);
        yRot = (yRot + 8 * dx);
        updateGL();
    }
    lastPos = event->pos();
}

void heightView::wheelEvent(QWheelEvent *event)
{
    event->accept();
    if (event->buttons() & Qt::LeftButton) {
        distance = distance + float(event->delta())/32;
        updateGL();
    }
}

void heightView::timerEvent(QTimerEvent *)
{
    _angle += 0.7f;
    if (_angle > 360) {
        _angle -= 360;
    }

    update();
}
