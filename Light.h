#ifndef Light_H
#define Light_H

#include <Common.h>

// Tipus de Lights
enum LightType {Puntual, Direccional, Spot};

// Classe que representa els atributs d'una Light
class Light {
    public:
        LightType type;
        vec4 pos;// lighsource position in scene
        vec4 direction;
        vec3 ambient; // ambient component
        vec3 diffuse;// diffuse component
        vec3 specular; //specular component
        float a; // abc params of light attenuation
        float b;
        float c;
        float angle;
        bool disabled;
        Light(LightType l);
        ~Light();
        vec4 getLightPosition();
        void setLightPosition(vec4 v);

        void switchOnOff();

        vec3 getIa() const;
        void setIa(const vec3 &value);

        vec3 getDiffuseIntensity();
        void setDiffuseIntensity(vec3 i);

        vec3 getIs() const;
        void setIs(const vec3 &value);

        vec3 getCoeficients() const;
        void setCoeficients(const vec3 &value);

        bool getEstaActivat() const;
        void setEstaActivat(bool value);

        LightType getTipusLight() const;
        void setTipusLight(const LightType &value);
        int getTypeInt() const;
        int getDisabled() const;
        vec4 getLightDirection();

};

#endif // Light_H
