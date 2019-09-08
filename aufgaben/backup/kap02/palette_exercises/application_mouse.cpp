#include <SDL2/SDL.h>
#include <stdio.h>

#include "palette.h"

// Screen dimension constants
const int SCREEN_WIDTH  = 640;
const int SCREEN_HEIGHT = 480;

int main(int argc, char* args[]) {
	// The windwo we'll be rendering to
	SDL_Window* window = NULL;

	// The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize SDL_Error: %s\n", SDL_GetError());
		return 1;
	}

	// Create Widonw
	window = SDL_CreateWindow(
			"Palette",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH,
			SCREEN_HEIGHT,
			SDL_WINDOW_SHOWN);

	if (window == NULL) {
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return 1;
	}

	SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Texture * texture = SDL_CreateTexture(renderer,
	SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
	Uint32 * pixels = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT]; 
	memset(pixels, 255, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));

	bool leftMouseButtonDown = false;
	bool quit = false;
	SDL_Event event;
	while (!quit) {
		SDL_UpdateTexture(texture, NULL, pixels, 640 * sizeof(Uint32));
		SDL_WaitEvent(&event);

		switch (event.type)
		{
		case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
						leftMouseButtonDown = false;
				break;
		case SDL_MOUSEBUTTONDOWN:
				if (event.button.button == SDL_BUTTON_LEFT)
						leftMouseButtonDown = true;
		case SDL_MOUSEMOTION:
				if (leftMouseButtonDown)
				{
						int mouseX = event.motion.x;
						int mouseY = event.motion.y;
						pixels[mouseY * 640 + mouseX] = 0;
				}
				break;
		case SDL_QUIT:
				quit = true;
				break;
		}

		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, texture, NULL, NULL);
		SDL_RenderPresent(renderer);
	}

	// Destroy window
	SDL_DestroyWindow(window);

	delete[] pixels;
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);

	// Qutt SDL subsystems
	SDL_Quit();

	return 0;
}
