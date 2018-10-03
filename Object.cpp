#include <Object.h>
#include <readfile.h>

/**
 * @brief Object::Object
 * @param npoints
 * @param parent
 */
Object::Object(int npoints, QObject *parent) : QObject(parent) {
    numPoints = npoints;
    points = new point4[numPoints];
    normals= new point4[numPoints];
    cont_normal = new int[vertexs.size()];
    ids_points = new int[numPoints];
    vertexsTextura = new point2[numPoints];
    normals_v = new point4[vertexs.size()];
    this->m = new Material();
    for(unsigned i = 0; i< vertexs.size();i++){
        normals_v[i] = vec4(0);
        cont_normal[i] = 0;
    }
 }

/**
 * @brief Object::Object
 * @param npoints
 * @param n
 */
Object::Object(int npoints, QString n) : numPoints(npoints){
    points = new point4[numPoints];
    normals= new point4[numPoints];
    this->m = new Material();
    readObj(n);
    cont_normal = new int[vertexs.size()];
    ids_points = new int[numPoints];
    normals_v = new point4[vertexs.size()];
    vertexsTextura = new point2[numPoints];
    for(unsigned i = 0; i< vertexs.size();i++){
        normals_v[i] = vec4(0);
        cont_normal[i] = 0;
    }
    make();
}


/**
 * @brief Object::~Object
 */
Object::~Object(){
    delete points;
    delete normals;
    delete cont_normal;
    delete ids_points;
}

/**
 * @brief Object::toGPU
 * @param pr
 */
void Object::toGPU(QGLShaderProgram *pr) {
    // TO  DO: A modificar a la fase 1 de la practica 2
    // Cal passar les normals

    program = pr;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*Index + sizeof(point4)*Index, NULL, GL_STATIC_DRAW );
    glEnable( GL_DEPTH_TEST );
    GLuint cp = program->uniformLocation(QString("cp"));
    vec4 aux = vec4(capsa.pmin +vec3(capsa.a,capsa.h,capsa.p)/2,1.0);
    glUniform4fv(cp, 1,aux);


}


/**
 * Pintat en la GPU.
 * @brief Object::draw
 */
void Object::draw(){
    // TO  DO: A modificar a la fase 1 de la practica 2
    // Cal passar les normals a la GPU

    // Aqui es torna a repetir el pas de dades a la GPU per si hi ha més d'un objecte
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*Index, &points[0] );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*Index, sizeof(point4)*Index, &normals[0] );

    int vertexLocation = program->attributeLocation("vPosition");
    int normalLocation = program->attributeLocation("vNormal");

    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer("vPosition", GL_FLOAT, 0, 4);
    program->enableAttributeArray(normalLocation);
    program->setAttributeBuffer("vNormal", GL_FLOAT, sizeof(point4)*Index, 4);
    this->m->toGPU(program);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays( GL_TRIANGLES, 0, Index );
}

/**
 * @brief Object::make
 */
void Object::make(){
    float u, v;
    // TO  DO: A modificar a la fase 1 de la practica 2
    // Cal calcular la normal a cada vertex a la CPU
    this->initTextura();
    static vec3  base_colors[] = {
        vec3( 1.0, 0.0, 0.0 ),
        vec3( 0.0, 1.0, 0.0 ),
        vec3( 0.0, 0.0, 1.0 ),
        vec3( 1.0, 1.0, 0.0 )
    };

    Index = 0;
    for(unsigned int i=0; i<cares.size(); i++){
        for(unsigned int j=0; j<cares[i].idxVertices.size(); j++){
            points[Index] = vertexs[cares[i].idxVertices[j]];
            ids_points[Index] = cares[i].idxVertices[j];
            //colors[Index] = vec4(base_colors[j%4], 1.0);
           // normals[Index] = normalize(vec4(cares[i].normal,0));
            normals_v[cares[i].idxVertices[j]] += normalize(vec4(cares[i].normal,0));
            cont_normal[cares[i].idxVertices[j]]++;
            Index++;
        }
    }

    vertexsTextura[Index] = vec2(u,v);
    capsa= this->calculCapsa3D();

    //vec3 l = c.pmin +vec3(c.a,c.h,c.p)/2;
    //vec3 uni;
    for(unsigned int i=0; i<Index; i++){
        //uni = normalize(vec3(points[i][0]-l[0],points[i][1]-l[1],points[i][2]-l[2]));
        normals[i] = normals_v[ids_points[i]]/cont_normal[ids_points[i]];
        //u = 0.5 + atan2(uni[2],uni[0] )/ (2*M_PI);
        //v = 0.5 - asin(uni[1])/ M_PI;
        //vertexsTextura[i] = vec2(u,v);
    }
}




/**
 * @brief Object::toGPUTexture
 * @param pr
 */
void Object::toGPUTexture(QGLShaderProgram *pr) {
    program = pr;
// TO DO: Cal implementar en la fase 2 de la practica 2
// S'ha d'activar la textura i es passa a la GPU
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(point4)*Index + sizeof(point4)*Index, NULL, GL_STATIC_DRAW ); //+ sizeof(point2)*Index
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_TEXTURE_2D);
    GLuint bump_id;
    bump_id =program->uniformLocation(QString("bump"));
    glUniform1i(bump_id,0);
    bump = false;
    GLuint cp = program->uniformLocation(QString("cp"));
    vec4 aux = vec4(capsa.pmin +vec3(capsa.a,capsa.h,capsa.p)/2,1.0);
    glUniform4fv(cp, 1,aux);
}


/**
 * Pintat en la GPU.
 * @brief Object::drawTexture
 */
void Object::drawTexture(){

    // TO DO: Cal implementar en la fase 2 de la practica 2
    // S'ha d'activar la textura i es passa a la GPU

    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(point4)*Index, &points[0] );
    glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*Index, sizeof(point4)*Index, &normals[0] );
    //glBufferSubData( GL_ARRAY_BUFFER, sizeof(point4)*Index+sizeof(point4)*Index, sizeof(point2)*Index, &vertexsTextura[0] );
    int vertexLocation = program->attributeLocation("vPosition");
    int normalLocation = program->attributeLocation("vNormal");
    //int textureLocation = program->attributeLocation("vTexture");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer("vPosition", GL_FLOAT, 0, 4);
    program->enableAttributeArray(normalLocation);
    program->setAttributeBuffer("vNormal", GL_FLOAT, sizeof(point4)*Index, 4);
    //program->enableAttributeArray(textureLocation);
    //program->setAttributeBuffer("vTexture", GL_FLOAT, sizeof(point4)*Index+sizeof(point4)*Index, 2);
/*
    program->bindAttributeLocation("vPosition", vertexLocation);
    program->bindAttributeLocation("vNormal", normalLocation);
    program->bindAttributeLocation("vTexture", textureLocation); */
    this->m->toGPU(program);
    texture->bind(0);
    program->setUniformValue("texMap", 0);
    if(bump){
        textureb->bind(1);
        program->setUniformValue("bumpMap", 1);
    }
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDrawArrays( GL_TRIANGLES, 0, Index );


}



/**
 * @brief Object::initTextura
 */
void Object::initTextura()
 {
    // TO DO: A implementar a la fase 2 de la practica 2
    // Cal inicialitzar la textura de l'objecte: veure l'exemple del CubGPUTextura
    qDebug() << "Initializing textures...";
    glActiveTexture(GL_TEXTURE0);
    texture = new QOpenGLTexture(QImage("://resources/textures/2k_earth_daymap.jpg"));
    texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    texture->setMagnificationFilter(QOpenGLTexture::Linear);
    textureb = new QOpenGLTexture(QImage("://resources/textures/earth3.png"));
    textureb->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    textureb->setMagnificationFilter(QOpenGLTexture::Linear);
    texture->bind(0);
    textureb->bind(1);

 }
void Object::initBump()
{
    bump = true;
    textureb->bind(1);
    GLuint bump_id;
    bump_id =program->uniformLocation(QString("bump"));
    glUniform1i(bump_id,1);
}



// Llegeix un fitxer .obj
//  Si el fitxer referencia fitxers de materials (.mtl), encara no es llegeixen
//  Tots els elements del fitxer es llegeixen com a un unic Object.
void Object::readObj(QString filename){

    FILE *fp = fopen(filename.toLocal8Bit(),"rb");
    if (!fp)
    {
        cout << "No puc obrir el fitxer " << endl;
    }
    else {

        while (true)
        {
            char *comment_ptr = ReadFile::fetch_line (fp);

            if (comment_ptr == (char *) -1)  /* end-of-file */
                break;

            /* did we get a comment? */
            if (comment_ptr) {
                //make_comment (comment_ptr);
                continue;
            }

            /* if we get here, the line was not a comment */
            int nwords = ReadFile::fetch_words();

            /* skip empty lines */
            if (nwords == 0)
                continue;

            char *first_word = ReadFile::words[0];

            if (!strcmp (first_word, "v"))
            {
                if (nwords < 4) {
                    fprintf (stderr, "Too few coordinates: '%s'", ReadFile::str_orig);
                    exit (-1);
                }

                QString sx(ReadFile::words[1]);
                QString sy(ReadFile::words[2]);
                QString sz(ReadFile::words[3]);
                double x = sx.toDouble();
                double y = sy.toDouble();
                double z = sz.toDouble();

                if (nwords == 5) {
                    QString sw(ReadFile::words[4]);
                    double w = sw.toDouble();
                    x/=w;
                    y/=w;
                    z/=w;
                }
                // S'afegeix el vertex a l'Object
                vertexs.push_back(point4(x, y, z, 1));

            }
            else if (!strcmp (first_word, "vn")) {

            }
            else if (!strcmp (first_word, "vt")) {
            }
            else if (!strcmp (first_word, "f")) {
                // S'afegeix la cara a l'Object
                // A modificar si es vol carregar mes de un Object
                construeix_cara (&ReadFile::words[1], nwords-1);
            }
            // added
            else if (!strcmp (first_word, "mtllib")) {
                //read_mtllib (&words[1], nwords-1, matlib, filename);
            }
            else if (!strcmp (first_word, "usemtl")) {
                //int size = strlen(words[1])-1;
                //while (size && (words[1][size]=='\n' || words[1][size]=='\r') ) words[1][size--]=0;
                //currentMaterial = matlib.index(words[1]);
            }
            // fadded
            else {
                //fprintf (stderr, "Do not recognize: '%s'\n", str_orig);
            }
        }
    }
    // Calcul de les normals a cada cara
    for(unsigned int i=0; i<cares.size(); i++){
        cares[i].calculaNormal(vertexs);
    }
}

Capsa3D Object::calculCapsa3D()
{
    vec3    pmin, pmax;
    int     i;
    Capsa3D capsa;

    pmin.x = points[0].x;
    pmin.y = points[0].y;
    pmin.z = points[0].z;
    pmax = pmin;
    for(i=1; i<Index; i++) {
        if(points[i].x <pmin[0])
            pmin[0] = points[i].x;
        if(points[i].y <pmin[1])
            pmin[1] = points[i].y;
        if(points[i].z <pmin[2])
            pmin[2] = points[i].z;

        if(points[i].x >pmax[0])
            pmax[0] = points[i].x;
        if(points[i].y >pmax[1])
            pmax[1] = points[i].y;
        if(points[i].z >pmax[2])
            pmax[2] = points[i].z;
    }
    capsa.pmin = pmin;
    capsa.a = pmax[0]-pmin[0];
    capsa.h = pmax[1]-pmin[1];
    capsa.p = pmax[2]-pmin[2];
    return capsa;
}



void Object::construeix_cara ( char **words, int nwords) {
    Cara face;

    for (int i = 0; i < nwords; i++) {
        int vindex;
        int nindex;
        int tindex;

        if ((words[i][0]>='0')&&(words[i][0]<='9')) {
            ReadFile::get_indices (words[i], &vindex, &tindex, &nindex);

#if 0
            printf ("vtn: %d %d %d\n", vindex, tindex, nindex);
#endif

            /* store the vertex index */

            if (vindex > 0)       /* indices are from one, not zero */
                face.idxVertices.push_back(vindex - 1);
            else if (vindex < 0)  /* negative indices mean count backwards */
                face.idxVertices.push_back(this->vertexs.size() + vindex);
            else {
                fprintf (stderr, "Zero indices not allowed: '%s'\n", ReadFile::str_orig);
                exit (-1);
            }
        }
    }
    face.color = vec4(1.0, 0.0, 0.0, 1.0);
    this->cares.push_back(face);
}
