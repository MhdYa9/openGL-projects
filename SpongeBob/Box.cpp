#include "Box.h"
#include "Point.h"
#include <windows.h>		// Header File For Windows
#include <gl/gl.h>			// Header File For The OpenGL32 Library
#include <gl/glu.h>			// Header File For The GLu32 Library
#include <gl/glaux.h>		// Header File For The Glaux Library
#include <gl/glut.h>		// Header File For The Glut Library
#include <cmath>
#include "math3d.h"
#include "texture.h"
#define db double
#define gf GLfloat
#define pshm glPushMatrix()
#define ppm glPopMatrix()
#define shadow glColor3f(0.5,0.5,0.5);glMultMatrixf((gf*)shadowMat)
#define txt(s,t) glTexCoord2d(s,t)

void Box::drawOutside(db width, db length, db height, int texture[]) {
	glEnable(GL_TEXTURE_2D);
	pshm;
	pshm;
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	glNormal3f(0, -1, 0);
	txt(0, 0);
	glVertex3d(0, 0, 0);
	txt(1, 0);
	glVertex3d(width, 0, 0);
	txt(1, 1);
	glVertex3d(width, 0, length);
	txt(0, 1);
	glVertex3d(0, 0, length);
	glEnd();
	ppm;
	//the top
	pshm;
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	//glColor3ub(12, 213, 122);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);
	txt(0, 0);
	glVertex3d(0, height, 0);
	txt(1, 0);
	glVertex3d(0, height, length);
	txt(1, 1);
	glVertex3d(width, height, length);
	txt(0, 1);
	glVertex3d(width, height, 0);
	glEnd();
	ppm;

	//the back face
	pshm;
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	//glColor3ub(12, 213, 122);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, -1);
	txt(0, 0);
	glVertex3d(0, 0, 0);
	txt(1, 0);
	glVertex3d(0, height, 0);
	txt(1, 1);
	glVertex3d(width, height, 0);
	txt(0, 1);
	glVertex3d(width, 0, 0);
	glEnd();
	ppm;

	//the front face
	pshm;
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	//glColor3ub(12, 213, 122);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	txt(0, 0);
	glVertex3d(0, 0, length);
	txt(1, 0);
	glVertex3d(width, 0, length);
	txt(1, 1);
	glVertex3d(width, height, length);
	txt(0, 1);
	glVertex3d(0, height, length);
	glEnd();
	ppm;

	//the right side
	pshm;
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	//glColor3ub(12, 213, 122);
	glBegin(GL_QUADS);
	glNormal3f(-1, 0, 0);
	txt(0, 0);
	glVertex3d(0, 0, 0);
	txt(1, 0);
	glVertex3d(0, 0, length);
	txt(1, 1);
	glVertex3d(0, height, length);
	txt(0, 1);
	glVertex3d(0, height, 0);
	glEnd();
	ppm;

	//the left side
	pshm;
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glBegin(GL_QUADS);
	glNormal3f(0, 0, 1);
	txt(0, 0);
	glVertex3d(width, 0, 0);
	txt(1, 0);
	glVertex3d(width, height, 0);
	txt(1, 1);
	glVertex3d(width, height, length);
	txt(0, 1);
	glVertex3d(width, 0, length);
	glEnd();
	ppm;
	glDisable(GL_TEXTURE_2D);

	ppm;


}

void Box::drawInside(db width, db length, db height, int texture[]) {

	glEnable(GL_TEXTURE_2D);
	pshm;
	glFrontFace(GL_CCW);
	glCullFace(GL_FRONT);
	//base
	pshm;
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glBegin(GL_QUADS);
	//glNormal3f(0, 1, 0);
	txt(0, 0);
	glVertex3d(0, 0, 0);
	txt(1, 0);
	glVertex3d(width, 0, 0);
	txt(1, 1);
	glVertex3d(width, 0, length);
	txt(0, 1);
	glVertex3d(0, 0, length);
	glEnd();

	//the top
	pshm;
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glBegin(GL_QUADS);
	//glNormal3f(0, -1, 0);
	txt(0, 0);
	glVertex3d(0, height, 0);
	txt(1, 0);
	glVertex3d(0, height, length);
	txt(1, 1);
	glVertex3d(width, height, length);
	txt(0, 1);
	glVertex3d(width, height, 0);
	glEnd();

	//the back face
	pshm;
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glBegin(GL_QUADS);
	//glNormal3f(0, 0, 1);
	txt(0, 0);
	glVertex3d(0, 0, 0);
	txt(1, 0);
	glVertex3d(0, height, 0);
	txt(1, 1);
	glVertex3d(width, height, 0);
	txt(0, 1);
	glVertex3d(width, 0, 0);
	glEnd();
	pshm;

	//the front face
	pshm;
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	//glColor3ub(12, 213, 122);
	glBegin(GL_QUADS);
	//glNormal3f(0, 0, -1);
	txt(0, 0);
	glVertex3d(0, 0, length);
	txt(1, 0);
	glVertex3d(width, 0, length);
	txt(1, 1);
	glVertex3d(width, height, length);
	txt(0, 1);
	glVertex3d(0, height, length);
	glEnd();

	//the right side
	pshm;
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	//glColor3ub(12, 213, 122);
	glBegin(GL_QUADS);
	//glNormal3f(1, 0, 0);
	txt(0, 0);
	glVertex3d(0, 0, 0);
	txt(1, 0);
	glVertex3d(0, 0, length);
	txt(1, 1);
	glVertex3d(0, height, length);
	txt(0, 1);
	glVertex3d(0, height, 0);
	glEnd();
	ppm;

	//the left side
	pshm;
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	//glColor3ub(12, 213, 122);
	glBegin(GL_QUADS);
	//glNormal3f(0, 0, -1);
	txt(0, 0);
	glVertex3d(width, 0, 0);
	txt(1, 0);
	glVertex3d(width, height, 0);
	txt(1, 1);
	glVertex3d(width, height, length);
	txt(0, 1);
	glVertex3d(width, 0, length);
	glEnd();
	ppm;

	glDisable(GL_TEXTURE_2D);
	glFrontFace(GL_CW);

	ppm;

	glDisable(GL_TEXTURE_2D);
}

void Box::drawShadow(db width, db length, db height, M3DMatrix44f shadowMat) {


	ppm;
	//the top
	pshm;
	shadow;
	glBegin(GL_QUADS);
	//glNormal3f(0, 1, 0);
	glVertex3d(0, height, 0);
	glVertex3d(0, height, length);
	glVertex3d(width, height, length);
	glVertex3d(width, height, 0);
	glEnd();
	ppm;

	//back face
	pshm;
	shadow;
	glBegin(GL_QUADS);
	//glNormal3f(0, 0, -1);
	glVertex3d(0, 0, 0);
	glVertex3d(0, height, 0);
	glVertex3d(width, height, 0);
	glVertex3d(width, 0, 0);
	glEnd();
	ppm;

	//the front face
	pshm;
	shadow;
	glBegin(GL_QUADS);
	//glNormal3f(0, 0, 1);
	glVertex3d(0, 0, length);
	glVertex3d(width, 0, length);
	glVertex3d(width, height, length);
	glVertex3d(0, height, length);
	glEnd();
	ppm;

	//the right sie
	pshm;
	shadow;
	glBegin(GL_QUADS);
	//glNormal3f(-1, 0, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, length);
	glVertex3d(0, height, length);
	glVertex3d(0, height, 0);
	glEnd();
	ppm;

	//the left side
	pshm;
	shadow;
	glBegin(GL_QUADS);
	//glNormal3f(0, 0, 1);
	glVertex3d(width, 0, 0);
	glVertex3d(width, height, 0);
	glVertex3d(width, height, length);
	glVertex3d(width, 0, length);
	glEnd();
	ppm;

}

Box::Box(db width, db length, db height, int texture1[], int texture2[], M3DMatrix44f shadowMat, bool shad) {
	glEnable(GL_CULL_FACE);
	drawOutside(width, length, height, texture1);
	drawInside(width, length, height, texture2);
	if (shad) {
		pshm;
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		drawShadow(width, length, height, shadowMat);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		ppm;
	}
	glDisable(GL_CULL_FACE);
}
Box::Box() {
}