#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "palette.h"
#include "vertex.h"

long offset(int,int);
void make_uint32(pixel_32, Uint32*);
void createWindow();
void cleanup(); 
void drawline(vertex, vertex, pixel_32);
void drawline_vram(vertex, vertex); 
vertex project(double, double); 
void test_projection(); 
void draw_functions(); 
void draw_function(double (* fun)(double x), double start, double end,  pixel_32 color); 
void draw_coordinatecross(pixel_32);
void draw_units(pixel_32);
void draw_arrows();
double f(double);
double df(double);
double ddf(double);

// Screen dimension constants
const int SCREEN_WIDTH  = 640; // x
const int SCREEN_HEIGHT = 320; // y

const double pi = 3.1415926535;
// mathematical boundaries of the coordinate system
double begin_x, end_x;
double begin_y, end_y;


// The windwo we'll be rendering to
SDL_Window* window = NULL;

SDL_Texture* display = NULL;
SDL_Renderer* renderer = NULL;

int main(int argc, char* args[]) {
	createWindow();

	pixel_32 color = palette[rand() % 256];

	SDL_Event event;
	bool quit = false;
	//	drawline(begin, end, color);

	//test_projection();


	double (*fun) (double);
	fun = &f;
	(*fun)(2);

	draw_functions();

	// wait
	while(true) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
					quit = true;
					printf("exiting...\n");
			}
		}
		if (quit) break;
	}

	cleanup();
	return 0;
}

void createWindow() {
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}

	// Create Widonw
	window = SDL_CreateWindow(
			"Functions",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);

	if (window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}
	
	// Get window renderer
	renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED   |
																					 SDL_RENDERER_TARGETTEXTURE |
																					 SDL_RENDERER_PRESENTVSYNC);
	// Get the texture
	display = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
																				SDL_TEXTUREACCESS_TARGET,
																				SCREEN_WIDTH,
																				SCREEN_HEIGHT);

	// SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	// Update screen
	SDL_SetRenderTarget(renderer, display);
	SDL_RenderClear(renderer);
	SDL_SetRenderTarget(renderer, NULL);
	SDL_RenderPresent(renderer);
}

void	cleanup() {
	// Destroy window
	SDL_DestroyWindow(window);

	// Qutt SDL subsystems
	SDL_Quit();
}

long offset(int y, int x) {
	if (y >= SCREEN_WIDTH || x >= SCREEN_HEIGHT || y < 0 || x < 0) {
		printf("y: %d, or x: %d out of range: (%d/%d)\n", y, x, SCREEN_WIDTH, SCREEN_HEIGHT);
		exit(1);
	}
	return y * SCREEN_WIDTH + x;
}

void make_uint32(pixel_32 pixel, Uint32 *p) {
	  memcpy(p, &pixel, sizeof(pixel_32));
}

void drawline_vram(vertex begin, vertex end) {
	long delta_x = end.x - begin.x;
	long delta_y = end.y - begin.y;
	long offset = begin.y * SCREEN_WIDTH + begin.x;
	long xstep = 1;
	long ystep = SCREEN_WIDTH;
	long x, y, e, length;

	// draw pixel to VRAM
	if (
			begin.x < 0 || begin.x >= SCREEN_WIDTH || begin.y < 0 || begin.y >= SCREEN_HEIGHT ||
			end.x   < 0 || end.x   >= SCREEN_WIDTH || end.y   < 0 || end.y   >= SCREEN_HEIGHT
		 )
		return;

	if (delta_x < 0) {
		// move from right to left 
		delta_x = -delta_x;
		xstep = -xstep;
	}
	if (delta_y < 0) {
		// move from down to up
		delta_y = -delta_y;
		ystep = -ystep;
	}

	if (delta_y > delta_x) {
		// m > 0: line is increasing
		long tmp;
		tmp	= delta_x; delta_x = delta_y; delta_y = tmp;
		tmp = xstep; xstep = ystep; ystep = tmp;
	}

	length = delta_x+1; e = 0;

	x = offset % SCREEN_WIDTH;
	y = offset / SCREEN_WIDTH;

	while(length-- > 0) {
		x = offset % SCREEN_WIDTH;
		y = offset / SCREEN_WIDTH;
		SDL_RenderDrawPoint(renderer, x, y);
		offset += xstep;

		e += delta_y;
		if (e >= delta_x) {
			e -= delta_x;
			offset += ystep;
		}
	}
}

void drawline(vertex begin, vertex end, pixel_32 color) {
	long delta_x = end.x - begin.x;
	long delta_y = end.y - begin.y;
	long offset = begin.y * SCREEN_WIDTH + begin.x;
	long xstep = 1;
	long ystep = SCREEN_WIDTH;
	long x, y, e, length;

	if (
			begin.x < 0 || begin.x > SCREEN_WIDTH || begin.y < 0 || begin.y > SCREEN_HEIGHT ||
			end.x   < 0 || end.x   > SCREEN_WIDTH || end.y   < 0 || end.y   > SCREEN_HEIGHT
		 )
		return;

	if (delta_x < 0) {
		// move from right to left 
		delta_x = -delta_x;
		xstep = -xstep;
	}
	if (delta_y < 0) {
		// move from down to up
		delta_y = -delta_y;
		ystep = -ystep;
	}

	if (delta_y > delta_x) {
		// m > 0: line is increasing
		long tmp;
		tmp	= delta_x; delta_x = delta_y; delta_y = tmp;
		tmp = xstep; xstep = ystep; ystep = tmp;
	}

	length = delta_x+1; e = 0;

	// set color
	SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, color.alpha);

	// connect renderer with the texture aka VRAM
	SDL_SetRenderTarget(renderer, display);

	x = offset % SCREEN_WIDTH;
	y = offset / SCREEN_WIDTH;
	//printf("begin:(%d,%d), end:(%d,%d)\n", begin.x, begin.y, end.x, end.y);
	//printf("delta_x: %d, delta_y: %d\n", delta_x, delta_y);
	//printf("start:(%d,%d)\n", x,y);
	//printf("length: %d, e: %d\n", length, e);
	//printf("xstep: %d, ystep: %d\n", xstep, ystep);

	while(length-- > 0) {
		x = offset % SCREEN_WIDTH;
		y = offset / SCREEN_WIDTH;
		SDL_RenderDrawPoint(renderer, x, y);
		offset += xstep;

		e += delta_y;
		if (e >= delta_x) {
			e -= delta_x;
			offset += ystep;
		}
	}
	//printf("end:(%d,%d)\n", x,y);

	// connect renderer with screen
	SDL_SetRenderTarget(renderer, NULL);

	// bring VRAM closer to the screen display
	SDL_RenderCopy(renderer, display, NULL, NULL);

	// draw to the screen
	SDL_RenderPresent(renderer);
}

vertex project(double x, double y) {
	long vx = long((x - begin_x) * SCREEN_WIDTH / (end_x - begin_x));
	long vy = long((y - end_y) * SCREEN_HEIGHT / (begin_y - end_y));

	return vertex(vx,vy);
}

void test_projection() {
	begin_x = -100; end_x=100;
	begin_y = -100; end_y=100;
	int x = 5, y = 10;
	vertex screen_coord = project(x, y);
	printf("mathematical coord: begin_x:%d, end_x:%d, begin_y: %d, end_y: %d\n", begin_x, end_x, begin_y, end_y);
	printf("screen coord: begin_x:%d, end_x:%d, begin_y: %d, end_y: %d\n", 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
	printf("mathematical coord: (%d,%d), screen coord: (%d,%d)\n", x,y,screen_coord.x, screen_coord.y);
	printf("x-unit: %f, y-unit: %f\n", (1.)*SCREEN_WIDTH/(end_x-begin_x), (1.)*SCREEN_HEIGHT/(end_y-begin_y));
}

double a = .001, b = -.0093, c = -.06, d = .464;
double f(double x) {
	return a*pow(x,4) + b*pow(x,3) + c*pow(x,2) + d*x;
}
double df(double x) {
	return 4*a*pow(x,3) + 3*b*pow(x,2) + 2*c*x + d;
}
double ddf(double x) {
	return 12*a*pow(x,2) + 6*b*x + 2*c;
}

void draw_function(double (*fun) (double x), double start, double end, pixel_32 color) {
	/*
*/
	// set color
	SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, color.alpha);

	// connect renderer with the texture aka VRAM
	SDL_SetRenderTarget(renderer, display);
	vertex p = project(start, (*fun)(start));

	double step = fabs(end_x - begin_x) / double(SCREEN_WIDTH);

	for (double x=start + step; x<=end_x; x+=step) {
		vertex q = project(x, (*fun)(x));
		//drawline(p, q, color);
		drawline_vram(p, q);
		p=q;
	}

	/*
*/
	// connect renderer with screen
	SDL_SetRenderTarget(renderer, NULL);

	// bring VRAM closer to the screen display
	SDL_RenderCopy(renderer, display, NULL, NULL);

	// draw to the screen
	SDL_RenderPresent(renderer);
}
	
void draw_coordinatecross(pixel_32 color) {
	drawline(project(begin_x, 0), project(end_x, 0), color);
	drawline(project(0, begin_y), project(0, end_y), color);
	draw_arrows();
	draw_units(color);
}

void draw_arrows() {
	vertex right = project(end_x, 0);
	vertex top = project(0, end_y);
	// x
	drawline(vertex(right.x - 11, right.y-5), right, pixel_32(128,128,128));
	drawline(vertex(right.x - 11, right.y+5), right, pixel_32(128,128,128));
	// y
	drawline(vertex(top.x - 5, top.y + 11), top, pixel_32(128,128,128));
	drawline(vertex(top.x + 5, top.y + 11), top, pixel_32(128,128,128));
}

void draw_units(pixel_32 color) {
	for (long x = begin_x ; x < end_x; x++) {
		vertex x_base = project(x,0);
		drawline(vertex(x_base.x, x_base.y+3), vertex(x_base.x, x_base.y-3), color);
	}
	for (long y = begin_y ; y < end_y; y++) {
		vertex y_base = project(0,y);
		drawline(vertex(y_base.x-3, y_base.y), vertex(y_base.x+3, y_base.y), color);
	}
}

void draw_functions() {
	/*
	begin_x = -9; end_x=14;
	begin_y = -3; end_y=4;
	draw_function(&f, pixel_32(255,0,255));
	draw_function(&df, pixel_32(255,0,0));
	draw_function(&ddf, pixel_32(0,0,255));
	*/
	begin_x = -4*pi; end_x=4*pi;
	begin_y = -5; end_y=5;
	draw_function(&sin, begin_x, end_x, pixel_32(0,255,0));
	draw_function(&cos, begin_x, end_x, pixel_32(0,255,255));
	draw_function(&tan, begin_x, end_x, pixel_32(0,255,255));

	draw_coordinatecross(pixel_32(128,128,128));
}
