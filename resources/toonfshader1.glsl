#if __VERSION__<130
#define IN varying
#define OUT varying
#else
#define IN in
#define OUT out
#endif
#define LMAX 3


IN float intensity;

struct Material
{
    vec4 diffuse;
    vec4 especular;
    vec4 ambient;
    int exp_especular;
};
uniform Material m;

void main()
{
        if (intensity > 0.95){
            gl_FragColor = m.diffuse*1.0;
        }
        else if (intensity > 0.5){

            gl_FragColor = m.diffuse*0.75;
        }
        else if (intensity > 0.25) {

            gl_FragColor = m.diffuse*0.5;
        }
        else{
            gl_FragColor = m.diffuse*0.3;
        }

}
