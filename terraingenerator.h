#ifndef TERRAINGENERATOR_H
#define TERRAINGENERATOR_H

#include "openglwindow.h"
#include <stdio.h>      /* printf, NULL */
#include <stdlib.h>
#include <time.h>

#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QScreen>
#include <QKeyEvent>
#include <QImage>
#include <QImageReader>
#include <iostream>
#include <QVector>

class TerrainGenerator : public OpenGLWindow
{
public:
    TerrainGenerator();
    TerrainGenerator(int, int);

    void initialize() Q_DECL_OVERRIDE;
    void render() Q_DECL_OVERRIDE;
    void keyPressEvent(QKeyEvent *keyEvent);

    void generateTerrain();

private:
    int _width;
    int _height;
    bool _wireFrame;

    GLfloat _camX;
    GLfloat _camY;
    GLfloat _camZ;


    GLfloat _rotX;

    QVector<GLfloat> _map;
    QVector<GLfloat> _color;
    GLuint loadShader(GLenum type, const char *source);

    GLuint m_posAttr;
    GLuint m_colAttr;
    GLuint m_matrixUniform;

    QOpenGLShaderProgram *m_program;
    int m_frame;
};

#endif // TERRAINGENERATOR_H
