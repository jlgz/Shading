#if __VERSION__<130
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif
#define LMAX 3

IN vec4 vPosition;
IN vec4 vNormal;
OUT float intensity;

struct Light
{
    vec4 diffuse;
    vec4 especular;
    vec4 ambient;
    vec3 coef;
    vec4 pos;
    vec4 direction;
    float angle;
    int type;
    int disabled;
};
uniform vec4 ambient_scene;

uniform Light l[3];
uniform vec4 cam_origin;
uniform mat4 model_view;
uniform mat4 projection;
void main()
{
    intensity=0.01;
    int i = 0;
    vec4 L,V,H;
    float d;
    for(i = 0;i<3;i++){
        if(l[i].disabled == 0){//POINT LIGHT
            if (l[i].type == 0){// POINT LIGHT
                L = l[i].pos - vPosition;
                d = sqrt(L[0]*L[0] + L[1]*L[1] + L[2]*L[2]);
                L /= d;
                intensity+=max(dot(vNormal,L),0.0);
            }
            if (l[i].type == 1){// DIRECTIONAL LIGHT
                L = l[i].direction;
                intensity+=max(dot(vNormal,L),0.0);
            }
            if (l[i].type == 2){// SPOT LIGHT
                L = l[i].pos - vPosition;
                L[3]=0.0;
                d = sqrt(L[0]*L[0] + L[1]*L[1] + L[2]*L[2]);
                L /= d;
                if (dot(L, l[i].direction)>l[i].angle){
                    intensity+=max(dot(vNormal,L),0.0);


                }
            }
        }
    }

    gl_Position = projection*model_view*vPosition;
}
