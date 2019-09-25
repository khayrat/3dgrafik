// Headerphile.com OpenGL Tutorial part 1
// A Hello World in the world of OpenGL ( creating a simple windonw and setting background color )
// Source code is an C++ adaption / simplicication of : https://www.opengl.org/wiki/Tutorial1:_Creating_a_Cross_Platform_OpenGL_3.2_Context_in_SDL_(C_/_SDL)
// Compile : clang++ main.cpp -lGL -lSDL2 -std=c++11 -o Test

// C++ Headers
#include <string>
#include <iostream>

// OpenGL / glew Headers
#define GL3_PROTOTYPES 1
#include <GL/glew.h>

// SDL2 Headers
#include <SDL2/SDL.h>

#include "palette.h"

std::string programName = "Zycloid on a line";

// Our SDL_Window ( just like with SDL2 wihout OpenGL)
SDL_Window *mainWindow;

// Our opengl context handle
SDL_GLContext mainContext;

bool SetOpenGLAttributes();
void PrintSDL_GL_Attributes();
void CheckSDLError(int line);
void Render();
void Cleanup();

void draw_xy(double begin_x, double end_x, double begin_y, double end_y);
void draw_f(svertex (*f)(double x), double begin, double end, pixel_32 color);
svertex kreis(double alpha);
svertex z(double alpha);
svertex zs(double alpha);
svertex zb(double alpha);
svertex zg(double alpha);
svertex lr(double alpha);

double t = 1;
double r = 1;
const double pi = 3.1415926535;
double begin_x = -6, end_x = 6;
double begin_y = -3.2, end_y = 5.8;
int x_res = 640;
int y_res = 480;

bool Init()
{
	// Initialize SDL's Video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Failed to init SDL\n";
		return false;
	}

	SetOpenGLAttributes();

	// Create our window centered
	mainWindow = SDL_CreateWindow(
			programName.c_str(),
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			x_res,
			x_res,
			SDL_WINDOW_OPENGL);

	// Check that everything worked out okay
	if (!mainWindow)
	{
		std::cout << "Unable to create window\n";
		CheckSDLError(__LINE__);
		return false;
	}

	// Create our opengl context and attach it to our window
	mainContext = SDL_GL_CreateContext(mainWindow);

// Init GLEW
// Apparently, this is needed for Apple. Thanks to Ross Vander for letting me know
#ifndef __APPLE__
	glewExperimental = GL_TRUE;
	glewInit();
#endif

	// white background
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(begin_x, end_x, begin_y, end_y);

	return true;
}

bool SetOpenGLAttributes()
{
	// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	// Enable compatibility mode
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

	// Set our OpenGL version. Use older version
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	// turn off double buffering
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 0);

	return true;
}

int main(int argc, char *argv[])
{
	if (!Init())
		return -1;

	Render();

	Cleanup();

	return 0;
}

void Render()
{
	const double pi = 3.1415926535;
	SDL_Event event;

	glColor3ub(0, 0, 0);

	while (true)
	{
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
			break;

		draw_xy(begin_x, end_x, begin_y, end_y);
		draw_f(kreis, begin_x, end_x, pixel_32(0, 255, 0));
		draw_f(zg, begin_x, end_x, pixel_32(255, 0, 255));
		draw_f(zs, begin_x, end_x, pixel_32(255, 0, 0));
		draw_f(zb, begin_x, end_x, pixel_32(0, 255, 255));

		// render
		glFlush();
	}
}

void Cleanup()
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(mainContext);

	// Destroy our window
	SDL_DestroyWindow(mainWindow);

	// Shutdown SDL 2
	SDL_Quit();
}

void CheckSDLError(int line = -1)
{
	std::string error = SDL_GetError();

	if (error != "")
	{
		std::cout << "SLD Error : " << error << std::endl;

		if (line != -1)
			std::cout << "\nLine : " << line << std::endl;

		SDL_ClearError();
	}
}

void PrintSDL_GL_Attributes()
{
	int value = 0;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION : " << value << std::endl;

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: " << value << std::endl;
}

void draw_xy(double begin_x, double end_x, double begin_y, double end_y)
{
	glColor3ub(128, 128, 128);

	glBegin(GL_LINES);

	glVertex2d(begin_x, 0);
	glVertex2d(end_x, 0);
	glVertex2d(0, begin_y);
	glVertex2d(0, end_y);

	double e1, e2;

	e1 = -fabs(end_y - begin_y) / double(y_res) * 3;
	e2 = fabs(end_y - begin_y) / double(y_res) * 4;
	for (long x = long(begin_x); x <= long(end_x); x++)
	{
		glVertex2d(x, e1);
		glVertex2d(x, e2);
	}

	e1 = -fabs(end_x - begin_x) / double(x_res) * 3;
	e2 = fabs(end_x - begin_x) / double(x_res) * 4;
	for (long y = long(begin_y); y <= long(end_y); y++)
	{
		glVertex2d(e1, y);
		glVertex2d(e2, y);
	}

	e1 = fabs(end_x - begin_x) / double(x_res) * 11;
	e2 = fabs(end_y - begin_y) / double(y_res) * 5;
	glVertex2d(end_x - e1, -e2);
	glVertex2d(end_x, 0);
	glVertex2d(end_x - e1, e2);
	glVertex2d(end_x, 0);

	e1 = fabs(end_x - begin_x) / double(x_res) * 5;
	e2 = fabs(end_y - begin_y) / double(y_res) * 11;
	glVertex2d(-e1, end_y - e2);
	glVertex2d(0, end_y);
	glVertex2d(e1, end_y - e2);
	glVertex2d(0, end_y);

	glEnd();
}

void draw_f(svertex (*f)(double x), double begin, double end, pixel_32 color)
{
	glColor3ub(color.red, color.green, color.blue);

	glBegin(GL_LINE_STRIP);

	for (double x = -100; x <= 100; x += 0.01)
	{
		svertex p = f(x);
		glVertex2d(p.sx, p.sy);
	}

	glEnd();
}

svertex kreis(double alpha)
{
	return svertex(r * cos(alpha), r * sin(alpha) + r);
}

svertex lr(double alpha)
{
	return svertex(t * r * sin(alpha), t * r * cos(alpha));
}

svertex zg(double alpha)
{
	svertex rotation = lr(alpha);
	return svertex(rotation.sx + alpha * r, rotation.sy + r);
}
svertex z(double alpha)
{
	return svertex(t * r * sin(alpha) + alpha * r, t * r * cos(alpha) + r);
}

svertex zs(double alpha)
{
	double ts = -0.3;

	return svertex(ts * r * sin(alpha) + alpha * r, ts * r * cos(alpha) + r);
}

svertex zb(double alpha)
{
	double tb = 1.72;

	return svertex(tb * r * sin(alpha) + alpha * r, tb * r * cos(alpha) + r);
}
