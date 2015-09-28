#include "terraingenerator.h"

#define SIZE 8.0

static const char *vertexShaderSource =
    "attribute highp vec4 posAttr;\n"
    "attribute lowp vec4 colAttr;\n"
    "varying lowp vec4 col;\n"
    "uniform highp mat4 matrix;\n"
    "void main() {\n"
    "   col = colAttr;\n"
    "   gl_Position = matrix * posAttr;\n"
    "}\n";

static const char *fragmentShaderSource =
    "varying lowp vec4 col;\n"
    "void main() {\n"
    "   gl_FragColor = col;\n"
    "}\n";

TerrainGenerator::TerrainGenerator() : m_program(0), m_frame(0)
{
    _width = 64;
    _height = 64;
    _wireFrame = false;
    _camX = 0;
    _camY = 0;
    _camZ = -2;

    _rotX = 0;
}

TerrainGenerator::TerrainGenerator(int w, int h) : m_program(0), m_frame(0)
{
    _width = w;
    _height = h;
    _wireFrame = false;
    _camX = 0;
    _camY = 0;
    _camZ = -2;

    _rotX = 0;
}

GLuint TerrainGenerator::loadShader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, 0);
    glCompileShader(shader);
    return shader;
}

void TerrainGenerator::initialize()
{
    m_program = new QOpenGLShaderProgram(this);
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    m_program->link();
    m_posAttr = m_program->attributeLocation("posAttr");
    m_colAttr = m_program->attributeLocation("colAttr");
    m_matrixUniform = m_program->uniformLocation("matrix");

    generateTerrain();
}

void TerrainGenerator::generateTerrain()
{
    int ecart = 1;
    GLfloat posX = 0;
    GLfloat posZ = 0;
    GLfloat scale = 0.05;

//    system("pwd");
//    exit(0);

    QImage image;
    if (QFile::exists(":/heightmap-1.png")) {
        if(!image.load(":/heightmap-1.png"))
        {
            std::cout << "image non chargé ";
            exit(0);
        }
    }
    else
    {
        std::cout << "image not found ";
    }

    for(int x = 0; x < _height; x+=ecart)
    {
        posX = x*scale;
        unsigned char* line = image.scanLine(x);
        for(int z = 0; z < _width; z+=ecart)
        {
            posZ = z*scale;
            _color.push_back(1.0f); _color.push_back(1.0f); _color.push_back(1.0f);
            _color.push_back(1.0f); _color.push_back(1.0f); _color.push_back(1.0f);
            _color.push_back(1.0f); _color.push_back(1.0f); _color.push_back(1.0f);

            ///////
            GLfloat y = 1.f - 1.f/(GLfloat)line[z*4];
            _map.push_back(posX); _map.push_back(y*2); _map.push_back(posZ);
            _map.push_back(posX); _map.push_back(y*2); _map.push_back((z+ecart)*scale);
            _map.push_back((x+ecart)*scale); _map.push_back(y*2); _map.push_back((z+ecart)*scale);

            _map.push_back(posX); _map.push_back(y*2); _map.push_back(posZ);
            _map.push_back((x+ecart)*scale); _map.push_back(y*2); _map.push_back((z+ecart)*scale);
            _map.push_back((x+ecart)*scale); _map.push_back(y*2); _map.push_back(posZ);
        }
    }
}

void TerrainGenerator::render()
{
    const qreal retinaScale = devicePixelRatio();
    glViewport(0, 0, width() * retinaScale, height() * retinaScale);

    glClear(GL_COLOR_BUFFER_BIT);

    if(_wireFrame)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    m_program->bind();

    QMatrix4x4 matrix;
    matrix.perspective(60.0f, 16.0f/9.0f, 0.1f, 100.0f);
    matrix.translate(_camX, _camY, _camZ);
    matrix.rotate(0, 0, 0, 0);
    matrix.rotate(100.0f * _rotX / screen()->refreshRate(), 0, 1, 0);

    m_program->setUniformValue(m_matrixUniform, matrix);

    //on remplie...
    //Num du tableau, nb coordonnée par vertex (2D = 2), type, normalized, ?, pointeur sur données
    glVertexAttribPointer(m_posAttr, 3, GL_FLOAT, GL_FALSE, 0, _map.constData());
    glVertexAttribPointer(m_colAttr, 3, GL_FLOAT, GL_FALSE, 0, _color.constData());

    //... puis active le tableau vertex attrib...
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    //on affiche le triangle
    glDrawArrays(GL_TRIANGLES, 0, _width * _height * 6); //mode : forme final, indice du premier vertex, nb vertex

    //on désactive le tableau vertex attrib
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);

    m_program->release();

    ++m_frame;
}

void TerrainGenerator::keyPressEvent(QKeyEvent *keyEvent)
{
    switch(keyEvent->key())
    {
        case Qt::Key_Escape:
            close();
            break;
        case Qt::Key_W:
            _wireFrame = !_wireFrame;
            break;
        case Qt::Key_Z:
            _camZ += 0.1;
            break;
        case Qt::Key_S:
            _camZ -= 0.1;
            break;
        case Qt::Key_Q:
            _camX += 0.1;
            break;
        case Qt::Key_D:
            _camX -= 0.1;
            break;
        case Qt::Key_P:
            _camY += 0.1;
            break;
        case Qt::Key_M:
            _camY -= 0.1;
            break;
        case Qt::Key_J:
            _rotX += 1.0;
            break;
        case Qt::Key_L:
            _rotX -= 1.0;
            break;
    }
}

