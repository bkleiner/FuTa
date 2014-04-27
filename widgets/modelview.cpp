#include "modelview.h"

modelView::modelView(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    distance = -10.0f;
    xRot = 0;
    yRot = 0;
    m2file = new m2File("ElwynnLog02.m2");
}

void modelView::initializeGL()
{

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glShadeModel(GL_SMOOTH);


}

void modelView::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (float)w / (float)h, 1.0, 100.0);
    glClearColor(0.0f, 0.0f, 1.0f, 0.0f);
}


void modelView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0.0f,0.0f, distance); //glTranslatef(-16*8/2.0, -16*8/2.0, distance);

    GLfloat lightPosition[4] = { 0.0, 0.0, 20.0, 1.0 };
    glLightfv (GL_LIGHT0, GL_POSITION, lightPosition);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);


    glColor3f(0.3f, 0.9f, 0.0f);

    for (size_t j = 0; j<m2file->skinHeader.nTex; j++)
    {
        glBegin(GL_TRIANGLES);
        for (size_t k = 0, b=m2file->getOps(j).begin; k<m2file->getOps(j).count; ++k,++b)
        {
            glNormal3fv(m2file->getVertex(b).normal);
            glTexCoord2f(m2file->getVertex(b).texcoordsX,m2file->getVertex(b).texcoordsY);
            glVertex3fv( m2file->getVertex(b).pos);
        }
        glEnd();
    }
}

void modelView::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void modelView::mouseMoveEvent(QMouseEvent *event)
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

void modelView::wheelEvent(QWheelEvent *event)
{
    event->accept();
    if (event->buttons() & Qt::LeftButton) {
        distance = distance + float(event->delta())/32;
        updateGL();
    }
}

void modelView::timerEvent(QTimerEvent *)
{
    _angle += 0.7f;
    if (_angle > 360) {
        _angle -= 360;
    }

    update();
}
