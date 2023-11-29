#include "ring.h"
#include "color.h"
#include <windows.h>		// Header File For Windows
#include <gl.h>			// Header File For The OpenGL32 Library
#include <glu.h>			// Header File For The GLu32 Library
#include <glaux.h>		// Header File For The Glaux Library
#include <cmath>
#define db double
const double pi = 3.14159265358979323846;


ring::ring(db outter, db inner, color& c) {

	glColor4ub(c.red, c.green, c.blue,10);
	glBegin(GL_LINES);
	for (db angle = 0; angle <= pi * 2; angle += 0.01) {
		db x = inner * cos(angle);
		db z = inner * sin(angle);
		glVertex3d(x, 0, z);

		x = outter * cos(angle);
		z = outter * sin(angle);
		glVertex3d(x, 0, z);
	}
	glEnd();
}