#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

#include "palette.h"

long offset(int,int);
void createWindow();
void cleanup(); 

// Screen dimension constants
const int SCREEN_WIDTH  = 200;
const int SCREEN_HEIGHT = 100;

void displayRandom();

// The windwo we'll be rendering to
SDL_Window* window = NULL;

SDL_Renderer *renderer = NULL;
SDL_Texture *display = NULL;

int main(int argc, char* args[]) {
  createWindow();

  displayRandom();

  cleanup();
  return 0;
}

void displayRandom() {
  SDL_Event event;
  bool quit = false;
  int i=0;
  while (i++<10000) {
    int x = rand() % SCREEN_WIDTH;
    int y = rand() % SCREEN_HEIGHT;
    int entry = 16 + rand() % 16;

    pixel_32 color = palette[entry];

    // set color
    SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, color.alpha);

    // connect renderer with the texture aka VRAM
    SDL_SetRenderTarget(renderer, display);

    // render to VRAM
    SDL_RenderDrawPoint(renderer, x, y);

    // connect renderer with screen
    SDL_SetRenderTarget(renderer, NULL);

    // bring VRAM closer to the screen display
    SDL_RenderCopy(renderer, display, NULL, NULL);

    // draw to screen
    SDL_RenderPresent(renderer);

    while (SDL_PollEvent(&event)) {
      switch (event.type) {
      case SDL_QUIT:
        quit = true;
      }
    }
    if (quit) break;
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

void  cleanup() {
  // Destroy window
  SDL_DestroyWindow(window);
  SDL_DestroyTexture(display);
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
