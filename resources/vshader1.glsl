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
    int disabled
};
uniform vec4 ambient_scene;
uniform Material m;
uniform Light l[LMAX];
uniform vec4 cam_origin;
void main()
{
    int i = 0;
    vec4 L,V,H;
    float d;
    color = ambient_scene*m.ambient;
    for(i = 0;i<LMAX;i++){
        L = l[i].pos - vPosition;
        d = sqrt(L[0]*L[0] + L[1]*L[1] + L[2]*L[2]);
        L /= d;
        V = normalize(cam_origin - vPosition);
        H = normalize(L+V);
        color += 1.0/(d*d*l[i].coef[0] + d*l[i].coef[1] + l[i].coef[2])*((l[i].diffuse*m.diffuse*max(dot(L,vNormal),0.0) + l[i].especular* m.especular *pow(max(dot(vNormal,H),0.0),m.exp_especular)) + l[i].ambient * m.ambient);
    }
    color[0] = min(color[0],1.0);
    color[1] = min(color[1],1.0);
    color[2] = min(color[2],1.0);
    gl_Position = vPosition;
}
