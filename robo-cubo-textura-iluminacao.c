//Guilherme Salgado Alves
// RA: 140083

#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define MAX_NO_TEXTURES 6

typedef struct BMPImagem
{
    int   width;
    int   height;
    char *data;
}BMPImage;

GLuint texture_id[MAX_NO_TEXTURES];
char* filenameArray[MAX_NO_TEXTURES] = {
        "earthTexture.bmp",
        "surface1.bmp",
        "surface2.bmp",
        "surface3.bmp",
        "surface4.bmp",
        "surface5.bmp"
};
GLUquadricObj *obj;
GLfloat angleX = 0.0f;
GLfloat angleY = 0.0f;

void getBitmapImageData( char *pFileName, BMPImage *pImage )
{
    FILE *pFile = NULL;
    unsigned short nNumPlanes;
    unsigned short nNumBPP;
    int i;
    if( (pFile = fopen(pFileName, "rb") ) == NULL )
        printf("ERROR: getBitmapImageData - %s not found.\n", pFileName);
    fseek( pFile, 18, SEEK_CUR );
    if( (i = fread(&pImage->width, 4, 1, pFile) ) != 1 )
        printf("ERROR: getBitmapImageData - Couldn't read width from %s.\n ", pFileName);
    if( (i = fread(&pImage->height, 4, 1, pFile) ) != 1 )
        printf("ERROR: getBitmapImageData - Couldn't read height from %s.\n ", pFileName);
    if( (fread(&nNumPlanes, 2, 1, pFile) ) != 1 )
        printf("ERROR: getBitmapImageData - Couldn't read plane count from %s.\n", pFileName);
    if( nNumPlanes != 1 )
        printf("ERROR: getBitmapImageData - Plane count from %s.\n ", pFileName);
    if( (i = fread(&nNumBPP, 2, 1, pFile)) != 1 )
        printf( "ERROR: getBitmapImageData - Couldn't read BPP from %s.\n ", pFileName);
    if( nNumBPP != 24 )
        printf("ERROR: getBitmapImageData - BPP from %s.\n ", pFileName);
    fseek( pFile, 24, SEEK_CUR );
    int nTotalImagesize = (pImage->width * pImage->height) * 3;

    pImage->data = (char*) malloc( nTotalImagesize );

    if( (i = fread(pImage->data, nTotalImagesize, 1, pFile) ) != 1 )
        printf("ERROR: getBitmapImageData - Couldn't read image data from %s.\n ", pFileName);
    char charTemp;
    for( i = 0; i < nTotalImagesize; i += 3 )
    {
        charTemp = pImage->data[i];
        pImage->data[i] = pImage->data[i+2];
        pImage->data[i+2] = charTemp;
    }
}

void CarregaTexturas()
{
    BMPImage textura;
    glGenTextures(MAX_NO_TEXTURES, texture_id); 
    int i;
    for ( i=0; i<MAX_NO_TEXTURES; i++ ) {
        getBitmapImageData( filenameArray[i], &textura);
        glBindTexture(GL_TEXTURE_2D, texture_id[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, textura.width, textura.height, 0, GL_RGB, GL_UNSIGNED_BYTE, textura.data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    }
}

void initTexture (void)
{
    glEnable ( GL_TEXTURE_2D );
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    CarregaTexturas();
}

void lightning(){
    GLfloat light0_pos[] = {2.0f, 2.0f, 2.0f, 1.0f};
    GLfloat white[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat black[] = {0.0f, 0.0f, 0.0f, 1.0f};

    glLightfv(GL_LIGHT0,GL_POSITION,light0_pos);
    glLightfv(GL_LIGHT0,GL_AMBIENT,black);
    glLightfv(GL_LIGHT0,GL_DIFFUSE,white);
    glLightfv(GL_LIGHT0,GL_SPECULAR,white);


    GLfloat light1_pos[] = {-2.0f, 0.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT1,GL_POSITION,light1_pos);
    glLightfv(GL_LIGHT1,GL_DIFFUSE, white);
    glLightfv(GL_LIGHT1,GL_SPECULAR, white);
    GLfloat direction[] = {1.0f, 0.0f, 0.0f};
    glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION,direction);       //vetor direção
    glLightf(GL_LIGHT1,GL_SPOT_CUTOFF,45.0f);               //espalhamento angular
    glLightf(GL_LIGHT1,GL_SPOT_EXPONENT,0.1f);              //atenuação angular


    //Desativação de uma fonte de luz, vindo apenas da esquerda
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //glEnable(GL_LIGHT1);
}

void init(void)
{  
    glEnable ( GL_COLOR_MATERIAL );
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
    glEnable(GL_DEPTH_TEST); 
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glMatrixMode(GL_MODELVIEW); 
    glLoadIdentity(); 
    gluLookAt(4.0, 2.0, 1.0,   
              0.0, 0.0, 0.0,   
              0.0, 1.0, 0.0);  
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
    gluPerspective(45.0, 1.0, 2.0, 8.0); 
    glViewport(0, 0, 500, 500);
    lightning();
}

void cubo(){
     float size = 1.0;
     GLfloat n[6][3] =
     {
        {-1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {1.0, 0.0, 0.0},
        {0.0, -1.0, 0.0},
        {0.0, 0.0, 1.0},
        {0.0, 0.0, -1.0}
     };
     GLint faces[6][4] =
     {
        {0, 1, 2, 3},
        {3, 2, 6, 7},
        {7, 6, 5, 4},
        {4, 5, 1, 0},
        {5, 6, 2, 1},
        {7, 4, 0, 3}
     };
     GLfloat v[8][3];
     GLint i;

     v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
     v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
     v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
     v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
     v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
     v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

     for (i = 5; i >= 0; i--) {
         glBindTexture(GL_TEXTURE_2D, texture_id[i]);
         glBegin(GL_QUADS);
            glNormal3fv(&n[i][0]);
            glTexCoord2f(0.0f,0.0f);
            glVertex3fv(&v[faces[i][0]][0]);
            glTexCoord2f(1.0f,0.0f);
            glVertex3fv(&v[faces[i][1]][0]);
            glTexCoord2f(1.0f,1.0f);
            glVertex3fv(&v[faces[i][2]][0]);
            glTexCoord2f(0.0f,1.0f);
            glVertex3fv(&v[faces[i][3]][0]);
         glEnd();
     }
}

void cabeca() {
  //glPushMatrix();
  glTranslatef(0.0, 0.5, 0.0);
  glScalef(0.8, 1.0, 0.8);
  //glRotatef(thetaX, 1.0, 0.0, 0.0);
  //glRotatef(thetaY, 0.0, 1.0, 0.0);
  cubo();
  //glPopMatrix();
}

void tronco() {
  glScalef(1.2, 1.5, 1.0);
  glTranslatef(0.0, -0.2, 0.0);
  glRotatef(angleX, 1.0, 0.0, 0.0);
  glRotatef(angleY, 0.0, 1.0, 0.0);
  cubo();
}

void bracodir() {
  glTranslatef(0.7, 0.1, 0.0);
  glScalef(2.0, 0.2, 0.1);
  //glRotatef(thetaX, 1.0, 0.0, 0.0);
  //glRotatef(thetaY, 0.0, 1.0, 0.0);
  cubo();
}

void bracoesq() {
  glTranslatef(-0.7, 0.1, 0.0);
  glScalef(2.0, 0.2, 0.1);
  //glRotatef(thetaX, 1.0, 0.0, 0.0);
  //glRotatef(thetaY, 0.0, 1.0, 0.0);
  cubo();
}


void pernadir() {
  //glPushMatrix();
  glTranslatef(0.25, -0.8, 0.0);
  glScalef(0.24, 0.5, 0.2);
  //glRotatef(thetaX, 1.0, 0.0, 0.0);
  //glRotatef(thetaY, 0.0, 1.0, 0.0);
  cubo();
  //glPopMatrix();
}

void pernaesq() {
  //glPushMatrix();
  glTranslatef(-0.24, -0.8, 0.2);
  glScalef(0.25, 0.5, 0.0);
  //glRotatef(thetaX, 1.0, 0.0, 0.0);
  //glRotatef(thetaY, 0.0, 1.0, 0.0);
  cubo();
  //glPopMatrix();
}

void displayFunc() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        glMatrixMode(GL_MODELVIEW); 
  
        glPushMatrix();
        tronco();
  
        glPushMatrix();
        bracodir();
        glPopMatrix();

        glPushMatrix();
        bracoesq();
        glPopMatrix();
  
        glPushMatrix();
        cabeca();
        glPopMatrix();

        glPushMatrix();
        pernadir();
        glPopMatrix();
  
        glPushMatrix();
        pernaesq();
        glPopMatrix();

        glPopMatrix();
        glFlush();
}

void rotacoes(int tecla, int x, int y) {
  switch (tecla) {
  case GLUT_KEY_RIGHT:
    angleY += 2;
    break;
  case GLUT_KEY_LEFT:
    angleY -= 2;
    break;
  case GLUT_KEY_UP:
    angleX += 2;
    break;
  case GLUT_KEY_DOWN:
    angleX -= 2;
    break;
  default:
    break;
  }
  glutPostRedisplay();
}

int main(int argc, char *argv[]){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(50,50);
    glutInitWindowSize(500,500);
    glutCreateWindow("RoboCuboTexturaIluminacao");
    glutDisplayFunc(displayFunc);
    glutSpecialFunc(rotacoes);
    init();
    initTexture();
    glutMainLoop();
    return 0;
}