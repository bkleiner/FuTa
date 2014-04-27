#ifndef MODELVIEW_H
#define MODELVIEW_H

#include <QGLWidget>
#include <GL/glu.h>
#include "file/m2file.h"
#include <QMouseEvent>

class modelView : public QGLWidget
{
    Q_OBJECT
public:
    modelView(QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void timerEvent(QTimerEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    //void keyPressEvent ( QKeyEvent * event );

private:
    GLuint _textureId;
    GLfloat xRot;
    GLfloat yRot;
    GLfloat distance;
    QPoint lastPos;
    float _angle;
    m2File *m2file;

    
};

#endif // MODELVIEW_H
