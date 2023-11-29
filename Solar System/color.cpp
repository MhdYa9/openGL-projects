#include "color.h"
#include <windows.h>		// Header File For Windows
#include <gl.h>			// Header File For The OpenGL32 Library
#include <glu.h>			// Header File For The GLu32 Library
#include <glaux.h>		// Header File For The Glaux Library
#include <cmath>
#define db double
const double pi = 3.14159265358979323846;


color::color(db red, db green, db blue) {
	this->red = red;
	this->green = green;
	this->blue = blue;
}