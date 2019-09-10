#include <SDL2/SDL.h>

#include "palette.h"

int main(int argc, char **argv) {
    bool quit = false;
    SDL_Event event;

    // initialize the graphics system
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("error intializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    // create a window on the screen
    SDL_Window *window = SDL_CreateWindow(
        "Empty Window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        256, 480, 0);

    if (!window) {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // create a renderer, which sets up the graphics hardware
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, render_flags);

    if (!renderer) {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // texture corresponds to image data loaded into the graphic card memory (VRAM)
    SDL_Texture *texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_TARGET,
            //SDL_TEXTUREACCESS_STATIC,
            256, 480);

    if (!texture) {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
  
    // render to texture (VRAM)
    SDL_SetRenderTarget(renderer, texture);

    // render loop
    for (int x=0; x<256; x++) {
      SDL_SetRenderDrawColor(renderer, palette[x].red, palette[x].green, palette[x].blue, palette[x].alpha);
      for (int y=0; y<479; y++) {
          SDL_RenderDrawPoint(renderer, x, y);
      }
    }

    SDL_SetRenderTarget(renderer, NULL);
    // double buffering:
    // write to back buffer
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    // switch to for- and background buffer
    SDL_RenderPresent(renderer);
   
    // wait loop
    while (!quit) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT) quit = true;
    }

    // cleanup resources before exiting
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
