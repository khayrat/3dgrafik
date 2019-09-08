#include <SDL2/SDL.h>
#include <stdio.h>

#include "palette.h"

pixel_32 my_palette[256];

long offset(int,int);
void load_palette();
void make_uint32(pixel_32, Uint32*);

// Screen dimension constants
const int SCREEN_WIDTH  = 256;
const int SCREEN_HEIGHT = 90;

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
	SDL_Texture * texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, SCREEN_WIDTH, SCREEN_HEIGHT);
	Uint32 *pixels = new Uint32[SCREEN_WIDTH * SCREEN_HEIGHT]; 
	memset(pixels, 255, SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(Uint32));

	load_palette();

	for (long x=0; x<256; x++) {
		pixel_32 p32 = my_palette[x];
		Uint32 pixel;
		make_uint32(p32, &pixel);
		
		for (long i=0, y=0; i < SCREEN_HEIGHT; i++, y += SCREEN_WIDTH) {
			pixels[y+x] = pixel;
			SDL_UpdateTexture(texture, NULL, pixels, SCREEN_WIDTH * sizeof(Uint32));

			SDL_RenderClear(renderer);
			SDL_RenderCopy(renderer, texture, NULL, NULL);
			SDL_RenderPresent(renderer);
		}
	}

	bool leftMouseButtonDown = false;
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

	// Destroy window
	SDL_DestroyWindow(window);

	delete[] pixels;
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);

	// Qutt SDL subsystems
	SDL_Quit();

	return 0;
}

long offset(int y, int x) {
	if (y >= SCREEN_WIDTH || x >= SCREEN_HEIGHT || y < 0 || x < 0) {
		printf("y: %d, or x: %d out of range: (%d/%d)\n", y, x, SCREEN_WIDTH, SCREEN_HEIGHT);
		exit(1);
	}
	return y * SCREEN_WIDTH + x;
}

void load_palette() {
	long x=0; char c=0; 

	for(x=0, c=0; x<64; x++, c += 4) {
			my_palette[x      ] = pixel_32(c, 0, 0);
			my_palette[x + 64 ] = pixel_32(c, c, c);
			my_palette[x + 128] = pixel_32(0, c, 0);
			my_palette[x + 192] = pixel_32(0, 0, c);
	}
}

void make_uint32(pixel_32 pixel, Uint32 *p) {
	  memcpy(p, &pixel, sizeof(pixel_32));
}
