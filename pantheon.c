#include<stdlib.h>
#include<stdio.h>
#include<GL/glut.h>
#include<math.h>
#define PI 3.14159265

GLdouble phi = PI/2, theta = 0, radius = 30;
float fAspect;
double inc = 5*PI/180;
char style = 's';

void drawTriangle(){
    glBegin(GL_TRIANGLES);
        glVertex3f(-4.15,-1.5,10.3);
        glVertex3f(4.15,-1.5,10.3);
        glVertex3f(0,1,10.3);
    glEnd();
}

void drawRect(char drawMode, GLfloat x, GLfloat y, GLfloat z, GLfloat sx, GLfloat sy, GLfloat sz){
    glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(sx, sy, sz);
        if(drawMode == 'w') {
            glutWireCube(1.0f);
        } else if(drawMode == 's') {
            glutSolidCube(1.0f);
        }
    glPopMatrix();
}

void drawSphere(char drawMode, GLfloat x, GLfloat y, GLfloat z, GLfloat radius){
    glPushMatrix();
        glTranslatef(x, y, z);
        glScalef(radius, radius, radius);
        glRotatef(-90, 1, 0, 0);
        if(drawMode == 'w') {
            glutWireSphere(1.0f, 20, 20);
        } else if(drawMode == 's') {
            glutSolidSphere(1.0f, 20, 20);
        }
    glPopMatrix();
}

void drawDome(char drawMode, GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat bottom, GLfloat top){
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

        if(drawMode == 'w') {
            glutWireSphere(1.0f, 20, 20);
        } else if(drawMode == 's') {
            glutSolidSphere(1.0f, 20, 20);
        }

        glDisable(GL_CLIP_PLANE0);
        glDisable(GL_CLIP_PLANE1);
    glPopMatrix();
}

void drawCylinder(char drawMode, GLfloat x, GLfloat y, GLfloat z, GLdouble radius, GLdouble height){
    GLUquadricObj *obj = gluNewQuadric();

    glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(270.0, 1.0, 0.0, 0.0);
        glScalef(radius, radius, height);

        if(drawMode == 'w') {
            gluQuadricDrawStyle(obj, GLU_LINE);
        } else if(drawMode == 's') {
            gluQuadricDrawStyle(obj, GLU_FILL);
        }
        gluCylinder(obj, 1, 1, 1, 20, 2);
	glPopMatrix();
}

void drawCylinderWithCut(char drawMode, GLfloat x, GLfloat y, GLfloat z, GLdouble radius, GLdouble height, GLfloat cut){
    GLUquadricObj *obj = gluNewQuadric();
    GLdouble eqn[4] = {0.0, 1.0, 0.0, cut};

    glPushMatrix();
        glTranslatef(x, y, z);
        glRotatef(270.0, 1.0, 0.0, 0.0);
        glScalef(radius, radius, height);

        glClipPlane (GL_CLIP_PLANE0, eqn);
        glEnable (GL_CLIP_PLANE0);

        if(drawMode == 'w') {
            gluQuadricDrawStyle(obj, GLU_LINE);
        } else if(drawMode == 's') {
            gluQuadricDrawStyle(obj, GLU_FILL);
        }
        gluCylinder(obj, 1, 1, 1, 20, 2);

        glDisable(GL_CLIP_PLANE0);
    glPopMatrix();
}

void drawLinha(char drawMode, float x, float y, float z, int n, float espacamento){
	float start = espacamento/2;
	float end = start + (n)/2 * espacamento;

	for (float i = start; i < end; i+= espacamento){
		drawCylinder(drawMode, x-i*espacamento, y, z, 0.2, 3);
		drawCylinder(drawMode, x+i*espacamento, y, z, 0.2, 3);
	}
}

void glColor255(float r, float g, float b){
    glColor3f(r/255, g/255, b/255);
}

// Função callback chamada para fazer o desenho
void desenha(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //Piso
	glColor255(89,87,84);
	drawRect(style,0,-4.5,5,11,0.3,20);

	//Rotunda
    glColor255(145,131,101);
	drawDome(style,0,0,0,4.5,0,0.97);
	drawDome(style,0,0,0,4.8,0,0.97);

    //Cilindros
    glColor255(191,169,135);
    drawCylinderWithCut(style,0,-4.5,0,4.5,4.5,0.9);
	drawCylinderWithCut(style,0,-4.5,0,4.8,4.5,0.9);

	//Pórtico
	glColor255(216,194,145);
	drawRect(style,4,-1.25,4.5,0.3,6.5,4);
	drawRect(style,-4,-1.25,4.5,0.3,6.5,4);
	drawRect(style,0,2,4.5,8.3,0.3,4);

    drawRect(style,3,-2.25,6.35,2,4.5,0.3);
	drawRect(style,-3,-2.25,6.35,2,4.5,0.3);
	drawRect(style,0,-0.125,6.35,8.3,4.25,0.3);

    //Colunas
	glColor255(145,131,101);
	drawLinha(style,0,-4.5,6.85,8,1);
	drawLinha(style,0,-4.5,7.85,8,1);
	drawLinha(style,0,-4.5,8.85,8,1);
	drawLinha(style,0,-4.5,9.85,8,1);

    //Teto
    drawRect(style,0,-1.5,8.5,8.3,0.3,4);
    glColor255(140,93,22);
    glPushMatrix();
        glTranslatef(-2,-0.3,8.5);
        glRotatef(30, 0, 0, 1);
        drawRect(style,0,0,0,4.8,0.3,3.9);
    glPopMatrix();

    glPushMatrix();
        glTranslatef(2,-0.3,8.5);
        glRotatef(-30, 0, 0, 1);
        drawRect(style,0,0,0,4.8,0.3,3.9);
    glPopMatrix();

    glColor255(191,169,135);

    glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(-4.15,-1.5,10.3);
        glVertex3f(4.15,-1.5,10.3);
        glVertex3f(0,1,10.3);
    glEnd();

	glutSwapBuffers();
}

void inicializa (void) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
                radius += 5;
                break;
            case GLUT_KEY_F1 :
                if(style == 'w'){
                    style = 's';
                } else if(style == 's'){
                    style = 'w';
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
  glEnable(GL_DEPTH_TEST);
  glutMainLoop();
}
