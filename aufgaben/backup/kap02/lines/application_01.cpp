#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "palette.h"
#include "vertex.h"

long offset(int,int);
void make_uint32(pixel_32, Uint32*);
void createWindow();
void draw_pixel(int, int, pixel_32);
void putpixel(SDL_Surface *, int, int, Uint32);
void cleanup(); 
void drawline(vertex, vertex, pixel_32);

// Screen dimension constants
const int SCREEN_WIDTH  = 640; // x
const int SCREEN_HEIGHT = 320; // y

// The windwo we'll be rendering to
SDL_Window* window = NULL;

// The surface contained by the window
SDL_Surface* screenSurface = NULL;

int main(int argc, char* args[]) {
	createWindow();

	vertex begin = vertex(100, 100);
	vertex end = vertex(200, 310);

	SDL_Event event;
	bool quit = false;
	for (int i=0; i<100; i++) {
		drawline(begin, end, palette[rand() % 256]);
		begin = end;
		end = vertex(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);

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
			"Lines",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);

	if (window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		exit(1);
	}
	
	// Get window surface
	screenSurface = SDL_GetWindowSurface(window);

	// Fill the surface white
	SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0, 0, 0));

	// Update the surface
	SDL_UpdateWindowSurface(window);
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

void draw_pixel(int x, int y, pixel_32 pixel) {
		Uint32 _pixel;
		make_uint32(pixel, &_pixel);
		//printf("draw x:%d, y.%d\n", x, y);
		long offset = SCREEN_WIDTH*SCREEN_HEIGHT;
		long current_offset = y*SCREEN_WIDTH + x;

		if (current_offset > offset) {
			printf("offset out of range: x:%d, y:%d, offset: %d, current_offset: %d!!\n", x, y, offset, current_offset);
			exit(1);
		}

		putpixel(screenSurface, x, y, _pixel);

		SDL_UpdateWindowSurface(window);
}

void putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * 4;
	*((Uint32 *) p) = pixel;
}

// portable version of putpixel
void _putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * bpp;

	switch(bpp) {
	case 1:
			*p = pixel;
			break;

	case 2:
			*(Uint16 *)p = pixel;
			break;

	case 3:
			if(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
					p[0] = (pixel >> 16) & 0xff;
					p[1] = (pixel >> 8) & 0xff;
					p[2] = pixel & 0xff;
			} else {
					p[0] = pixel & 0xff;
					p[1] = (pixel >> 8) & 0xff;
					p[2] = (pixel >> 16) & 0xff;
			}
			break;

	case 4:
			*(Uint32 *)p = pixel;
			break;
	}
}

void drawline(vertex begin, vertex end, pixel_32 color) {
	double detail = 0.001;

	int vx = end.x - begin.x;
	int vy = end.y - begin.y;

	/*
	printf("begin      : (%d,%d)\n", begin.x, begin.y);
	printf("end        : (%d,%d)\n", end.x, end.y);
	printf("(being-end): (%d,%d)\n", vx, vy);
	*/

	for (double t=0; t <= 1; t += detail) {
		ushort x = begin.x + t * vx;
		ushort y = begin.y + t * vy;
		draw_pixel(x, y, color); 
	}
}
