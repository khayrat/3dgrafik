#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "palette.h"
#include "vertex.h"
#include "svertex.h"

long offset(int,int);
void make_uint32(pixel_32, Uint32*);
void createWindow();
void cleanup(); 
void drawline(svertex, svertex, pixel_32);
void drawline_vram(svertex, svertex); 
svertex project(vertex p); 
void test_projection(); 
void draw_functions(); 
void draw_function(svertex (* fun)(double x), double start, double end,  pixel_32 color); 
void draw_coordinatecross(pixel_32);
void draw_units(pixel_32);
void draw_arrows();
vertex e(double alpha); 
vertex circle(double alpha); 
vertex spiral(double alpha); 
vertex spiral2(double alpha); 
vertex spiral3(double alpha); 

// Screen dimension constants
const int SCREEN_WIDTH  = 640; // x
const int SCREEN_HEIGHT = 320; // y
const long x_res = SCREEN_WIDTH;
const long y_res = SCREEN_HEIGHT;


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

void drawline_vram(svertex begin, svertex end) {
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

void drawline(svertex begin, svertex end, pixel_32 color) {
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

svertex project(vertex p) {
	long vx = long((p.x - begin_x) * 
						(double(x_res-1) / (end_x - begin_x)));

	long vy = long((p.y - end_y) *
						(double( y_res-1 ) / (begin_y - end_y)));

	return svertex(vx,vy);
}

void test_projection() {
	begin_x = -100; end_x=100;
	begin_y = -100; end_y=100;
	int x = 5, y = 10;
	svertex screen_coord = project(vertex(x, y));
	printf("mathematical coord: begin_x:%d, end_x:%d, begin_y: %d, end_y: %d\n", begin_x, end_x, begin_y, end_y);
	printf("screen coord: begin_x:%d, end_x:%d, begin_y: %d, end_y: %d\n", 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
	printf("mathematical coord: (%d,%d), screen coord: (%d,%d)\n", x,y,screen_coord.x, screen_coord.y);
	printf("x-unit: %f, y-unit: %f\n", (1.)*SCREEN_WIDTH/(end_x-begin_x), (1.)*SCREEN_HEIGHT/(end_y-begin_y));
}

vertex e(double alpha) {
	double a = 3;
	double b= 2;
	double px = a*cos(alpha)+4;
	double py = b*sin(alpha)+3;
	return vertex(px,py);
}

vertex circle(double alpha) {
	double r = 3;
	double px = r*cos(alpha);
	double py = r*sin(alpha);
	return vertex(px,py);
}

vertex spiral(double alpha) {
	double r = 3;
	double roh = .3;
	double pho = .3;
	double rad = alpha; //alpha*pi/180;
	double px = roh*rad*r*cos(alpha);
	double py = pho*rad*r*sin(alpha);
	return vertex(px,py);
}

vertex spiral2(double alpha) {
	double r = 3;
	double roh = .1;
	double pho = .3;
	double rad = alpha; //alpha*pi/180;
	double px = roh*rad*r*cos(alpha);
	double py = pho*rad*r*sin(alpha);
	return vertex(px,py);
}

vertex spiral3(double alpha) {
	double r = 3;
	double roh = .3;
	double pho = .1;
	double rad = alpha; //alpha*pi/180;
	double px = roh*rad*r*cos(alpha);
	double py = pho*rad*r*sin(alpha);
	return vertex(px,py);
}

void draw_function(vertex (*fun) (double x), double start, double end, pixel_32 color) {
	/*
*/
	// set color
	SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, color.alpha);

	// connect renderer with the texture aka VRAM
	SDL_SetRenderTarget(renderer, display);

	svertex p = project((*fun)(start));

	double dl = 0.01;

	for (double x=start + dl; x<=end; x+=dl) {
		svertex q = project((*fun)(x));
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
	drawline(project(vertex(begin_x, 0)), project(vertex(end_x, 0)), color);
	svertex c = project(vertex(begin_x, 0)); 
	svertex d = project(vertex(end_x, 0));
	drawline(project(vertex(0, begin_y)), project(vertex(0, end_y)), color);
	svertex a = project(vertex(0, begin_y));
	svertex b = project(vertex(0, end_y));
	draw_arrows();
	//draw_units(color);
}

void draw_arrows() {
	svertex right = project(vertex(end_x, 0));
	svertex top = project(vertex(0, end_y));
	// x
	drawline(svertex(right.x - 11, right.y-5), right, pixel_32(128,128,128));
	drawline(svertex(right.x - 11, right.y+5), right, pixel_32(128,128,128));
	// y
	drawline(svertex(top.x - 5, top.y + 11), top, pixel_32(128,128,128));
	drawline(svertex(top.x + 5, top.y + 11), top, pixel_32(128,128,128));
}

void draw_units(pixel_32 color) {
	for (long x = begin_x ; x < end_x; x++) {
		svertex x_base = project(vertex(x,0));
		drawline(svertex(x_base.x, x_base.y+3), svertex(x_base.x, x_base.y-3), color);
	}
	for (long y = begin_y ; y < end_y; y++) {
		svertex y_base = project(vertex(0,y));
		drawline(svertex(y_base.x-3, y_base.y), svertex(y_base.x+3, y_base.y), color);
	}
}

void draw_functions() {
	begin_x = -100; end_x=100;
	end_y = (end_x/y_res) * x_res;
	begin_y = (begin_x/y_res) * x_res;
	//begin_y = -5; 
	//end_y=(double(y_res)/double(x_res))*(end_x - begin_x) + begin_y;
	draw_coordinatecross(pixel_32(128,128,128));
	//draw_function(&e, 0, 7, pixel_32(0,255,255));
	//draw_function(&circle, 1, 50, pixel_32(0,0,255));
	draw_function(&spiral, 1, 100, pixel_32(0,0,255));
	draw_function(&spiral2, 1, 100, pixel_32(0,255,0));
	draw_function(&spiral3, 1, 100, pixel_32(255,0,0));
}
