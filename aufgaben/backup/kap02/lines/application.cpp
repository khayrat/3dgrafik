#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "palette.h"
#include "vertex.h"

long offset(int,int);
void make_uint32(pixel_32, Uint32*);
void createWindow();
void cleanup(); 
void drawline(vertex, vertex, pixel_32);

// Screen dimension constants
const int SCREEN_WIDTH  = 640; // x
const int SCREEN_HEIGHT = 320; // y

// The windwo we'll be rendering to
SDL_Window* window = NULL;

SDL_Texture* display = NULL;
SDL_Renderer* renderer = NULL;

int main(int argc, char* args[]) {
	createWindow();

	vertex begin = vertex(100, 100);
	vertex end = vertex(200, 310);
  pixel_32 color = palette[rand() % 256];

	SDL_Event event;
	bool quit = false;
	for (int i=0; i<10000; i++) {
		drawline(begin, end, color); 
		begin = end;
		end = vertex(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);

		SDL_Delay(2000);

		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
					quit = true;
					printf("exiting...\n");
			}
		}
		if (quit) break;
	}
	if (!quit) {
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

void drawline(vertex begin, vertex end, pixel_32 color) {
	double detail = 0.001;

	int vx = end.x - begin.x;
	int vy = end.y - begin.y;

	/*
	printf("begin      : (%d,%d)\n", begin.x, begin.y);
	printf("end        : (%d,%d)\n", end.x, end.y);
	printf("(being-end): (%d,%d)\n", vx, vy);
	*/

	// set color
	SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, color.alpha);

	// connect renderer with the texture aka VRAM
	SDL_SetRenderTarget(renderer, display);

	for (double t=0; t <= 1; t += detail) {
		ushort x = begin.x + t * vx;
		ushort y = begin.y + t * vy;
		// render to texture aka VRAM
		SDL_RenderDrawPoint(renderer, x, y);
	}

	// connect renderer with screen
	SDL_SetRenderTarget(renderer, NULL);

	// bring VRAM closer to the screen display
	SDL_RenderCopy(renderer, display, NULL, NULL);

	// draw to the screen
	SDL_RenderPresent(renderer);
}
