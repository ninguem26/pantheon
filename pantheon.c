#include <GL/glut.h>
#include <math.h>
#include <stdbool.h>
#include <SFML/Graphics.hpp>

#define WIRE 0
#define FILL 1

#define CONCRETE 0
#define WOOD 1
#define GOLD 2
#define MARBLE 3
#define WALL 4
#define STATUE 5
#define WATER 6
#define INTERIOR 7
#define ROTUNDA 8

// Camera
GLdouble phi = M_PI / 2, theta = 0, radius = 15;

GLfloat fAspect;

double inc = 5 * M_PI / 180;

int style = 0;    // Wireframe or solid

GLdouble doorAngle = 0;     // Door angle
bool openingDoor = false;    // Door opening flag

GLuint texture_handle[10];

void loadTexture(GLuint texture, const char* filename)
{
    sf::Image img;
    img.loadFromFile(filename);

   glBindTexture(GL_TEXTURE_2D, texture);

   glTexImage2D(
       GL_TEXTURE_2D, 0, GL_RGBA,
       img.getSize().x, img.getSize().y,
       0,
       GL_RGBA, GL_UNSIGNED_BYTE, img.getPixelsPtr()
   );

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void rgb(float r, float g, float b){
  //glColor3f(r/255, g/255, b/255);
}

void drawTriangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat z) {
  glBegin(GL_TRIANGLES);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(x1,y1,z);
    glTexCoord2f(1.0f, 0.0f); glVertex3f(x2,y2,z);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(x3,y3,z);
  glEnd();
}

void drawPyramid(GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz) {
  glPushMatrix();

    glTranslatef(x, y, z);
    glScalef(sx, sy, sz);

    glBegin(GL_TRIANGLES);
      glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 1, 0);
      glTexCoord2f(1.0f, 0.0f); glVertex3i(-1, -1, 1);
      glTexCoord2f(0.0f, 1.0f); glVertex3i(1, -1, 1);

      glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 1, 0);
      glTexCoord2f(1.0f, 0.0f); glVertex3i(1, -1, 1);
      glTexCoord2f(0.0f, 1.0f); glVertex3i(1, -1, -1);

      glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 1, 0);
      glTexCoord2f(1.0f, 0.0f); glVertex3i(1, -1, -1);
      glTexCoord2f(0.0f, 1.0f); glVertex3i(-1, -1, -1);

      glTexCoord2f(0.0f, 0.0f); glVertex3i(0, 1, 0);
      glTexCoord2f(1.0f, 0.0f); glVertex3i(-1, -1, -1);
      glTexCoord2f(0.0f, 1.0f); glVertex3i(-1, -1, 1);
    glEnd();

    glBegin(GL_QUADS);
      glTexCoord2f(1.0f, 0.0f); glVertex3i(-1, -1, 1);
      glTexCoord2f(1.0f, 1.0f); glVertex3i(1, -1, 1);
      glTexCoord2f(0.0f, 1.0f); glVertex3i(1, -1, -1);
      glTexCoord2f(0.0f, 0.0f); glVertex3i(-1, -1, -1);
    glEnd();
  glPopMatrix();
}

void drawPyramidLine(GLfloat x, GLfloat y, GLfloat z, int n, double spacing) {
  for (int i = 0; i < n / 2 + 1; i += 1){
    drawPyramid(x - i * spacing, y, z, 0.1, 0.5, 0.1);
    drawPyramid(x + i * spacing, y, z, 0.1, 0.5, 0.1);
  }
}

void drawRect(GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz) {
  glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(sx, sy, sz);
    //glutSolidCube(1);
    glBegin(GL_QUADS);
    // Frente
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
    // Trás
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f,  0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f,  0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    // Direita
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    // Esquerda
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
    // Cima
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, 0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, 0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, 0.5f, 0.5f);
    // Baixo
    glTexCoord2f(1.0f, 0.0f); glVertex3f(0.5f, -0.5f, 0.5f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-0.5f, -0.5f, -0.5f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-0.5f, -0.5f, 0.5f);
    glEnd();
  glPopMatrix();
}

void drawSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat radius) {
  GLUquadricObj *sphere = gluNewQuadric();
  glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(radius, radius, radius);
    glRotatef(-90, 1, 0, 0);

    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, true);
    gluQuadricNormals(sphere, GLU_SMOOTH);
    //glutSolidSphere(1, 20, 20);
    gluSphere(sphere,1.0,20,20);
  glPopMatrix();
}

void drawDome(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat bottom, GLfloat top) {
  GLdouble eqnBottom[4] = {0.0, 0.0, 1.0, bottom};
  GLdouble eqnTop[4] = {0.0, 0.0, -1.0, top};

  GLUquadricObj *sphere = gluNewQuadric();
  glPushMatrix();
    glTranslatef(x, y, z);
    glScalef(radius, radius, radius);
    glRotatef(-90, 1, 0, 0);

    glClipPlane(GL_CLIP_PLANE0, eqnBottom);
    glEnable(GL_CLIP_PLANE0);

    glClipPlane(GL_CLIP_PLANE1, eqnTop);
    glEnable(GL_CLIP_PLANE1);
    //glutSolidSphere(1, 30, 30);
    gluQuadricDrawStyle(sphere, GLU_FILL);
    gluQuadricTexture(sphere, true);
    gluQuadricNormals(sphere, GLU_SMOOTH);

    gluSphere(sphere,1.0,50,50);

    glDisable(GL_CLIP_PLANE0);
    glDisable(GL_CLIP_PLANE1);
  glPopMatrix();
}

void drawCylinder(GLfloat x, GLfloat y, GLfloat z, GLdouble radius, GLdouble height) {
    GLUquadricObj *obj = gluNewQuadric();
    glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(270.0, 1.0, 0.0, 0.0);
        glScalef(radius, radius, height);

        gluQuadricDrawStyle(obj, GLU_FILL);
        gluQuadricTexture(obj, true);
        gluCylinder(obj, 1, 1, 1, 6, 2);
  glPopMatrix();
}

void drawCylinderWithCut(GLfloat x, GLfloat y, GLfloat z, GLdouble radius, GLdouble height, GLfloat cut) {
  GLUquadricObj *obj = gluNewQuadric();
  GLdouble eqn[4] = {0.0, 1.0, 0.0, cut};

  glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(270.0, 1.0, 0.0, 0.0);
    glScalef(radius, radius, height);

    glClipPlane (GL_CLIP_PLANE0, eqn);
    glEnable (GL_CLIP_PLANE0);

    gluQuadricDrawStyle(obj, GLU_FILL);
    gluQuadricTexture(obj, true);
    gluCylinder(obj, 1, 1, 1, 30, 2);

    glDisable(GL_CLIP_PLANE0);
  glPopMatrix();
}

void drawDisk(GLfloat x, GLfloat y, GLfloat z, GLdouble innerRadius, GLdouble outerRadius) {
  GLUquadricObj *obj = gluNewQuadric();

  glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(270.0, 1.0, 0.0, 0.0);

    gluQuadricDrawStyle(obj, GLU_FILL);
    gluQuadricTexture(obj, true);
    gluCylinder(obj, outerRadius, innerRadius, 0, 6, 2);
  glPopMatrix();
}

void drawColumnLine(float x, float y, float z, int n, float spacing) {
  float start = spacing/2;
  float end = start + (n)/2 * spacing;

  for (float i = start; i < end; i+= spacing){
    drawCylinder(x - i * spacing, y, z, 0.2, 3);
    drawCylinder(x + i * spacing, y, z, 0.2, 3);
  }
}

void drawBench(GLfloat x, GLfloat y, GLfloat z, GLfloat scale, GLfloat rx, GLfloat ry, GLfloat rz) {
  glPushMatrix();
    glTranslatef(x, y, z);
    glRotatef(rx, 1, 0, 0);
    glRotatef(ry, 0, 1, 0);
    glRotatef(rz, 0, 0, 1);
    glScalef(scale, scale, scale);

    //Assento
    drawRect(0, 0, 0, 20, 0.5, 5);

    //Pernas
    drawRect(-9.3, -2, -1.8, 1,4, 1);
    drawRect(-9.3, -2, 1.8, 1,4, 1);
    drawRect(9.3, -2, -1.8, 1,4, 1);
    drawRect(9.3, -2, 1.8, 1, 4, 1);
    drawRect(0, -2, -1.8, 1, 4, 1);
    drawRect(0, -2, 1.8, 1, 4, 1);

    //Encosto
    drawRect(0, 3, -2.1, 20, 3, 0.5);

    //Suportes
    drawRect(0, 2, -2.5, 0.5, 4, 0.5);
    drawRect(-8.3, 2, -2.5, 0.5, 4, 0.5);
    drawRect(8.3, 2, -2.5, 0.5, 4, 0.5);
  glPopMatrix();
}

void drawCandelabro(GLfloat x, GLfloat y, GLfloat z, GLfloat scale, GLfloat angle){
    glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(scale, scale, scale);
        glRotatef(angle, 0, 1, 0);

        glBindTexture(GL_TEXTURE_2D, texture_handle[GOLD]);
        drawCylinder(0,0,0,0.2,0.2);
        drawDisk(0,0.2,0,0,0.2);
        drawCylinder(0,0.2,0,0.05,1);
        drawRect(0,0.8,0,0.7,0.06,0.06);
        drawRect(0,0.4,0,0.9,0.06,0.06);
        drawDisk(0.4,0.43,0,0,0.1);
        drawDisk(-0.4,0.43,0,0,0.1);
        drawDisk(0.3,0.83,0,0,0.1);
        drawDisk(-0.3,0.83,0,0,0.1);
        drawDisk(0,1.2,0,0,0.1);
        glBindTexture(GL_TEXTURE_2D, texture_handle[MARBLE]);
        drawRect(0.3,0.93,0,0.05,0.2,0.05);
        drawRect(-0.3,0.93,0,0.05,0.2,0.05);
        drawRect(0.4,0.53,0,0.05,0.2,0.05);
        drawRect(-0.4,0.53,0,0.05,0.2,0.05);
        drawRect(0,1.3,0,0.05,0.2,0.05);
    glPopMatrix();
}

void drawDoor(){
  glPushMatrix();
    glRotatef(doorAngle, 0, 1, 0);
    glTranslatef(2, 0, 0);
    glScalef(4, 2.25, 0.15);
    //glutSolidCube(1);
    drawRect(0,0,0,1,1,1);
  glPopMatrix();
}

void drawOratorio(){
  glBindTexture(GL_TEXTURE_2D, texture_handle[MARBLE]);
  drawRect(0, -4.3, 0, 1.4, 0.4, 0.7);

  drawCylinder(0.6, -4.3, 0, .1, 2);
  drawCylinder(-0.6, -4.3, 0, .1, 2);

  //Telhado
  drawRect(0, -2.3, 0, 1.4, 0.3, 0.5);

  glPushMatrix();
    glTranslatef(0, -1.9, 0);
    glScalef(.2, .2, .3);

    glPushMatrix();
      glTranslatef(-2, -0.3, 0);
      glRotatef(30, 0, 0, 1);
      drawRect(0, 0, 0, 4.8, 0.3, 3.9);
    glPopMatrix();

    glPushMatrix();
      glTranslatef(2, -0.3, 0);
      glRotatef(-30, 0, 0, 1);
      drawRect(0, 0, 0, 4.8, 0.3, 3.9);
    glPopMatrix();

  drawTriangle(-4.15, -1.5, 4.15, -1.5, 0, 1, 1.4);

  glPopMatrix();

  glBindTexture(GL_TEXTURE_2D, texture_handle[STATUE]);
  drawRect(0, -3.3, 0, 0.7, 1.4, 0);
}

// Função callback chamada para gerenciar eventos do mouse
void mouseFunc(int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON)
         if (state == GLUT_DOWN)
          openingDoor = !openingDoor;
    glutPostRedisplay();
}

// Função callback chamada para fazer o desenho
void desenha(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //Piso
  glBindTexture(GL_TEXTURE_2D, texture_handle[CONCRETE]);
  drawRect(0, -4.5, 7, 11, 0.3, 24);

  //Rotunda
  glBindTexture(GL_TEXTURE_2D, texture_handle[ROTUNDA]);
  drawDome(0, 0, 0, 4.5, 0,0.97);
  glBindTexture(GL_TEXTURE_2D, texture_handle[WALL]);
  drawDome(0, 0, 0, 4.8, 0, 0.97);

  //Cilindros

  glBindTexture(GL_TEXTURE_2D, texture_handle[INTERIOR]);
  drawCylinderWithCut(0, -4.5, 0, 4.5, 4.5, 0.85);
  glBindTexture(GL_TEXTURE_2D, texture_handle[WALL]);
  drawCylinderWithCut(0, -4.5, 0, 4.8, 4.5, 0.85);

  //Portico
  drawRect(4, -1.25, 4.5, 0.3, 6.3, 4);
  drawRect(-4, -1.25, 4.5, 0.3, 6.3, 4);
  drawRect(0, 2.05, 4.5, 8.3, 0.3, 4);

  drawRect(2.925, -3.3, 6.35, 1.85, 2.2, 0.3);
  drawRect(-2.925, -3.3, 6.35, 1.85, 2.2, 0.3);
  drawRect(0, -0.15, 6.35, 7.7, 4.1, 0.3);

  //Colunas
  glBindTexture(GL_TEXTURE_2D, texture_handle[MARBLE]);
  drawColumnLine(0, -4.5, 6.85, 8, 1);
  drawColumnLine(0, -4.5, 7.85, 8, 1);
  drawColumnLine(0, -4.5, 8.85, 8, 1);
  drawColumnLine(0, -4.5, 9.85, 8,1);

  //Telhado
  glBindTexture(GL_TEXTURE_2D, texture_handle[MARBLE]);
  drawRect(0, -1.5, 8.5, 8.3, 0.3, 4);

  glPushMatrix();
    glTranslatef(-2, -0.3, 8.5);
    glRotatef(30, 0, 0, 1);
    drawRect(0, 0, 0, 4.8, 0.3, 3.9);
  glPopMatrix();

  glPushMatrix();
    glTranslatef(2, -0.3, 8.5);
    glRotatef(-30, 0, 0, 1);
    drawRect(0, 0, 0, 4.8, 0.3, 3.9);
  glPopMatrix();

  glBindTexture(GL_TEXTURE_2D, texture_handle[MARBLE]);
  drawTriangle(-4.15, -1.5, 4.15, -1.5, 0, 1, 10.3);

  //Mesa
  glBindTexture(GL_TEXTURE_2D, texture_handle[WOOD]);
  drawRect(0, -4.05, -3, 2, 1, 1);

  glBindTexture(GL_TEXTURE_2D, texture_handle[GOLD]);
  drawPyramidLine(0, -3.05, -3, 5, 0.4);

  //Cruz da mesa
  glBindTexture(GL_TEXTURE_2D, texture_handle[WOOD]);
  drawRect(0, -2.5, -3, 0.08, 0.5, 0.08);
  drawRect(0, -2.4, -3, 0.3, 0.08, 0.08);

  //Cadeiras
  glBindTexture(GL_TEXTURE_2D, texture_handle[WOOD]);
  drawBench(-2, -4.1, -1.5, 0.05, 0, 180, 0);
  drawBench(-0.8, -4.1, -1.5, 0.05, 0, 180, 0);
  drawBench(2, -4.1, -1.5, 0.05, 0, 180, 0);
  drawBench(0.8, -4.1, -1.5, 0.05, 0, 180, 0);
  drawBench(-2, -4.1, -1, 0.05, 0, 180, 0);
  drawBench(-0.8, -4.1, -1, 0.05, 0, 180, 0);
  drawBench(2, -4.1, -1, 0.05, 0, 180, 0);
  drawBench(0.8, -4.1, -1, 0.05, 0, 180, 0);
  drawBench(-2, -4.1, -0.5, 0.05, 0, 180,0);
  drawBench(-0.8, -4.1, -0.5, 0.05, 0, 180,0);
  drawBench(2, -4.1, -0.5, 0.05, 0, 180, 0);
  drawBench(0.8, -4.1, -0.5, 0.05, 0, 180, 0);

  //Oratorio
  glPushMatrix();
    glTranslatef(-2.3, 0, -3);
    glRotatef(45, 0, 1, 0);
    drawOratorio();
  glPopMatrix();

  glPushMatrix();
    glTranslatef(2.3, 0, -3);
    glRotatef(-45, 0, 1, 0);
    drawOratorio();
  glPopMatrix();

  //Obelisco
  glBindTexture(GL_TEXTURE_2D, texture_handle[MARBLE]);
  drawCylinder(0, -4.5, 15, 1.5, 0.5);
  drawCylinder(0, -4.5, 15, 1.3, 0.5);
  drawDisk(0, -4, 15, 1.3, 1.5);

  drawRect(0, -4., 15, 0.75, 0.75, 0.75);

  drawPyramid(0, -2, 15, 0.2, 2, 0.2);

  glBindTexture(GL_TEXTURE_2D, texture_handle[WATER]);
  drawDisk(0, -4.25, 15, 0.2, 1.3);

  //Porta
  glBindTexture(GL_TEXTURE_2D, texture_handle[WOOD]);
  glPushMatrix();
    glTranslatef(-2, -3.375, 6.35);
    drawDoor();
  glPopMatrix();

  //Candelabros

  drawCandelabro(4,-4.3,0,1,90);
  drawCandelabro(-4,-4.3,0,1,90);
  drawCandelabro(3,-4.3,5.3,1,90);
  drawCandelabro(-3,-4.3,5.3,1,90);

  glutSwapBuffers();
}

void inicializa (void) {
  GLfloat luzAmbiente[4] = {0.3, 0.3, 0.3, 1.0};
  GLfloat luzDifusa[4]={0.4, 0.4, 0.4, 1.0};
  GLfloat posicaoLuz[4]={0.0, 20.0, 30.0, 1.0};

  glShadeModel(GL_SMOOTH);

  // Ativa o uso da luz ambiente
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);

  // Define os parâmetros da luz de número 0
  glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa );
  glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz );

  // Habilita a definição da cor do material a partir da cor corrente
  glEnable(GL_COLOR_MATERIAL);
  //Habilita o uso de iluminação
  glEnable(GL_LIGHTING);
  // Habilita a luz de número 0
  glEnable(GL_LIGHT0);
  // Habilita o depth-buffering

  glClearColor(0, 0, 0, 1);

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_TEXTURE_2D);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  glGenTextures(9, texture_handle);

  loadTexture(texture_handle[0], "img/concrete.jpg");
  loadTexture(texture_handle[1], "img/wood.jpg");
  loadTexture(texture_handle[2], "img/gold.jpg");
  loadTexture(texture_handle[3], "img/marble.jpg");
  loadTexture(texture_handle[4], "img/wall.jpg");
  loadTexture(texture_handle[5], "img/statue.png");
  loadTexture(texture_handle[6], "img/agua.jpg");
  loadTexture(texture_handle[7], "img/interior.png");
  loadTexture(texture_handle[8], "img/rotunda.png");
  glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void visao(void) {
  // Especifica sistema de coordenadas do modelo
  glMatrixMode(GL_MODELVIEW);
  // Inicializa sistema de coordenadas do modelo
  glLoadIdentity();
  // Especifica posição do observador e do alvo
  gluLookAt(radius*sin(theta)*sin(phi),radius*cos(phi),radius*cos(theta)*sin(phi), 0, 0, 0, 0, 1, 0);
}

// Função usada para especificar o volume de visualização
void visualizacaoParams(void) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(65, fAspect, 0.5, 500);
  visao();
}

void reshapeFunc(GLsizei w, GLsizei h) {
  if (h == 0)
    h = 1;
  glViewport(0, 0, w, h);
  fAspect = (GLfloat) w / (GLfloat) h;
  visualizacaoParams();
}

// Callback para gerenciar eventos do teclado para teclas especiais (F1, PgDn, entre outras)
void specialFunc(int key, int x, int y) {
  glLoadIdentity();

  switch (key) {
    case GLUT_KEY_LEFT :
      theta += inc;
    break;
    case GLUT_KEY_RIGHT :
      theta -= inc;
    break;
    case GLUT_KEY_UP :
      if(phi + inc <= M_PI) phi += inc;
    break;
    case GLUT_KEY_DOWN :
      if(phi - inc >= 0) phi -= inc;
    break;
    case GLUT_KEY_PAGE_UP :
      if(radius - 1 > 2) radius -= 1;
    break;
    case GLUT_KEY_PAGE_DOWN :
      radius += 1;
    break;
    case GLUT_KEY_F1 :
      if(style == WIRE) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        style = FILL;
      } else if(style == FILL) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        style = WIRE;
      }
    break;
  }

  phi = phi - ((int) (phi / (2 * M_PI)));
  gluLookAt(radius * sin(theta) * sin(phi), radius * cos(phi), radius * cos(theta) * sin(phi), 0, 0, 0, 0, 1, 0);
  glutPostRedisplay();
}

static void timer(int value){
  if (openingDoor){
    if (doorAngle < 90)
      doorAngle += 5;
  } else if (doorAngle > 0)
    doorAngle -= 5;

  glutPostRedisplay();
  glutTimerFunc(33, timer, 0);
}

// Programa Principal
int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(350,300);
  glutCreateWindow("Pantheon");
  glutDisplayFunc(desenha);
  glutReshapeFunc(reshapeFunc);
  glutSpecialFunc(specialFunc);
  glutMouseFunc(mouseFunc);
  inicializa();
  timer(0);
  glutMainLoop();
}
