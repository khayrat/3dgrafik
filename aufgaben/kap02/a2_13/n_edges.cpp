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

std::string programName = "N-Edges";

// Our SDL_Window ( just like with SDL2 wihout OpenGL)
SDL_Window *mainWindow;

// Our opengl context handle
SDL_GLContext mainContext;

bool SetOpenGLAttributes();
void PrintSDL_GL_Attributes();
void CheckSDLError(int line);
void Render();
void Cleanup();

static pixel_32 my_palette[256];
void load_palette();

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

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
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
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
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0);

	load_palette();

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

pixel_32 ChooseColor();
svertex ChoosePoint();

void Render()
{
	const double pi = 3.1415926535;
	SDL_Event event;

	while (true)
	{
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
			break;

		// long mx = (rand() % 638) + 2;
		// long my = (rand() % 478) + 2;
		uint mx = (rand() % (SCREEN_WIDTH - 2)) + 2;
		uint my = (rand() % (SCREEN_HEIGHT - 2)) + 2;
		uint n = (rand() % 8) + 3;

		uint r = rand() % (uint)(fmin(fmin(mx, SCREEN_WIDTH - mx), fmin(my, SCREEN_HEIGHT - my)));

		printf("render n: %d, (%d, %d), r: %d\n", n, mx, my, r);

		pixel_32 c = ChooseColor();
		glColor3ub(c.red, c.green, c.blue);

		glBegin(GL_LINE_LOOP);

		for (long i = 0; i < n; i++)
		{
			double x = r * cos(i * 2 * pi / n);
			double y = r * sin(i * 2 * pi / n);

			glVertex2d(mx + x, my + y);
		}

		glEnd();

		glFinish();

		// render
		glFlush();
	}
}

pixel_32 ChooseColor()
{
	return my_palette[rand() % 256];
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

void load_palette()
{
	int part_size = 256 / 4;
	int red_offset = 0 * part_size;
	int white_offset = 1 * part_size;
	int green_offset = 2 * part_size;
	int blue_offset = 3 * part_size;
	int c = 0; // to fit the 256 sized color-run into size of the part

	for (int i = 0; i < part_size; i++, c += 4)
	{
		my_palette[red_offset + i] = pixel_32(c, 0, 0);		// black-red
		my_palette[white_offset + i] = pixel_32(c, c, c); // black-white
		my_palette[green_offset + i] = pixel_32(0, c, 0); // black-green
		my_palette[blue_offset + i] = pixel_32(0, 0, c);	// black-blue
	}
}