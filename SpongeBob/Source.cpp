#include <windows.h>		// Header File For Windows
#include <gl.h>			// Header File For The OpenGL32 Library
#include <glu.h>			// Header File For The GLu32 Library
#include <glaux.h>		// Header File For The Glaux Library
#include <glut.h>		// Header File For The Glut Library
#include <cmath>
#include "math3d.h"
#include <texture.h>
#include "Camera.h"
#include "Point.h"
#include "Constraints.h"
#include "box.h"
#include "Cylinder.h"
#define db double
#define gf GLfloat
#define pshm glPushMatrix()
#define ppm glPopMatrix()
#define shadow 	glMultMatrixf((gf*)shadowMat); glColor3f(0.5,0.5,0.5);
#define txt(s,t) glTexCoord2d(s,t)
#define white glColor3f(1,1,1)
const double pi = 3.14159265358979323846;
#define PIdiv180 (pi/180.0)




HDC			hDC = NULL;		// Private GDI Device Context
HGLRC		hRC = NULL;		// Permanent Rendering Cntext
HWND		hWnd = NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active = TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen = FALSE;	// Fullscreen Flag Set To Fullscreen Mode By Default

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

GLsizei screenWidth, screenHeight;

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// Resize And Initialize The GL Window
{
	if (height == 0)										// Prevent A Divide By Zero By
	{
		height = 1;										// Making Height Equal One
	}

	glViewport(0, 0, width, height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 10000.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

//lights
gf LightPos[] = { 500,1000,500,0 };
gf LightAmb[] = { 1,1,1,0 };
gf LightDiff[] = { 5,5,5,0 };
gf LightSpec[] = { 1,1,1,0 };
gf MatAmb[] = { 1,1,1,0 };
gf MatDiff[] = { 5,5,5,0 };
gf MatSpec[] = { 1,1,1,0 };
gf MatShin[] = { 120 };


//db eyeX = 0, eyeY = 1, eyeZ = 0;
//db centerX = 0, centerY = 0, centerZ = -5;
//double angle_step = 0.1, move_step = 0.1, angle = 0;
//db f = 0, r = 0;
//void Camera(db n = 0.1)
//{
//	if (keys['D'])
//		eyeX += n;
//	if (keys['A'])
//		eyeX -= n;
//	if (keys['W'])
//		eyeY += n;
//	if (keys['S'])
//		if (eyeY > 1) eyeY -= n;
//	if (keys['Z'])
//		eyeZ += n;
//	if (keys['X'])
//	{
//		eyeZ -= n;
//	}
//
//	if (keys[VK_LEFT])
//		centerX += n;
//	if (keys[VK_RIGHT])
//		centerX -= n;
//	if (keys[VK_UP])
//		centerY += n;
//	if (keys[VK_DOWN])
//		centerY -= n;
//	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0, 1, 0);
//
//}
M3DMatrix44f shadowMat;
M3DMatrix44f shadowMat2;
M3DVector4f vPlaneEquation;
GLUquadric* q;



Camera* camera;
void drawSkyBox(const Point& position, const Constraints& constraints, int texture[]) {
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	float x = position.x, y = position.y, z = position.z;
	float width = constraints.width, height = constraints.height, length = constraints.length;
	// Center the Skybox around the given x,y,z position
	x = x - width / 2;
	y = y - height / 2;
	z = z - length / 2;
	glEnable(GL_TEXTURE_2D);

	// Draw Front side
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glEnd();

	// Draw Back side
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	// Draw Left side
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z);
	glEnd();

	// Draw Right side
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
	glEnd();

	// Draw Up side
	glBindTexture(GL_TEXTURE_2D, texture[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y + height, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x, y + height, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x, y + height, z);
	glEnd();

	// Draw Down side
	glBindTexture(GL_TEXTURE_2D, texture[5]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, 0x812F);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, 0x812F);
	glBegin(GL_QUADS);
	y += 120;
	glTexCoord2f(0.0f, 0.0f); glVertex3f(x, y, z);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(x, y, z + length);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y, z + length);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y, z);
	glEnd();

	glColor3f(1, 1, 1);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glDisable(GL_TEXTURE_2D);

}
int a, b, c, d, e, f, g, h, i, j, k, l, m, leg;
int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	Camera::cameraInit();

	a = LoadTexture((char*)"bikini_bottom_skybox/back.bmp");
	b = LoadTexture((char*)"bikini_bottom_skybox/side3.bmp");
	c = LoadTexture((char*)"bikini_bottom_skybox/side2.bmp");
	d = LoadTexture((char*)"bikini_bottom_skybox/side1.bmp");
	e = LoadTexture((char*)"bikini_bottom_skybox/top.bmp");
	f = LoadTexture((char*)"bikini_bottom_skybox/ground.bmp");
	j = LoadTexture((char*)"spongeBob/face.bmp");
	k = LoadTexture((char*)"spongeBob/back.bmp");
	l = LoadTexture((char*)"spongeBob/side1.bmp");
	m = LoadTexture((char*)"spongeBob/side2.bmp");
	i = LoadTexture((char*)"spongeBob/top.bmp");
	h = LoadTexture((char*)"spongeBob/base.bmp");
	leg = LoadTexture((char*)"spongeBob/legs.bmp");

	M3DVector3f points[3] = { {-10.0f,0,-10.0f},{-10.0f,0,10.0f},{10.0f,0,-10.0f} };
	m3dGetPlaneEquation(vPlaneEquation, points[0], points[1], points[2]);
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmb);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiff);
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpec);
	glEnable(GL_LIGHT1);
	glDisable(GL_LIGHTING);
	glMaterialfv(GL_FRONT, GL_AMBIENT, MatAmb);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, MatDiff);
	glMaterialfv(GL_FRONT, GL_SPECULAR, MatSpec);
	glMaterialfv(GL_FRONT, GL_SHININESS, MatShin);
	glEnable(GL_COLOR_MATERIAL);
	q = gluNewQuadric();
	glEnable(GL_CULL_FACE);


	return TRUE;										// Initialization Went OK
}

gf p0 = 0, p1 = 0, p2 = 0;

db pang = 0; db v =4;

int mx = 0, my = 0, isClicked = 0, isRClicked = 0;
void drawSpongeBob() {
	int textures[6];
	Box spongBob = Box();
	textures[0] = h;
	textures[1] = i;
	textures[2] = k;
	textures[3] = j;
	textures[4] = l;
	textures[5] = m;

	//spongeBob
	pshm;

	//glTranslated(0, -110, 20);


	if (keys[VK_NUMPAD4]) p0 += 0.1;
	if (keys[VK_NUMPAD6]) p0 -= 0.1;
	if (keys[VK_NUMPAD1]) p1 += 0.1;
	if (keys[VK_NUMPAD3]) p1 -= 0.1;
	if (keys['W'])
	{
		p2 += 0.1;

		if (pang == 20) v *= -1;
		if (pang == -20) v *= -1;
		pang += v;
	}
	if (keys['S'])
	{
		p2 -= 0.1;
		if (pang == 20) v *= -1;
		if (pang == -20) v *= -1;
		pang -= v;
	}
	glTranslated(p0, p1, p2);

	spongBob.drawOutside(8, 3, 10, textures);

	//shirt sleeve
	pshm;
	glTranslated(-0.2, 2.5, 1.5);
	glRotated(-90, 1, 0, 0);
	glRotated(45, 0, 1, 0);
	glColor3ub(240, 240, 250);
	Cylinder c1 = Cylinder(0.8, 0.1, 1);
	c1.draw();
	ppm;

	//arm;
	pshm;
	glTranslated(0, 2.3, 0);
	glRotated(90, 1, 0, 0);
	glRotated(-45, 0, 1, 0);
	pshm;
	glTranslated(-0.2, 1.4, 1.5);
	glColor3f(1, 1, 0);
	Cylinder arm2 = Cylinder(0.2, 0.2, 6);
	arm2.draw();
	pshm;
	glTranslated(0, 0, 3);
	gluSphere(q, 0.5, 36, 36);
	ppm;
	ppm;
	ppm;


	//shirt sleeve
	pshm;
	glTranslated(8.3, 2.5, 1.5);
	glRotated(-90, 1, 0, 0);
	glRotated(-45, 0, 1, 0);
	glColor3ub(240, 240, 250);
	Cylinder c2 = Cylinder(0.8, 0.1, 1);
	c2.draw();
	ppm;


	//arm;
	pshm;
	/*glTranslated(3, -4, 0);
	glRotated(90, 1, 0, 0);*/
	glTranslated(3, -4, 0);
	glRotated(45, 0, 0, 1);
	glRotated(90, 1, 0, 0);
	pshm;
	glTranslated(8.3, 1.4, 1);
	glColor3f(1, 1, 0);
	Cylinder arm1 = Cylinder(0.2, 0.2, 4);
	arm1.draw();
	pshm;
	glTranslated(0, 0, 2);
	gluSphere(q, 0.5, 36, 36);
	ppm;
	ppm;
	ppm;



	//whole leg
	pshm;
	//glTranslated(0, 0, 0);

	glRotated(pang, 1, 0, 0);
	//shorts leg
	pshm;
	pshm;
	white;
	glTranslated(7.3, -1, 1.2);
	glRotated(90, 1, 0, 0);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, leg);
	Cylinder leg1 = Cylinder(0.2, 0.2, 7.5);
	leg1.draw();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	ppm;
	glTranslated(7.3, 0, 1.2);
	glRotated(-90, 1, 0, 0);
	glColor3ub(214, 135, 43);
	Cylinder c3 = Cylinder(1, 0.1, 1);
	c3.draw();
	ppm;
	//shoes
	pshm;
	glTranslated(7.3, -5, 1.6);
	glRotated(180, 0, 1, 0);
	glColor3f(0, 0, 0);
	pshm;
	glTranslated(0, -0.3, -0.2);
	gluSphere(q, 0.6, 36, 36);
	ppm;
	pshm;
	glTranslated(0, 0, 0.6);
	gluSphere(q, 0.5, 36, 36);
	glTranslated(-0.3, -0.8, -0.3);
	Box b1 = Box();
	int x[] = { -1,-1,-1,-1,-1,-1 };
	glDisable(GL_CULL_FACE);
	b1.drawOutside(0.6, 0.6, 0.6, x);
	glEnable(GL_CULL_FACE);
	ppm;
	ppm;

	ppm; //for the whole leg


	//the whole leg
	pshm;
	glRotated(-pang, 1, 0, 0);
	//short leg
	pshm;
	pshm;
	white;
	glTranslated(0.6, -1, 1.2);
	glRotated(90, 1, 0, 0);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, leg);
	Cylinder leg2 = Cylinder(0.2, 0.2, 7.5);
	leg2.draw();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_CULL_FACE);
	ppm;
	glTranslated(0.6, 0, 1.2);
	glRotated(-90, 1, 0, 0);
	glColor3ub(214, 135, 43);
	Cylinder c4 = Cylinder(1, 0.1, 1);
	c4.draw();
	ppm;
	//shoes
	pshm;
	glTranslated(0.6, -5, 1.6);
	glRotated(180, 0, 1, 0);
	glColor3f(0, 0, 0);
	pshm;
	glTranslated(0, -0.3, -0.2);
	gluSphere(q, 0.6, 36, 36);
	ppm;
	pshm;
	glTranslated(0, 0, 0.6);
	glRotated(180, 0, 1, 0);
	gluSphere(q, 0.5, 36, 36);
	glTranslated(-0.3, -0.8, -0.3);
	glDisable(GL_CULL_FACE);
	Box b2 = Box();
	b2.drawOutside(0.6, 0.6, 0.6, x);
	glEnable(GL_CULL_FACE);
	ppm;
	ppm;
	ppm;

	ppm; //for the whole leg



	ppm;
}
GLfloat zoomFactor = 1.0f; // Adjust this value based on your zoom requirement
void updatePerspective() {
	// Assuming you have a perspective projection set up
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Adjust the field of view based on zoom factor
	GLdouble aspectRatio = screenWidth / static_cast<GLdouble>(screenHeight);
	GLdouble nearPlane = 0.1;
	GLdouble farPlane = 10000.0;
	GLdouble fovY = 45.0 / zoomFactor; // Adjust the field of view

	gluPerspective(fovY, aspectRatio, nearPlane, farPlane);
}


int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	camera = Camera::getInstance();

	camera->Render();
	camera->decodeKeyboard(keys, 0.1);
	camera->decodeMouse(mx,my,isClicked,isRClicked);
	//lights ans shadows
	int textures[6];
	textures[0] = a;
	textures[1] = b;
	textures[2] = c;
	textures[3] = d;
	textures[4] = e;
	textures[5] = f;

	/*if (keys[VK_NUMPAD4]) p0 += 0.1;
	if (keys[VK_NUMPAD6]) p0 -= 0.1;
	if (keys[VK_NUMPAD1]) p1 += 0.1;
	if (keys[VK_NUMPAD3]) p1 -= 0.1;
	if (keys[VK_NUMPAD8]) p2 += 0.1;
	if (keys[VK_NUMPAD2]) p2 -= 0.1;
	LightPos[0] = p0;
	LightPos[1] = p1;
	LightPos[2] = p2;
	glLightfv(GL_LIGHT1, GL_POSITION, LightPos);
	if (keys['O']) glEnable(GL_LIGHTING);
	if (keys['F']) glDisable(GL_LIGHTING);
	m3dMakePlanarShadowMatrix(shadowMat, vPlaneEquation, LightPos);
	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslated(LightPos[0], LightPos[1], LightPos[2]);
	gluSphere(q, 0.1, 32, 32);*/
	glPopMatrix();
	pshm;
	glDisable(GL_CULL_FACE);
	glTranslated(0, 10, 0);
	drawSkyBox(Point(0, 0, 0), Constraints(500, 300, 500), textures);
	glEnable(GL_CULL_FACE);
	ppm;

	if (camera->getMode() == 2)
	{
		Point p = camera->getPosition();
		float angel = 180 + camera->getRotatedY(), r = 40;

		p.x += r * sin(angel * PIdiv180);
		p.z += r * cos(angel * PIdiv180);
		p.y = -0.5 * 0.07 * cos(4 * (p.x + p.z));

	
	glTranslated(p.x, p.y, p.z);
	glRotated(angel,0,1,0);
	drawSpongeBob();
	
	}
	else {
	glTranslated(-10, 0, -10);
	drawSpongeBob();

	}

	
	return TRUE;
}


GLvoid KillGLWindow(GLvoid)								// Properly Kill The Window
{
	if (fullscreen)										// Are We In Fullscreen Mode?
	{
		ChangeDisplaySettings(NULL, 0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	}

	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL, NULL))					// Are We Able To Release The DC And RC Contexts?
		{
			MessageBox(NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
		{
			MessageBox(NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		}
		hRC = NULL;										// Set RC To NULL
	}

	if (hDC && !ReleaseDC(hWnd, hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hDC = NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hWnd = NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("OpenGL", hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
		hInstance = NULL;									// Set hInstance To NULL
	}
}

/*	This Code Creates Our OpenGL Window.  Parameters Are:					*
*	title			- Title To Appear At The Top Of The Window				*
*	width			- Width Of The GL Window Or Fullscreen Mode				*
*	height			- Height Of The GL Window Or Fullscreen Mode			*
*	bits			- Number Of Bits To Use For Color (8/16/24/32)			*
*	fullscreenflag	- Use Fullscreen Mode (TRUE) Or Windowed Mode (FALSE)	*/

BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left = (long)0;			// Set Left Value To 0
	WindowRect.right = (long)width;		// Set Right Value To Requested Width
	WindowRect.top = (long)0;				// Set Top Value To 0
	WindowRect.bottom = (long)height;		// Set Bottom Value To Requested Height

	fullscreen = fullscreenflag;			// Set The Global Fullscreen Flag

	hInstance = GetModuleHandle(NULL);				// Grab An Instance For Our Window
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc = (WNDPROC)WndProc;					// WndProc Handles Messages
	wc.cbClsExtra = 0;									// No Extra Window Data
	wc.cbWndExtra = 0;									// No Extra Window Data
	wc.hInstance = hInstance;							// Set The Instance
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.hbrBackground = NULL;									// No Background Required For GL
	wc.lpszMenuName = NULL;									// We Don't Want A Menu
	wc.lpszClassName = "OpenGL";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
	{
		MessageBox(NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;											// Return FALSE
	}

	if (fullscreen)												// Attempt Fullscreen Mode?
	{
		DEVMODE dmScreenSettings;								// Device Mode
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));	// Makes Sure Memory's Cleared
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);		// Size Of The Devmode Structure
		dmScreenSettings.dmPelsWidth = width;				// Selected Screen Width
		dmScreenSettings.dmPelsHeight = height;				// Selected Screen Height
		dmScreenSettings.dmBitsPerPel = bits;					// Selected Bits Per Pixel
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

		// Try To Set Selected Mode And Get Results.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
		if (ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		{
			// If The Mode Fails, Offer Two Options.  Quit Or Use Windowed Mode.
			if (MessageBox(NULL, "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?", "GL template", MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
			{
				fullscreen = FALSE;		// Windowed Mode Selected.  Fullscreen = FALSE
			}
			else
			{
				// Pop Up A Message Box Letting User Know The Program Is Closing.
				MessageBox(NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP);
				return FALSE;									// Return FALSE
			}
		}
	}

	if (fullscreen)												// Are We Still In Fullscreen Mode?
	{
		dwExStyle = WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle = WS_POPUP;										// Windows Style
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle = WS_OVERLAPPEDWINDOW;							// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hWnd = CreateWindowEx(dwExStyle,							// Extended Style For The Window
		"OpenGL",							// Class Name
		title,								// Window Title
		dwStyle |							// Defined Window Style
		WS_CLIPSIBLINGS |					// Required Window Style
		WS_CLIPCHILDREN,					// Required Window Style
		0, 0,								// Window Position
		WindowRect.right - WindowRect.left,	// Calculate Window Width
		WindowRect.bottom - WindowRect.top,	// Calculate Window Height
		NULL,								// No Parent Window
		NULL,								// No Menu
		hInstance,							// Instance
		NULL)))								// Dont Pass Anything To WM_CREATE
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		bits,										// Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		16,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	if (!(hDC = GetDC(hWnd)))							// Did We Get A Device Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))	// Did Windows Find A Matching Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!SetPixelFormat(hDC, PixelFormat, &pfd))		// Are We Able To Set The Pixel Format?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!(hRC = wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	if (!wglMakeCurrent(hDC, hRC))					// Try To Activate The Rendering Context
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	ShowWindow(hWnd, SW_SHOW);						// Show The Window
	SetForegroundWindow(hWnd);						// Slightly Higher Priority
	SetFocus(hWnd);									// Sets Keyboard Focus To The Window
	ReSizeGLScene(width, height);					// Set Up Our Perspective GL Screen

	if (!InitGL())									// Initialize Our Newly Created GL Window
	{
		KillGLWindow();								// Reset The Display
		MessageBox(NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
		return FALSE;								// Return FALSE
	}

	return TRUE;									// Success
}

LRESULT CALLBACK WndProc(HWND	hWnd,			// Handle For This Window
	UINT	uMsg,			// Message For This Window
	WPARAM	wParam,			// Additional Message Information
	LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
	case WM_ACTIVATE:							// Watch For Window Activate Message
	{
		if (!HIWORD(wParam))					// Check Minimization State
		{
			active = TRUE;						// Program Is Active
		}
		else
		{
			active = FALSE;						// Program Is No Longer Active
		}

		return 0;								// Return To The Message Loop
	}

	case WM_SYSCOMMAND:							// Intercept System Commands
	{
		switch (wParam)							// Check System Calls
		{
		case SC_SCREENSAVE:					// Screensaver Trying To Start?
		case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
			return 0;							// Prevent From Happening
		}
		break;									// Exit
	}

	case WM_CLOSE:								// Did We Receive A Close Message?
	{
		PostQuitMessage(0);						// Send A Quit Message
		return 0;								// Jump Back
	}

	case WM_MOUSEMOVE:
	{
		mx = (int)LOWORD(lParam);
		my = (int)HIWORD(lParam);
		isClicked = (LOWORD(wParam) & MK_LBUTTON) ? true : false;
		isRClicked = (LOWORD(wParam) & MK_RBUTTON) ? true : false;
		break;
	}

	case WM_KEYDOWN:							// Is A Key Being Held Down?
	{
		keys[wParam] = TRUE;					// If So, Mark It As TRUE
		if (keys['C']) {
			Camera::changeMode();
		}
		return 0;								// Jump Back
	}

	case WM_KEYUP:								// Has A Key Been Released?
	{
		keys[wParam] = FALSE;					// If So, Mark It As FALSE
		return 0;								// Jump Back
	}

	case WM_SIZE:								// Resize The OpenGL Window
	{
		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // LoWord=Width, HiWord=Height
		return 0;								// Jump Back
	}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE	hInstance,			// Instance
	HINSTANCE	hPrevInstance,		// Previous Instance
	LPSTR		lpCmdLine,			// Command Line Parameters
	int			nCmdShow)			// Window Show State
{
	MSG		msg;									// Windows Message Structure
	BOOL	done = FALSE;								// Bool Variable To Exit Loop

	// Ask The User Which Screen Mode They Prefer
	//if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen = FALSE;							// Windowed Mode
	}

	// Create Our OpenGL Window
	if (!CreateGLWindow((char*)"Example", 640, 480, 16, fullscreen))
	{
		return 0;									// Quit If Window Was Not Created
	}

	while (!done)									// Loop That Runs While done=FALSE
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Is There A Message Waiting?
		{
			if (msg.message == WM_QUIT)				// Have We Received A Quit Message?
			{
				done = TRUE;							// If So done=TRUE
			}
			else									// If Not, Deal With Window Messages
			{
				TranslateMessage(&msg);				// Translate The Message
				DispatchMessage(&msg);				// Dispatch The Message
			}
		}
		else										// If There Are No Messages
		{
			// Draw The Scene.  Watch For ESC Key And Quit Messages From DrawGLScene()
			if (active)								// Program Active?
			{
				if (keys[VK_ESCAPE])				// Was ESC Pressed?
				{
					done = TRUE;						// ESC Signalled A Quit
				}
				else								// Not Time To Quit, Update Screen
				{
					DrawGLScene();					// Draw The Scene
					SwapBuffers(hDC);				// Swap Buffers (Double Buffering)
				}
			}

			if (keys[VK_F1])						// Is F1 Being Pressed?
			{
				keys[VK_F1] = FALSE;					// If So Make Key FALSE
				KillGLWindow();						// Kill Our Current Window
				fullscreen = !fullscreen;				// Toggle Fullscreen / Windowed Mode
				// Recreate Our OpenGL Window
				if (!CreateGLWindow((char*)"OpenGL template", 640, 480, 16, fullscreen))
				{
					return 0;						// Quit If Window Was Not Created
				}
			}
		}
	}

	// Shutdown
	KillGLWindow();									// Kill The Window
	return (msg.wParam);							// Exit The Program
}

