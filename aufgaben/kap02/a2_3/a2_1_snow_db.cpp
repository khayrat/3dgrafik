#include <SDL2/SDL.h>
#include "palette.h"

#define WIN_HEIGHT 480
#define WIN_WIDTH  640

int main(int argc, char **argv) {
    SDL_Event event;

    // initialize the graphics system
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("error intializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    // create a window on the screen
    SDL_Window *window = SDL_CreateWindow(
        "Color Palette",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIN_WIDTH, WIN_HEIGHT, 0);

    if (!window) {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // create a renderer, which sets up the graphics hardware
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC;
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
            WIN_WIDTH, WIN_HEIGHT);

    if (!texture) {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
  
    // render to texture (VRAM)
    SDL_SetRenderTarget(renderer, texture);

    // top left coordinate when draw-rect position is the center
    int x = (WIN_WIDTH - 256) / 2;
    int y = (WIN_HEIGHT - 200) / 2;

    // render loop
    for (int i=0; i<10000; i++) {
      int xx = rand() % 256 + x; // force x-coordinate into rect-range
      int yy = rand() % 200 + y; // force y-coordinate into rect-range
      int ii = rand() % 16 + 16; // force color into grey-range

      // select color to render
      SDL_SetRenderDrawColor(renderer, palette[ii].red, palette[ii].green, palette[ii].blue, palette[ii].alpha);

      // render point to VRAM
      SDL_RenderDrawPoint(renderer, xx, yy);
    }

    // render to the window
    SDL_SetRenderTarget(renderer, NULL);
    // double buffering: draw to window
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
    
    // wait for first event and quit
    while (SDL_WaitEvent(&event)) break;

    // cleanup resources before exiting
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
