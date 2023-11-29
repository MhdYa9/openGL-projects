#include "orbit.h"
#include "color.h"
#include <windows.h>		// Header File For Windows
#include <gl.h>			// Header File For The OpenGL32 Library
#include <glu.h>			// Header File For The GLu32 Library
#include <glaux.h>		// Header File For The Glaux Library
#include <cmath>
#define db double
const double pi = 3.14159265358979323846;

orbit::orbit(db radius,db dist) {
	
	glBegin(GL_POINTS);
	for (db angle = 0; angle <= pi * 2; angle += dist) {
		db x = radius*cos(angle);
		db z = radius * sin(angle);
		glVertex3d(x, 0, z);
	}
	glEnd();
}