#include "Scene.h"

/**
 * @brief Scene::Scene
 */
Scene::Scene() {
    // Inicialització de la capsa minima
    capsaMinima.pmin = vec3(-1.0, -1.0,-1.0);
    capsaMinima.a = 2;
    capsaMinima.h = 2;
    capsaMinima.p = 2;
    lightAmbientGlobal = vec3(0.2, 0.2, 0.2);
}

/**
 * @brief Scene::~Scene
 */
Scene::~Scene() {
    elements.clear();
    lights.clear();
}

/**
 * @brief Scene::addObject
 * @param obj
 */
void Scene::addObject(Object *obj) {
    elements.push_back(obj);
    calculCapsaMinCont3DEscena();
}

/**
 * @brief Scene::draw
 */
void Scene::draw() {
    for(unsigned int i=0; i < elements.size(); i++){
        elements.at(i)->draw();
    }
}

/**
 * @brief Scene::drawTexture
 */
void Scene::drawTexture() {
    for(unsigned int i=0; i < elements.size(); i++){
        elements.at(i)->drawTexture();
    }
}

/**
 * @brief Scene::getLightActual
 * @return
 */
Light* Scene::getLightActual() {
    // TO DO OPCIONAL: A modificar si es vol canviar el comportament de la GUI
    // Ara per ara dona com a Light actual la darrera que s'ha inserit
    return (lights[lights.size()-1]);
}

/**
 * @brief Scene::setLightActual
 * @param l
 */
void Scene::setLightActual(Light* l){
    lights[lights.size()-1]=l;
}
void Scene::lightDirToGPU(QGLShaderProgram *program){
    GLuint disabl;
    GLuint diffuse;
    GLuint especular;
    GLuint ambient;
    GLuint direction;
    GLuint angle;
    GLuint type;
    GLuint disabled;
    for(int i=0;i<2;i++){
        disabl =program->uniformLocation(QString("l[%1].disabled").arg(i));
        glUniform1i(disabl,1);
    }
    Light *l2 = new Light(Direccional);
    l2->direction= vec4(0.0,1.0,0,0); //Direction must be uniform vector FROM viewer To light (opposite to actual light direction) - to reduce unnecesari GPU operations
    l2->setEstaActivat(true);
    l2->setDiffuseIntensity(vec3(0.8));
    l2->setIa(vec3(0.7));
    l2->setIs(vec3(0.7));
    ambient=program->uniformLocation(QString("l[%1].ambient").arg(2));
    diffuse=program->uniformLocation(QString("l[%1].diffuse").arg(2));
    especular=program->uniformLocation(QString("l[%1].especular").arg(2));
    type=program->uniformLocation(QString("l[%1].type").arg(2));
    direction=program->uniformLocation(QString("l[%1].direction").arg(2));
    disabled =program->uniformLocation(QString("l[%1].disabled").arg(2));
    glUniform4fv(ambient, 1,vec4(l2->getIa(),1.0));
    glUniform4fv(diffuse, 1,vec4(l2->getDiffuseIntensity(),1.0));
    glUniform4fv(especular, 1,vec4(l2->getIs(),1.0));
    glUniform4fv(direction, 1,l2->direction);
    glUniform1i(type,l2->getTypeInt());
    glUniform1i(disabled,l2->getDisabled());


}
void Scene::toGPUScene(QGLShaderProgram *program){
    for(unsigned int i = 0; i< this->elements.size();i++){
        this->elements[i]->toGPU(program);
    }
}
void Scene::toGPUSceneT(QGLShaderProgram *program){
    for(unsigned int i = 0; i< this->elements.size();i++){
        this->elements[i]->toGPUTexture(program);
    }
}
void Scene::activeBumpMap()
{
    for(unsigned int i = 0; i< this->elements.size();i++){
        this->elements[i]->initBump();
    }
}

/**
 * @brief Scene::lightsToGPU
 * @param program
 */
void Scene::lightsToGPU(QGLShaderProgram *program){
// TO DO: A implementar a la fase 1 de la practica 2
    for(int i=0;i<3;i++){
        this->gl_id_lights[i].ambient=program->uniformLocation(QString("l[%1].ambient").arg(i));
        this->gl_id_lights[i].diffuse=program->uniformLocation(QString("l[%1].diffuse").arg(i));
        this->gl_id_lights[i].especular=program->uniformLocation(QString("l[%1].especular").arg(i));
        this->gl_id_lights[i].coef=program->uniformLocation(QString("l[%1].coef").arg(i));
        this->gl_id_lights[i].type=program->uniformLocation(QString("l[%1].type").arg(i));
        this->gl_id_lights[i].pos=program->uniformLocation(QString("l[%1].pos").arg(i));
        this->gl_id_lights[i].angle=program->uniformLocation(QString("l[%1].angle").arg(i));
        this->gl_id_lights[i].direction=program->uniformLocation(QString("l[%1].direction").arg(i));
        this->gl_id_lights[i].disabled =program->uniformLocation(QString("l[%1].disabled").arg(i));
        glUniform4fv(this->gl_id_lights[i].ambient, 1,vec4(this->lights[i]->getIa(),1.0));
        glUniform4fv(this->gl_id_lights[i].diffuse, 1,vec4(this->lights[i]->getDiffuseIntensity(),1.0));
        glUniform4fv(this->gl_id_lights[i].especular, 1,vec4(this->lights[i]->getIs(),1.0));
        glUniform3fv(this->gl_id_lights[i].coef, 1,this->lights[i]->getCoeficients());
        glUniform4fv(this->gl_id_lights[i].pos, 1,vec4(this->lights[i]->getLightPosition()));
        if(this->lights[i]->getTipusLight() != Puntual){
            glUniform4fv(this->gl_id_lights[i].direction, 1,this->lights[i]->direction);
            glUniform1f(this->gl_id_lights[i].angle,this->lights[i]->angle);
        }
        glUniform1i(this->gl_id_lights[i].type,this->lights[i]->getTypeInt());
        glUniform1i(this->gl_id_lights[i].disabled,this->lights[i]->getDisabled());
    }
}
void Scene::lightToGPU(){
    int i;
    if(this->lights.size() <= 3){
        i = this->lights.size() -1;
    }
    else{
        i = 3;
    }
    glUniform4fv(this->gl_id_lights[ i ].ambient, 1,vec4(this->lights[this->lights.size()-1]->getIa(),1.0));
    glUniform4fv(this->gl_id_lights[ i ].diffuse, 1,vec4(this->lights[this->lights.size()-1]->getDiffuseIntensity(),1.0));
    glUniform4fv(this->gl_id_lights[ i ].especular, 1,vec4(this->lights[this->lights.size()-1]->getIs(),1.0));
    glUniform3fv(this->gl_id_lights[ i ].coef, 1,this->lights[this->lights.size()-1]->getCoeficients());
    glUniform4fv(this->gl_id_lights[ i ].pos, 1,vec4(this->lights[this->lights.size()-1]->getLightPosition()));
    glUniform4fv(this->gl_id_lights[ i ].direction, 1,this->lights[this->lights.size()-1]->direction);
    glUniform1f(this->gl_id_lights[ i ].angle,this->lights[this->lights.size()-1]->angle);
    glUniform1i(this->gl_id_lights[ i ].type,this->lights[this->lights.size()-1]->getTypeInt());
    glUniform1i(this->gl_id_lights[ i ].disabled,this->lights[this->lights.size()-1]->getDisabled());
}



void Scene::typeToGPU(){
    int i;
    if(this->lights.size() <= 3){
        i = this->lights.size() -1;
    }
    else{
        i = 3;
    }
    glUniform1i(this->gl_id_lights[i].type,this->lights[this->lights.size()-1]->getTypeInt());
}

void Scene::positionToGPU(){
    int i;
    if(this->lights.size() <= 3){
        i = this->lights.size() -1;
    }
    else{
        i = 3;
    }
    glUniform4fv(this->gl_id_lights[ i ].pos, 1,vec4(this->lights[this->lights.size()-1]->getLightPosition()));
}
void Scene::difuseToGPU(){
    int i;
    if(this->lights.size() <= 3){
        i = this->lights.size() -1;
    }
    else{
        i = 3;
    }
    glUniform4fv(this->gl_id_lights[ i ].diffuse, 1,vec4(this->lights[this->lights.size()-1]->getDiffuseIntensity(),1.0));
}
void Scene::disableToGPU(){
    int i;
    if(this->lights.size() <= 3){
        i = this->lights.size() -1;
    }
    else{
        i = 3;
    }
    glUniform1i(this->gl_id_lights[ i ].disabled,this->lights[this->lights.size()-1]->getDisabled());
}
void Scene::addLight(Light *l) {
    lights.push_back(l);

}

/**
 * @brief Scene::setAmbientToGPU
 * @param program
 */
void Scene::setAmbientToGPU(QGLShaderProgram *program){
    // TO DO: A implementar a la fase 1 de la practica 2
    this->ambient_scene =program->uniformLocation(QString("ambient_scene"));
    vec4 aux = vec4(this->lightAmbientGlobal,1.0);
    glUniform4fv(this->ambient_scene, 1,aux);

}
void Scene::initScene(){
    Light *l1 = new Light(Puntual);
    this->addLight(l1);
    //this->addLight(l1);
    //this->addLight(l1);//placeholder loght  - keep 3 lights in scene

    //Directional light
    Light *l2 = new Light(Direccional);
    l2->direction= vec4(0.0,1.0,0,0); //Direction must be uniform vector FROM viewer To light (opposite to actual light direction) - to reduce unnecesari GPU operations
    l2->setEstaActivat(true);
    l2->setDiffuseIntensity(vec3(0.8));
    l2->setIa(vec3(0.7));
    l2->setIs(vec3(0.7));
    this->addLight(l2);
/*
    //Point light
    Light *l = new Light(Puntual);
    l->setLightPosition(vec4(.0,2.0,0.0,1));
    l->setEstaActivat(true);
    l->setCoeficients(vec3(0.1,0.2,0.5));
    l->setDiffuseIntensity(vec3(0.8));
    l->setIa(vec3(0.1));
    l->setIs(vec3(0.9));
    this->addLight(l);*/

    //Spot light
    Light *m = new Light(Spot);
    m->setLightPosition(vec4(2.0,0.0,0.0,1));
    m->angle=cos(0.75);   //directn conversion to cosinus - to reduce gpu usage
    m->direction= normalize(vec4(1.0,0,0,0));
    m->setEstaActivat(true);
    m->setCoeficients(vec3(0.1,0.2,0.5));
    m->setDiffuseIntensity(vec3(0.8));
    m->setIa(vec3(0.1));
    m->setIs(vec3(0.9));
    this->addLight(m);
}


void Scene::calculCapsaMinCont3DEscena()
{
    Capsa3D c;
    vec3 pmax;

    if (elements.size()==1) {
        capsaMinima = elements[0]->calculCapsa3D();
        pmax[0] = capsaMinima.pmin[0]+capsaMinima.a;
        pmax[1] = capsaMinima.pmin[1]+capsaMinima.h;
        pmax[2] = capsaMinima.pmin[2]+capsaMinima.p;
    } else {
        capsaMinima.pmin[0]=200;
        capsaMinima.pmin[1]=200;
        capsaMinima.pmin[2]=200;
        pmax[0] = -200;
        pmax[1] = -200;
        pmax[2] = -200;
    }

    for (unsigned int i=0; i<elements.size(); i++) {
       c = elements[i]->calculCapsa3D();

       if (capsaMinima.pmin[0]>c.pmin[0]) capsaMinima.pmin[0] = c.pmin[0];
       if (capsaMinima.pmin[1]>c.pmin[1]) capsaMinima.pmin[1] = c.pmin[1];
       if (capsaMinima.pmin[2]>c.pmin[2]) capsaMinima.pmin[2] = c.pmin[2];
       if (pmax[0]<c.pmin[0]+c.a) pmax[0] = c.pmin[0]+c.a;
       if (pmax[1]<c.pmin[1]+c.h) pmax[1] = c.pmin[1]+c.h;
       if (pmax[2]<c.pmin[2]+c.p) pmax[2] = c.pmin[2]+c.p;
    }
    capsaMinima.a = pmax[0]-capsaMinima.pmin[0];
    capsaMinima.h = pmax[1]-capsaMinima.pmin[1];
    capsaMinima.p = pmax[2]-capsaMinima.pmin[2];
}
