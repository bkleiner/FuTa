#ifndef HEIGHTVIEW_H
#define HEIGHTVIEW_H

#include <QGLWidget>
#include <GL/glu.h>
#include "file/adtfile.h"
#include <QMouseEvent>

class heightView : public QGLWidget
{
    Q_OBJECT
public:
    heightView(AdtFile *adtfile, QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void timerEvent(QTimerEvent *);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    //void keyPressEvent ( QKeyEvent * event );

private:
    GLuint greensmall;
    GLuint redsmall;
    GLuint alphamap;
    GLfloat xRot;
    GLfloat yRot;
    GLfloat distance;
    QPoint lastPos;
    float _angle;
    AdtFile *adtFile;

    void drawPass(int cx, int cy);

    
signals:
    
public slots:
    
};

#endif // HEIGHTVIEW_H
