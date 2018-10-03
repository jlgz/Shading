#define MAX_L 3
#ifndef SCENE_H
#define SCENE_H

#include <cmath>
#include <iostream>
#include <stdlib.h>

#include <list>
#include <vector>
#include <string>
#include <stdio.h>

#include <Common.h>
#include <Object.h>
#include <Light.h>

using namespace std;

// Scene: contains all objects, lights to visualize the scene
// the viewer is placed in the z+ axis and the  visible frustum is enclosed to (-1,-1,-1) to (1,1,1)

class Scene {

public:
    GLuint ambient_scene;
    struct {
        GLuint diffuse;
        GLuint especular;
        GLuint ambient;
        GLuint coef;
        GLuint pos;
        GLuint direction;
        GLuint angle;
        GLuint type;
        GLuint disabled;
    }gl_id_lights[MAX_L];
    Scene();
    ~Scene();

    void addObject(Object *obj);


    void draw();
    void drawTexture();
    void lightDirToGPU(QGLShaderProgram *program);
    void   addLight(Light *l);
    Light *getLightActual();
    void   setLightActual(Light* l);

    void lightsToGPU(QGLShaderProgram *program);
    void toGPUScene(QGLShaderProgram *program);
    void toGPUSceneT(QGLShaderProgram *program);
    void activeBumpMap();
    void lightToGPU();
    void setAmbientToGPU(QGLShaderProgram *program);
    void typeToGPU();
    void positionToGPU();
    void difuseToGPU();
    void disableToGPU();
    void initScene();

    vector<Object*> elements;
    vector<Light*>  lights;

    vec3    lightAmbientGlobal;

    // Capsa contenedora de l'escena
    Capsa3D capsaMinima;
    void    calculCapsaMinCont3DEscena();

};

#endif // SCENE_H
