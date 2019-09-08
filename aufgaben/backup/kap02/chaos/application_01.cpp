#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "palette.h"
#include "point.h"

long offset(int,int);
void make_uint32(pixel_32, Uint32*);
void createWindow();
void draw_pixel(int, int, pixel_32);
void cleanup(); 
void wait();

// Screen dimension constants
const int SCREEN_WIDTH  = 640; // x
const int SCREEN_HEIGHT = 600; // y

// View port
//const int V_WIDTH = 200;
//const int V_HEIGHT = 100;
const int V_WIDTH = SCREEN_WIDTH;
const int V_HEIGHT = SCREEN_HEIGHT;

// Triangles
const int MAX_POINTS = 100000;
point trig[3];
point act_point;

void createTrigCorners();
void playChaos();
point choosePoint();
point midpoint(point, point);

// The windwo we'll be rendering to
SDL_Window* window = NULL;

// The surface contained by the window
SDL_Surface* screenSurface = NULL;

SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;

// The pixels representing the screen content.
Uint32 *pixels = NULL; 

int main(int argc, char* args[]) {
	createWindow();

	createTrigCorners();

	playChaos();

	wait();

	cleanup();
	return 0;
}

void createTrigCorners() {
	int start_x = (SCREEN_WIDTH - V_WIDTH) / 2;
	int end_x = V_WIDTH;
	int start_y = (SCREEN_HEIGHT - V_HEIGHT) / 2;
	int end_y = V_HEIGHT;
	for (int i = 0; i < 3; i++) {
		int x = start_x + random() % (end_x - start_x);
		int y = start_y + random() % (end_y - start_y);
		trig[i] = point(y,x);
	}
	/*
								// y ,   x
	trig[0] = point( 95, 435);
	trig[1] = point(320,  45);
	trig[2] = point(545, 435);
	*/
}

// randomly choose a trig corner
point choosePoint() {
	return trig[random() % 3];
}

void playChaos() {
	// select pixel
	int index = rand() % 256;
	pixel_32 pixel = palette[index];

	// randomly choose a point
	act_point = choosePoint();

	int i=0;
	while (i++ < 5000) {
		// randomly choose another point
		point a = choosePoint();

		//printf("act_point: (%d,%d)\n", act_point.y, act_point.x);
		//printf("another point: (%d,%d)\n", a.y, a.x);

		// find the midpoint
		point m = midpoint(act_point, a);
		//printf("mid point: (%d,%d)\n", m.y, m.x);

		// the midpoint becomes the new act_point
		act_point = m;

		// draw the midpoint
		draw_pixel(m.x, m.y, pixel);
	}	
}

point midpoint(point a, point b) {
	return point((b.y + a.y) / 2, (b.x + a.x) / 2);
}

void draw_pixel(int x, int y, pixel_32 pixel) {
		Uint32 _p;
		make_uint32(pixel, &_p);
		//printf("draw x:%d, y.%d\n", x, y);
		long offset = SCREEN_WIDTH*SCREEN_HEIGHT;
		long current_offset = y*SCREEN_WIDTH + x;

		if (current_offset > offset) {
			printf("offset out of range!!\n");
			exit(1);
		}

		pixels[current_offset] = _p;

		SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
}

void wait() {
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		SDL_WaitEvent(&event);
		switch (event.type) {
		case SDL_QUIT:
				quit = true;
				printf("exiting...\n");
				break;
		}
	}
}

void createWindow() {
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}

	// Create Widonw
	window = SDL_CreateWindow(
			"Schnee",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);

	if (window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}

	renderer = SDL_CreateRenderer(window, -1, 0);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);

	pixels = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT]; 
	memset(pixels, 255, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));
}

void	cleanup() {
	// Destroy window
	SDL_DestroyWindow(window);

	delete[] pixels;
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);

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
