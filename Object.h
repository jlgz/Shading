#ifndef Object_H
#define Object_H

#include <QObject>
#include <vector>
#include <Common.h>
#include <cara.h>
#include <QGLShaderProgram>
#include <QOpenGLTexture>
#include<Material.h>
#include <math.h>
using namespace std;
typedef Common::vec4  point4; typedef Common::vec2 point2;
// Classe que conte la representacio d'un Objecte: geometria, topologia, material i textures
class Object : public QObject {

    Q_OBJECT
protected:
    QString nom;
    vector<Cara> cares;
    vector<point4> vertexs;
    Capsa3D capsa;
    Material *m;
    GLuint buffer;
    GLuint buffer2;
    QOpenGLTexture *texture;
    QOpenGLTexture *textureb;
    int     numPoints;
    point4 *points;
    point4 *normals;
    point4 *normals_v;
    point2 *vertexsTextura;
    // Els colors s'usen en la primera execució però després son prescindibles
    int *cont_normal;
    int *ids_points;
    int Index; // index de control del numero de vertexs a passar a la GPU
    QGLShaderProgram *program;
    bool bump;

public:

    Object(const int npoints, QObject *parent = 0);
    Object(const int npoints, QString n);
    ~Object();

    virtual void readObj(QString filename);

    virtual void make();
    virtual void toGPU(QGLShaderProgram *p);
    virtual void toGPUTexture(QGLShaderProgram *pr);

    virtual void draw();
    virtual void drawTexture();

    Capsa3D calculCapsa3D();
    void initBump();

private:

    void construeix_cara ( char **words, int nwords);
    void initTextura();
};



#endif // Object_H
