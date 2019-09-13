#include <SDL2/SDL.h>

#include "palette.h"

SDL_Event event;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;

void init();
void createWindow();
void createRenderer();
void createTexture();
void quit();

void find_midpoint(vertex act_point, vertex other_point, vertex* midpoint);
void plot_point(vertex);

int main(int argc, char **argv) {
    init();
    createWindow();
    createRenderer();
    createTexture();

    SDL_SetRenderDrawColor(renderer, palette[15].red, palette[15].green, palette[15].blue, palette[15].alpha);

    // define corners
    vertex D[3] = {
        vertex (95, 435),
        vertex (320, 45),
        vertex (545, 435)
    };

    vertex m = vertex();

    // 0 assign random point to act_point
    int i = rand() % 3;
    printf("first point: %d\n", i);
    vertex act_point = D[i];

    for (int j=0; j<10000; j++) {
        // choose another point
        int ii = rand() % 3;
        vertex other_point = D[ii];

        // find midpoint
        find_midpoint(act_point, other_point, &m);

        // plot m
        plot_point(m);

        act_point = m;

        // get event loop
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) {
            quit();
            return 0;
        }
    }

    while(1) {
        SDL_WaitEvent(&event);
        if (event.type == SDL_QUIT) break;
    }

    quit();
    return 0;
}

void find_midpoint(vertex a, vertex b, vertex *midpoint) {
    if (a.x > b.x) {
        // switch
        vertex tmp = a;
        a = b;
        b = tmp;
    }
    uint delta_x = b.x - a.x;
    uint delta_y = b.y - a.y;

    midpoint->x = a.x + (int) delta_x / 2;
    midpoint->y = a.y + (int) delta_y / 2;
}

void plot_point(vertex p) {
    // render to texture (VRAM)
    SDL_SetRenderTarget(renderer, texture);

    SDL_RenderDrawPoint(renderer, p.x, p.y);

    SDL_SetRenderTarget(renderer, NULL);

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void createWindow() {
    // create a window on the screen
    window = SDL_CreateWindow(
        "Color Palette",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640, 480, 0);

    if (!window) {
        printf("error creating window: %s\n", SDL_GetError());
        quit();
        exit(1);
    }
}

void createRenderer() {
    // create a renderer, which sets up the graphics hardware
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    renderer = SDL_CreateRenderer(window, -1, render_flags);

    if (!renderer) {
        printf("error creating renderer: %s\n", SDL_GetError());
        quit();
        exit(1);
    }
}

void createTexture() {
    // texture corresponds to image data loaded into the graphic card memory (VRAM)
    texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_TARGET,
            //SDL_TEXTUREACCESS_STATIC,
            640, 480);

    if (!texture) {
        printf("error creating renderer: %s\n", SDL_GetError());
        quit();
        exit(1);
    }
}

void init() {
    // initialize the graphics system
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("error intializing SDL: %s\n", SDL_GetError());
        exit(1);
    }
}

void quit() {
    // cleanup resources before exiting
    if (texture) SDL_DestroyTexture(texture);
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}
