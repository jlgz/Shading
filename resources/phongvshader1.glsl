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

OUT vec4 normal;
OUT vec4 v;

uniform mat4 model_view;
uniform mat4 projection;
void main()
{
    normal = vNormal;
    gl_Position =projection*model_view*vPosition;
    v = vPosition;
}
