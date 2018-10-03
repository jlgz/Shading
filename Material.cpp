#include "Material.h"


Material::Material(): diffuse(vec4(0.5,0,0,0.1)), especular(vec4(0.9)), ambient(vec4(0.3,0,0,0.1)), exp_especular(50)
{}

/**
 * Passa el material de CPU a GPU
 * @brief Material::toGPU
 * @param program
 */
void Material::toGPU(QGLShaderProgram *program){
    this->gl_id_struct.ambient = program->uniformLocation("m.ambient");
    this->gl_id_struct.diffuse = program->uniformLocation("m.diffuse");
    this->gl_id_struct.especular = program->uniformLocation("m.especular");
    this->gl_id_struct.exp_especular = program->uniformLocation("m.exp_especular");
    //bind
    glUniform4fv(this->gl_id_struct.ambient,1,this->ambient);
    glUniform4fv(this->gl_id_struct.diffuse,1,this->diffuse);
    glUniform4fv(this->gl_id_struct.especular,1,this->especular);
    glUniform1i(this->gl_id_struct.exp_especular,this->exp_especular);


}

