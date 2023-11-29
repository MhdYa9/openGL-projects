#include "planet.h"
#include "color.h"
#include <windows.h>		// Header File For Windows
#include <gl.h>			// Header File For The OpenGL32 Library
#include <glu.h>			// Header File For The GLu32 Library
#include <glaux.h>		// Header File For The Glaux Library
#include <cmath>
#define db double
const double pi = 3.14159265358979323846;



planet::planet(db radius, color& c, bool redflag , bool greenflag , bool blueflag) {


	glBegin(GL_TRIANGLE_FAN);
	db r = 1, g = 1, b = 1;
	for (db phi = 0; phi <= pi; phi += 0.02) {
		for (db theta = 0; theta <= 2 * pi;theta += 0.02) {
			if (redflag) r = sin(phi);
			if (greenflag) g = sin(phi);
			if (blueflag) b = sin(phi);
			glColor3ub(c.red * r, c.green * g, c.blue  * b);
			db y = radius * cos(phi);
			db x = radius * sin(phi) * sin(theta);
			db z = radius * sin(phi) * cos(theta);
			//glVertex3d(0, 0, 0);
			glVertex3d(x, y, z);
		}
	}
	glEnd();

}