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
    glPushMatrix();
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
    glPopMatrix();
}
