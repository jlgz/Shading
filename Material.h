#ifndef MATERIAL_H
#define MATERIAL_H
#include <Common.h>
#include <QGLShaderProgram>
// TO DO: A canviar a la fase 1 de la practica 2
// Classe que representa els materials d'un objecte
typedef Common::vec4  point4;

class Material {
public:
    Material();
    ~Material();
    void toGPU(QGLShaderProgram *program);
    //material
    point4 diffuse;
    point4 especular;
    point4 ambient;
    int exp_especular;
    struct {
        GLuint diffuse;
        GLuint especular;
        GLuint ambient;
        GLuint exp_especular;
    }gl_id_struct;
};

#endif // MATERIAL_H
