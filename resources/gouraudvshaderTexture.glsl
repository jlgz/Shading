#if __VERSION__<130
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif
#define LMAX 3
#define M_PI  3.14159265358979323846
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
uniform sampler2D texMap;
uniform sampler2D bumpMap;
uniform int bump;
uniform vec4 cp;
void main()
{
    vec4 Normal;
    int i = 0;
    vec4 L,V,H;
    float d;
    vec2 vTexture;
    vec3 uni = normalize(vec3(vPosition[0]-cp[0],vPosition[1]-cp[1],vPosition[2]-cp[2]));
    vTexture[0] = 0.5 + atan(uni[2],uni[0] )/ (2.0*M_PI);
    vTexture[1] = 0.5 - asin(uni[1])/ M_PI;
    color = ambient_scene*m.ambient;
    if (bump ==1){
        Normal =  normalize(vNormal + (2.0*texture2D(bumpMap,vTexture) - vec4(1,1,1,0)));
    }
    else{
        Normal = vNormal;
    }
    for(i = 0;i<3;i++){
        if(l[i].disabled == 0){
            if (l[i].type == 0){// POINT LIGHT
                L = l[i].pos - vPosition;
                L[3]=0.0;
                d = sqrt(L[0]*L[0] + L[1]*L[1] + L[2]*L[2]);
                L /= d;
                V = normalize(cam_origin - vPosition);
                H = normalize(L+V);
                color += 1.0/(d*d*l[i].coef[0] + d*l[i].coef[1] + l[i].coef[2])*((texture2D(texMap, vTexture)*max(dot(L,Normal),0.0))*(1.0-dot(vNormal,V)) + l[i].especular* m.especular *pow(max(dot(Normal,H),0.0),m.exp_especular)) + l[i].ambient * m.ambient;
            }else if(l[i].type == 1){// DIRECTIONAL LIGHT
                L = l[i].direction;
                L[3]=0.0;
                V = normalize(cam_origin - vPosition);
                H = normalize(L+V);
                color += ((texture2D(texMap, vTexture)*max(dot(L,Normal),0.0)) + l[i].especular* m.especular *pow(max(dot(Normal,H),0.0),m.exp_especular)) + l[i].ambient * m.ambient;
            }else if(l[i].type == 2){// SPOT LIGHT
                L = l[i].pos - vPosition;
                L[3]=0.0;
                d = sqrt(L[0]*L[0] + L[1]*L[1] + L[2]*L[2]);
                L /= d;
                if (dot(L, l[i].direction)>l[i].angle){
                    V = normalize(cam_origin - vPosition);
                    H = normalize(L+V);
                    color += 1.0/(d*d*l[i].coef[0] + d*l[i].coef[1] + l[i].coef[2])*((texture2D(texMap, vTexture)*max(dot(L,Normal),0.0)) + l[i].especular* m.especular *pow(max(dot(Normal,H),0.0),m.exp_especular)) + l[i].ambient * m.ambient;

                }


            }
        }
    }
    gl_Position = projection*model_view*vPosition;
}
//normal = normalize(normal + (2*texture2D(bumpMap,vTexture) - vec4(1,1,1,0)) texture2D(texMap, vTexture);
