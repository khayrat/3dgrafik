#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "palette.h"

long offset(int,int);
void make_uint32(pixel_32, Uint32*);
void createWindow();
void cleanup(); 

// Screen dimension constants
const int SCREEN_WIDTH  = 200;
const int SCREEN_HEIGHT = 100;

void displayRandom();

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

	displayRandom();

	cleanup();
	return 0;
}

void displayRandom() {
	int i=0;
	while (i++<10000) {
		int x = rand() % SCREEN_WIDTH;
		int y = rand() % SCREEN_HEIGHT;
		int entry = 16 + rand() % 16;

		pixel_32 c32 = palette[entry];
		Uint32 color;
		make_uint32(c32, &color);
		pixels[y*SCREEN_WIDTH+x] = color;

		SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}	

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

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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
