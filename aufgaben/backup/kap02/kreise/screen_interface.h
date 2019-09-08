#ifndef SCREEN_INTERACE_H
#define SCREEN_INTERACE_H

#include <SDL2/SDL.h>
#include <stdio.h>

void createWindow();
void cleanup();
bool stop();

// Screen dimension constants
long x_res = 0;
long y_res = 0;

// The windwo we'll be rendering to
SDL_Window* window = NULL;

// VRAM Buffer
SDL_Texture* display = NULL;

// I the renderer
SDL_Renderer* renderer = NULL;

bool stop() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
				printf("exiting...\n");
				return true;
		}
	}
	return false;
}

void createWindow(long _x_res, long _y_res) {
	x_res = _x_res;
	y_res = _y_res;

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
			x_res,
			y_res,
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
																				x_res,
																				y_res);

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

#endif
