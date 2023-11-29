#include "planet.h"
#include "color.h"
#include "orbit.h"
#include "ring.h"
#include <windows.h>		// Header File For Windows
#include <gl.h>			// Header File For The OpenGL32 Library
#include <glu.h>			// Header File For The GLu32 Library
#include <glaux.h>		// Header File For The Glaux Library
#include <cmath>
#include <texture.h>
#define db double
const double pi = 3.14159265358979323846;


HDC			hDC = NULL;		// Private GDI Device Context
HGLRC		hRC = NULL;		// Permanent Rendering Cntext
HWND		hWnd = NULL;		// Holds Our Window Handle
HINSTANCE	hInstance;		// Holds The Instance Of The Application

bool	keys[256];			// Array Used For The Keyboard Routine
bool	active = TRUE;		// Window Active Flag Set To TRUE By Default
bool	fullscreen = FALSE;	// Fullscreen Flag Set To Fullscreen Mode By Default

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

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
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

int wall, brick, grass, wallpaper, carpet, wood;

int InitGL(GLvoid)										// All Setup For OpenGL Goes Here
{
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// Black Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations

	glEnable(GL_TEXTURE_2D);
	/*wall = LoadTexture((char*)"D:/wall.bmp");
	brick = LoadTexture((char*)"D:/brick.bmp");
	grass = LoadTexture((char*)"D:/grass.bmp");
	wallpaper = LoadTexture((char*)"D:/wallpaper.bmp");
	carpet = LoadTexture((char*)"D:/carpet.bmp");*/
	//wood = LoadTexture((char*)"D:/jupiter.bmp");

	glEnable(GL_CULL_FACE);

	return TRUE;										// Initialization Went OK
}

//int s = 1, t = 1;

//GLUquadric* quad = gluNewQuadric();

//attributs
db eyeX = 0, eyeY = 1, eyeZ = 0;
db centerX = 0, centerY = 0, centerZ = -5;
double angle_step = 0.1, move_step = 0.1, angle = 0;
void Camera(db n = 0.1)
{
	gluLookAt(eyeX, eyeY, eyeZ, centerX, centerY, centerZ, 0, 1, 0);
	if (keys['D'])
		eyeX += n;
	if (keys['A'])
		eyeX -= n;
	if (keys['W'])
		eyeY += n;
	if (keys['S'])
		if (eyeY > 1) eyeY -= n;
	if (keys['Z'])
		eyeZ += n;
	if (keys['X'])
		eyeZ -= n;
	if (keys[VK_LEFT])
		centerX += n;
	if (keys[VK_RIGHT])
		centerX -= n;
	if (keys[VK_UP])
		centerY += n;
	if (keys[VK_DOWN])
		centerY -= n;
	/*if (keys[VK_LEFT]) {
		angle += angle_step * pi / 180;
		centerX = eyeX + cos(angle) * (centerX - eyeX) - sin(angle) * (centerZ - eyeZ);
		centerZ = eyeZ + sin(angle) * (centerX - eyeX) + cos(angle) * (centerZ - eyeZ);
	}
	if (keys[VK_RIGHT]) {
		angle -= angle_step * pi / 180;
		centerX = eyeX + cos(angle) * (centerX - eyeX) - sin(angle) * (centerZ - eyeZ);
		centerZ = eyeZ + sin(angle) * (centerX - eyeX) + cos(angle) * (centerZ - eyeZ);
	}
	if (keys[VK_UP]) {
		eyeX += (centerX - eyeX) / sqrt(pow(centerX - eyeX, 2) + pow(centerZ - eyeZ, 2)) * move_step;
		eyeZ += (centerZ - eyeZ) / sqrt(pow(centerX - eyeX, 2) + pow(centerZ - eyeZ, 2)) * move_step;
		centerX += (centerX - eyeX) / sqrt(pow(centerX - eyeX, 2) + pow(centerZ - eyeZ, 2)) * move_step;
		centerZ += (centerZ - eyeZ) / sqrt(pow(centerX - eyeX, 2) + pow(centerZ - eyeZ, 2)) * move_step;
	}
	if (keys[VK_DOWN]) {
		eyeX -= (centerX - eyeX) / sqrt(pow(centerX - eyeX, 2) + pow(centerZ - eyeZ, 2)) * move_step;
		eyeZ -= (centerZ - eyeZ) / sqrt(pow(centerX - eyeX, 2) + pow(centerZ - eyeZ, 2)) * move_step;
		centerX -= (centerX - eyeX) / sqrt(pow(centerX - eyeX, 2) + pow(centerZ - eyeZ, 2)) * move_step;
		centerZ -= (centerZ - eyeZ) / sqrt(pow(centerX - eyeX, 2) + pow(centerZ - eyeZ, 2)) * move_step;
	}*/

}

db a = 0; db s = 1; //rotation speed
db mercurya = 0, mercurys = 0.2;
db venusa = 0, venuss = 0.15;
db eartha = 0, earths = 0.1;
db marsa = 0, marss = 0.15;
db jupitera = 0, jupiters = 0.3;
db saturna = 0, saturns = 0.1534;
db uranusa = 0, uranuss = 0.22;
db neptunea = 0, neptunes = 0.1;


int DrawGLScene(GLvoid)									// Here's Where We Do All The Drawing
{
	//drawscene opereates 30 times everysecond, so I draw a sqaure, moved the world and cleared it, reload the world 
	//with initial coordinates, translated it and draw the squrea then cleared everything and so on, this way, u get the illusion
	//that shapes are moving
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslated(0, 0, -10);
	glDisable(GL_CULL_FACE);
	Camera(0.5);

	/*glBindTexture(GL_TEXTURE_2D, wood);
	GLUquadricObj* quadric = gluNewQuadric();
	gluQuadricTexture(quadric, GL_TRUE);
	gluQuadricNormals(quadric, GLU_SMOOTH);
	gluSphere(quadric, 2.0, 32, 32);*/

	//orbits
	glPushMatrix();
	glColor3f(1, 1, 1);
	orbit o1(10, 0.1);
	orbit o2(13, 0.1);
	orbit o3(17, 0.1);
	orbit o4(20, 0.1);
	orbit o5(25, 0.1);
	orbit o6(30, 0.1);
	orbit o7(36, 0.1);
	orbit o8(39, 0.1);
	glPopMatrix();

	glPushMatrix();

	//the sun 
	glPushMatrix();
	color yellow(255, 204, 0);
	glRotated(a, 0, 1, 1);
	planet sun(5, yellow, 0, 1, 0);
	glPopMatrix();

	//mercury
	glPushMatrix();
	glRotated(mercurya, 0, 1, 0);
	glTranslated(0, 0, 10);
	glRotated(a, 0, 0, 1);
	color c(102, 51, 0);
	planet mercury(0.2, c, 0, 1, 0);
	glPopMatrix();

	//venus
	glPushMatrix();
	glRotated(-venusa, 0, 1, 0);
	glTranslated(0, 0, 13);
	glRotated(a, 0, 0, 1);
	color c1(255, 153, 0);
	planet venus(0.5, c1, 1, 1, 0);
	glPopMatrix();


	//earth
	glPushMatrix();
	glRotated(eartha, 0, 1, 0);
	glTranslated(0, 0, 17);
	glRotated(a, 0, 0, 1);
	color c2(0, 253, 255);
	planet earth(0.55, c2, 0, 1, 0);
	glPopMatrix();

	//mars
	glPushMatrix();
	glRotated(marsa, 0, 1, 0);
	glTranslated(0, 0, 20);
	glRotated(a, 0, 0, 1);
	color c3(255, 102, 20);
	planet mars(0.4, c3, 1, 1, 1);
	glPopMatrix();

	//jupiter
	glPushMatrix();
	glRotated(jupitera, 0, 1, 0);
	glTranslated(0, 0, 25);
	glRotated(a, 0, 0, 1);
	color c4(255, 117, 26);
	planet jupiter(1.5, c4, 0, 1, 1);
	glPopMatrix();


	//saturn
	glPushMatrix();
	glRotated(saturna, 0, 1, 0);
	glTranslated(0, 0, 30);
	glRotated(20, 0, 1, 0);
	glRotated(a, 0, 1, 0);
	color c5(209, 170, 119);
	planet saturn(1.2, c5, 0, 0, 0);
	color rc(255, 230, 204);
	ring r1(1.45, 1.3, rc);
	ring r2(2, 1.5, rc);
	ring r3(2.4, 2.2, rc);
	glPopMatrix();

	//uranus
	glPushMatrix();
	glRotated(uranusa, 0, 1, 0);
	glTranslated(0, 0, 36);
	glRotated(20, 0, 1, 0);
	glRotated(a, 0, 1, 0);
	color c6(46, 202, 207);
	planet uranus(0.8, c6, 1, 0, 0);
	color rc1(255, 230, 204);
	ring ur(1.33, 1.3, rc);
	glPopMatrix();

	//neptune
	glPushMatrix();
	glRotated(neptunea, 0, 1, 0);
	glTranslated(0, 0, 39);
	glRotated(20, 0, 1, 0);
	glRotated(a, 0, 1, 0);
	color c7(37, 66, 138);
	planet nuptune(0.8, c7, 0, 1, 0);
	glPopMatrix();

	//
	glPopMatrix();

	a += s;
	mercurya += mercurys;
	venusa += venuss;
	eartha += earths;
	marsa += marss;
	jupitera += jupiters;
	saturna += saturns;
	uranusa += uranuss;
	neptunea += neptunes;



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

	case WM_KEYDOWN:							// Is A Key Being Held Down?
	{
		keys[wParam] = TRUE;					// If So, Mark It As TRUE
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

