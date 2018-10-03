#if __VERSION__<130
#define IN varying
#define OUT varying
#else
#define IN in
#define OUT out
#endif
#define LMAX 3

IN vec4 normal;
IN vec4 v;
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
uniform Light l[LMAX];
uniform vec4 cam_origin;
void main()
{ 
    int i = 0;
    vec4 L,V,H,color;
    float d;
    color = ambient_scene*m.ambient;
    for(i = 0;i<LMAX;i++){
        if(l[i].disabled == 0){
            if (l[i].type == 0){// POINT LIGHT
                L = l[i].pos - v;
                d = sqrt(L[0]*L[0] + L[1]*L[1] + L[2]*L[2]);
                L /= d;
                V = normalize(cam_origin - v);
                H = normalize(L+V);
                color += 1.0/(d*d*l[i].coef[0] + d*l[i].coef[1] + l[i].coef[2])*(l[i].diffuse*m.diffuse*max(dot(L,normal),0.0) + l[i].especular* m.especular *pow(max(dot(normal,H),0.0),m.exp_especular)) + l[i].ambient * m.ambient;
                //(l[i].diffuse*m.diffuse*max(dot(L,normal),0.0)*(1.0-dot(normal,V))
            }else if(l[i].type == 1){// DIRECTIONAL LIGHT
                L = l[i].direction;
                d = sqrt(L[0]*L[0] + L[1]*L[1] + L[2]*L[2]);
                L /= d;
                V = normalize(cam_origin - v);
                H = normalize(L+V);
                color += (l[i].diffuse*m.diffuse*max(dot(L,normal),0.0) + l[i].especular* m.especular *pow(max(dot(normal,H),0.0),m.exp_especular)) + l[i].ambient * m.ambient;


            }else if(l[i].type == 2){// SPOT LIGHT
                L = l[i].pos - v;
                L[3]=0.0;
                d = sqrt(L[0]*L[0] + L[1]*L[1] + L[2]*L[2]);
                L /= d;
                if (dot(L, l[i].direction)>l[i].angle){
                    V = normalize(cam_origin - v);
                    H = normalize(L+V);
                    color += 1.0/(d*d*l[i].coef[0] + d*l[i].coef[1] + l[i].coef[2])*(l[i].diffuse*m.diffuse*max(dot(L,normal),0.0) + l[i].especular* m.especular *pow(max(dot(normal,H),0.0),m.exp_especular)) + l[i].ambient * m.ambient;
                }
            }
        }
    }
    gl_FragColor = color;
}

