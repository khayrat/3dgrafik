#include <SDL2/SDL.h>

#include "simple_types.h"

#define WIN_HEIGHT 480
#define WIN_WIDTH 640

pixel_32 *make_palette();

int main(int argc, char **argv)
{
    bool quit = false;
    SDL_Event event;
    pixel_32 *palette = make_palette();

    // initialize the graphics system
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("error intializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    // create a window on the screen
    SDL_Window *window = SDL_CreateWindow(
        "Color Palette",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WIN_WIDTH, WIN_HEIGHT, 0);

    if (!window)
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // create a renderer, which sets up the graphics hardware
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, render_flags);

    if (!renderer)
    {
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

    if (!texture)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // top left coordinate when draw-rect position is the center
    int x = (WIN_WIDTH - 256) / 2;
    int y = (WIN_HEIGHT - 200) / 2;

    // render to texture (VRAM)
    SDL_SetRenderTarget(renderer, texture);

    // render loop
    for (int i = 0, xx = x; i < 256; i++, xx++)
    {
        SDL_SetRenderDrawColor(renderer, palette[i].red, palette[i].green, palette[i].blue, palette[i].alpha);
        for (int yy = y; yy < y + 200; yy++)
        {
            SDL_RenderDrawPoint(renderer, xx, yy);
        }
    }

    // render to the window
    SDL_SetRenderTarget(renderer, NULL);

    // double buffering: draw to window
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    // wait loop
    while (!quit)
    {
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT)
            quit = true;
    }

    // cleanup resources before exiting
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

pixel_32 *make_palette()
{
    static pixel_32 palette[256];
    int part_size = 256 / 4;
    int red_offset = 0 * part_size;
    int white_offset = 1 * part_size;
    int green_offset = 2 * part_size;
    int blue_offset = 3 * part_size;
    int c = 0; // to fit the 256 sized color-run into size of the part

    for (int i = 0; i < part_size; i++, c += 4)
    {
        palette[red_offset + i] = pixel_32(c, 0, 0);   // black-red
        palette[white_offset + i] = pixel_32(c, c, c); // black-white
        palette[green_offset + i] = pixel_32(0, c, 0); // black-green
        palette[blue_offset + i] = pixel_32(0, 0, c);  // black-blue
    }

    return palette;
}
