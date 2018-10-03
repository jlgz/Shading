#if __VERSION__<130
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif
#define LMAX 3

IN vec4 vNormal;
IN vec4 vPosition;

OUT vec4 color;
struct Material
{
    vec4 diffuse;
    vec4 especular;
    vec4 ambient;
    int exp_especular;
};
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
uniform Material m;
uniform Light l[3];
uniform vec4 cam_origin;
uniform mat4 model_view;
uniform mat4 projection;
void main()
{
    int i = 0;
    vec4 L,V,H;
    float d;
    color = ambient_scene*m.ambient;
    for(i = 0;i<3;i++){
        if(l[i].disabled == 0){
            if (l[i].type == 0){// POINT LIGHT
                L = l[i].pos - vPosition;
                L[3]=0.0;
                d = sqrt(L[0]*L[0] + L[1]*L[1] + L[2]*L[2]);
                L /= d;
                V = normalize(cam_origin - vPosition);
                H = normalize(L+V);
                color += 1.0/(d*d*l[i].coef[0] + d*l[i].coef[1] + l[i].coef[2])*((l[i].diffuse*m.diffuse*max(dot(L,vNormal),0.0))*(1.0-dot(vNormal,V)) + l[i].especular* m.especular *pow(max(dot(vNormal,H),0.0),m.exp_especular)) + l[i].ambient * m.ambient;
            }else if(l[i].type == 1){// DIRECTIONAL LIGHT
                L = l[i].direction;
                L[3]=0.0;
                V = normalize(cam_origin - vPosition);
                H = normalize(L+V);
                color += ((l[i].diffuse*m.diffuse*max(dot(L,vNormal),0.0)) + l[i].especular* m.especular *pow(max(dot(vNormal,H),0.0),m.exp_especular)) + l[i].ambient * m.ambient;
            }else if(l[i].type == 2){// SPOT LIGHT
                L = l[i].pos - vPosition;
                L[3]=0.0;
                d = sqrt(L[0]*L[0] + L[1]*L[1] + L[2]*L[2]);
                L /= d;
                if (dot(L, l[i].direction)>l[i].angle){
                    V = normalize(cam_origin - vPosition);
                    H = normalize(L+V);
                    color += 1.0/(d*d*l[i].coef[0] + d*l[i].coef[1] + l[i].coef[2])*((l[i].diffuse*m.diffuse*max(dot(L,vNormal),0.0)) + l[i].especular* m.especular *pow(max(dot(vNormal,H),0.0),m.exp_especular)) + l[i].ambient * m.ambient;

                }


            }
        }
    }
    gl_Position = projection*model_view*vPosition;
}
