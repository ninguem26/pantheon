#include<stdlib.h>
#include<stdio.h>
#include<GL/glut.h>
#include<math.h>
#define PI 3.14159265

GLdouble phi = PI/2, theta = 0, radius = 30;
float fAspect;
double inc = 5*PI/180;
int style = 0;

void drawTriangle(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat z)    {
    glPushMatrix();
        glBegin(GL_TRIANGLES);
            glVertex3f(x1,y1,z);
            glVertex3f(x2,y2,z);
            glVertex3f(x3,y3,z);
        glEnd();
    glPopMatrix();
}

void drawPyramid(GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz){
    glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(sx, sy, sz);
        glBegin( GL_TRIANGLES );
            glVertex3f(0.0f, 1.0f, 0.0f );
            glVertex3f(-1.0f, -1.0f, 1.0f );
            glVertex3f(1.0f, -1.0f, 1.0f );

            glVertex3f(0.0f, 1.0f, 0.0f );
            glVertex3f(1.0f, -1.0f, 1.0f );
            glVertex3f(1.0f, -1.0f, -1.0f );

            glVertex3f(0.0f, 1.0f, 0.0f);
            glVertex3f(1.0f, -1.0f, -1.0f);
            glVertex3f(-1.0f, -1.0f, -1.0f);

            glVertex3f(0.0f, 1.0f, 0.0f );
            glVertex3f(-1.0f, -1.0f, -1.0f );
            glVertex3f(-1.0f, -1.0f, 1.0f );
        glEnd();
        glBegin( GL_QUADS );
            glVertex3f( -1.0f, -1.0f, 1.0f );
            glVertex3f( 1.0f, -1.0f, 1.0f );
            glVertex3f( 1.0f, -1.0f, -1.0f );
            glVertex3f( -1.0f, -1.0f, -1.0f );
        glEnd();
    glPopMatrix();
}

void drawLinhaVelas(GLfloat x, GLfloat y, GLfloat z, int n, double espacamento){
    for (int i = 0; i < n/2+1; i+= 1){
        drawPyramid(x-i*espacamento, y, z, 0.1, 0.5, 0.1);
        drawPyramid(x+i*espacamento, y, z, 0.1, 0.5, 0.1);
    }
}

void drawRect(GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz){
    glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(sx, sy, sz);
        glutSolidCube(1.0f);
    glPopMatrix();
}

void drawSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat radius){
    glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(radius, radius, radius);
        glRotatef(-90, 1, 0, 0);
        glutSolidSphere(1.0f, 20, 20);
    glPopMatrix();
}

void drawDome(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat bottom, GLfloat top){
    GLdouble eqnBottom[4] = {0.0, 0.0, 1.0, bottom};
    GLdouble eqnTop[4] = {0.0, 0.0, -1.0, top};

    glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(radius, radius, radius);
        glRotatef(-90, 1, 0, 0);

        glClipPlane (GL_CLIP_PLANE0, eqnBottom);
        glEnable (GL_CLIP_PLANE0);

        glClipPlane (GL_CLIP_PLANE1, eqnTop);
        glEnable (GL_CLIP_PLANE1);
        glutSolidSphere(1.0f, 20, 20);

        glDisable(GL_CLIP_PLANE0);
        glDisable(GL_CLIP_PLANE1);
    glPopMatrix();
}

void drawCylinder(GLfloat x, GLfloat y, GLfloat z, GLdouble radius, GLdouble height){
    GLUquadricObj *obj = gluNewQuadric();

    glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(270.0, 1.0, 0.0, 0.0);
        glScalef(radius, radius, height);

        gluQuadricDrawStyle(obj, GLU_FILL);
        gluCylinder(obj, 1, 1, 1, 6, 2);
	glPopMatrix();
}

void drawCylinderWithCut(GLfloat x, GLfloat y, GLfloat z, GLdouble radius, GLdouble height, GLfloat cut){
    GLUquadricObj *obj = gluNewQuadric();
    GLdouble eqn[4] = {0.0, 1.0, 0.0, cut};

    glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(270.0, 1.0, 0.0, 0.0);
        glScalef(radius, radius, height);

        glClipPlane (GL_CLIP_PLANE0, eqn);
        glEnable (GL_CLIP_PLANE0);

        gluQuadricDrawStyle(obj, GLU_FILL);
        gluCylinder(obj, 1, 1, 1, 20, 2);

        glDisable(GL_CLIP_PLANE0);
    glPopMatrix();
}

void drawLinha(float x, float y, float z, int n, float espacamento){
	float start = espacamento/2;
	float end = start + (n)/2 * espacamento;

	for (float i = start; i < end; i+= espacamento){
		drawCylinder(x-i*espacamento, y, z, 0.2, 3);
		drawCylinder(x+i*espacamento, y, z, 0.2, 3);
	}
}

void drawBench(GLfloat x, GLfloat y, GLfloat z, GLfloat scale, GLfloat rx, GLfloat ry, GLfloat rz){
    glTranslatef(x, y, z);
    glRotatef(rx, 1, 0, 0);
    glRotatef(ry, 0, 1, 0);
    glRotatef(rz, 0, 0, 1);
    glScalef(scale, scale, scale);

    //Assento
    drawRect(0,0,0,20,0.5,5);
    //Pernas
    drawRect(-9.3,-2,-1.8,1,4,1);
    drawRect(-9.3,-2,1.8,1,4,1);
    drawRect(9.3,-2,-1.8,1,4,1);
    drawRect(9.3,-2,1.8,1,4,1);
    drawRect(0,-2,-1.8,1,4,1);
    drawRect(0,-2,1.8,1,4,1);
    //Encosto
    drawRect(0,3,-2.1,20,3,0.5);
    //Suportes
    drawRect(0,2,-2.5,0.5,4,0.5);
    drawRect(-8.3,2,-2.5,0.5,4,0.5);
    drawRect(8.3,2,-2.5,0.5,4,0.5);
}

void glColor255(float r, float g, float b){
    glColor3f(r/255, g/255, b/255);
}

// Função callback chamada para fazer o desenho
void desenha(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Piso
    glColor255(89,87,84);
    drawRect(0,-4.5,5,11,0.3,20);
    
    //Rotunda
    glColor255(145,131,101);
	drawDome(0,0,0,4.5,0,0.97);
	drawDome(0,0,0,4.8,0,0.97);

    //Cilindros
    glColor255(191,169,135);
    drawCylinderWithCut(0,-4.5,0,4.5,4.5,0.9);
	drawCylinderWithCut(0,-4.5,0,4.8,4.5,0.9);

	//Pórtico
	glColor255(216,194,145);
	drawRect(4,-1.25,4.5,0.3,6.5,4);
	drawRect(-4,-1.25,4.5,0.3,6.5,4);
	drawRect(0,2,4.5,8.3,0.3,4);

    drawRect(3,-2.25,6.35,2,4.5,0.3);
	drawRect(-3,-2.25,6.35,2,4.5,0.3);
	drawRect(0,-0.125,6.35,8.3,4.25,0.3);

    //Colunas
	glColor255(145,131,101);
	drawLinha(0,-4.5,6.85,8,1);
	drawLinha(0,-4.5,7.85,8,1);
	drawLinha(0,-4.5,8.85,8,1);
	drawLinha(0,-4.5,9.85,8,1);

    //Teto
    drawRect(0,-1.5,8.5,8.3,0.3,4);
    glColor255(140,93,22);
    glPushMatrix();
        glTranslatef(-2,-0.3,8.5);
        glRotatef(30, 0, 0, 1);
        drawRect(0,0,0,4.8,0.3,3.9);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(2,-0.3,8.5);
        glRotatef(-30, 0, 0, 1);
        drawRect(0,0,0,4.8,0.3,3.9);
    glPopMatrix();

    glColor255(191,169,135);
    drawTriangle(-4.15,-1.5,4.15,-1.5,0,1,10.3);

    //Mesa
    glColor255(140,93,22);
    drawRect(0, -4.05, -3, 2, 1, 1);

    glColor255(216, 194, 104);
    drawLinhaVelas(0, -3.05, -3, 5, 0.4);

    //cruz
    glColor255(255,215,0);
    drawRect(0, -2.5, -3, 0.08, 0.5, 0.08);
    drawRect(0, -2.4, -3, 0.3, 0.08, 0.08);

	glutSwapBuffers();
}

void inicializa (void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void visao(void) {
  // Especifica sistema de coordenadas do modelo
  glMatrixMode(GL_MODELVIEW);
  // Inicializa sistema de coordenadas do modelo
  glLoadIdentity();
  // Especifica posição do observador e do alvo
  gluLookAt(radius*sin(theta)*sin(phi),radius*cos(phi),radius*cos(theta)*sin(phi), 0,0,0, 0,1,0);
}

// Função usada para especificar o volume de visualização
void visualizacaoParams(void) {
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(65, fAspect,0.5,500);
  visao();
}


void AlteraTamanhoJanela(GLsizei w, GLsizei h){
  if ( h == 0 ) h = 1;
  glViewport(0, 0, w, h);
  fAspect = (GLfloat)w/(GLfloat)h;
  visualizacaoParams();
}

// Callback para gerenciar eventos do teclado para teclas especiais (F1, PgDn, entre outras)
void SpecialKeys(int key, int x, int y) {
	glLoadIdentity();
        switch (key) {
            case GLUT_KEY_LEFT :
                theta += inc;
                break;
            case GLUT_KEY_RIGHT :
                theta -= inc;
                break;
            case GLUT_KEY_UP :
                if(phi + inc <= PI) phi += inc;
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
                if(style == 0){
                    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
                    style = 1;
                } else if(style == 1){
                    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
                    style = 0;
                }
                break;
        }
        phi = phi - ((int)(phi/(2*PI)));
		//phi = std::fmod(phi, 2*PI);
        gluLookAt(radius*sin(theta)*sin(phi),radius*cos(phi),radius*cos(theta)*sin(phi), 0,0,0, 0,1,0);
        glutPostRedisplay();
}

// Programa Principal
int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(350,300);
  glutCreateWindow("Visualizacao 3D");
  glutDisplayFunc(desenha);
  glutReshapeFunc(AlteraTamanhoJanela);
  glutSpecialFunc(SpecialKeys);
  inicializa();
  glutMainLoop();
}
