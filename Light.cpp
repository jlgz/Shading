#include "Light.h"

/**
 * @brief Light::Light
 * @param t
 */
Light::Light(LightType t) {
    this->type = t;
    this->disabled = true;
}

/**
 * @brief Light::getDiffuseIntensity
 * @return
 */
vec3 Light::getDiffuseIntensity() {
// TO DO: A canviar a la fase 1 de la practica 2
    if(this->disabled){return vec3(0,0,0);}
    else{return this->diffuse;}
}

/**
 * @brief Light::setDiffuseIntensity
 * @param i
 */
void Light::setDiffuseIntensity(vec3 i) {
    // TO DO: A canviar a la fase 1 de la practica 2
    // el float que es reb ha de multiplicar els tres valors de la intensitat difusa de la Light
    this->diffuse = i;

}
vec4 Light::getLightPosition() {
    // A canviar
    if (this->pos!=NULL){return this->pos;}
    else{return(vec4(0.0, 0.0, 0.0, 0.0));}
}
vec4 Light::getLightDirection() {
    // A canviar
    if (this->direction!=NULL){return this->direction;}
    else{return(vec4(0.0, 0.0, 0.0, 0.0));}
}
void Light::setLightPosition(vec4 v) {
    this->pos = v;
}

/**
 * @brief Light::switchOnOff
 */
void Light::switchOnOff() {
    // TO DO: A canviar a la fase 1 de la practica 2
    // Si esta en on la posa a off i a l'inreves
    this->disabled = !this->disabled;
 }

/**
 * @brief Light::getIa
 * @return
 */
vec3 Light::getIa() const
{
    // TO DO: A canviar a la fase 1 de la practica 2
       if(this->disabled){return(vec3(0, 0, 0));}
       else{return this->ambient;}
}

/**
 * @brief Light::setIa
 * @param value
 */
void Light::setIa(const vec3 &value)
{
    // TO DO: A canviar a la fase 1 de la practica 2
    // el float que es reb ha de multiplicar els tres valors de la intensitat difusa de la Light
    this->ambient = value;

}



/**
 * @brief Light::getIs
 * @return
 */
vec3 Light::getIs() const
{
    // TO DO: A canviar a la fase 1 de la practica 2
       if(this->disabled){return(vec3(0, 0, 0));}
       else{return this->specular;}
}

/**
 * @brief Light::setIs
 * @param value
 */
void Light::setIs(const vec3 &value)
{
    // TO DO: A canviar a la fase 1 de la practica 2
    // el float que es reb ha de multiplicar els tres valors de la intensitat difusa de la Light
    this->specular = value;
}

/**
 * @brief Light::getCoeficients
 * @return
 */
vec3 Light::getCoeficients() const
{
    // TO DO: A canviar a la fase 1 de la practica 2

       return(vec3(this->a, this->b, this->c));
}

/**
 * @brief Light::setCoeficients
 * @param value
 */
void Light::setCoeficients(const vec3 &value)
{
    // TO DO: A canviar a la fase 1 de la practica 2
    // el float que es reb ha de multiplicar els tres valors de la intensitat difusa de la Light
    this->a = value[0];
    this->b = value[1];
    this->c = value[2];
}

/**
 * @brief Light::getEstaActivat
 * @return
 */
bool Light::getEstaActivat() const
{
    // TO DO: A canviar a la fase 1 de la practica 2

       return !(this->disabled);
}

/**
 * @brief Light::setEstaActivat
 * @param value
 */
void Light::setEstaActivat(bool value)
{
    // TO DO: A canviar a la fase 1 de la practica 2
    this->disabled = !(value);
}
int Light::getDisabled() const
{
    if(disabled){
        return 1;}
    else{
        return 0;}
}
/**
 * @brief Light::getTipusLight
 *
 */
LightType Light::getTipusLight() const
{
    // TO DO: A canviar a la fase 1 de la practica 2
    return this->type;
}
int Light::getTypeInt() const
{
    switch (this->type) {
    case Puntual:
        return 0;
    case Spot:
        return 2;
    case Direccional:
        return 1;
    default:
        return 3;
    }
}
/**
 * @brief Light::setTipusLight
 * @param value
 */
void Light::setTipusLight(const LightType &value)
{
    // TO DO: A canviar a la fase 1 de la practica 2
    this->type = value;
}
