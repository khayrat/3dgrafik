#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include "palette.h"

SDL_Event event;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

void init();
void createWindow();
void createRenderer();
void createTexture();
void createCanvas();
void quit();

void create_random_corners(vertex points[]);
void find_midpoint(vertex act_point, vertex other_point, vertex* midpoint);
void plot_point(vertex);
void render_chaos(vertex points[]);
void delete_chaos();

int main(int argc, char **argv) {
    createCanvas();

    // corners holder
    vertex D[3] = {
        vertex (),
        vertex (),
        vertex ()
    };

    int i = 0;
    while(true) {
        create_random_corners(D);
        render_chaos(D);

        if (event.type == SDL_QUIT) break;

        // clear chaos after 4 renderings
        if (i++ == 3) {
            // wait a few seconds
            SDL_Delay(2000);
            delete_chaos();
            i = 0;
        }
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
    SDL_RenderDrawPoint(renderer, p.x, p.y);
}

void createWindow() {
    // create a window on the screen
    window = SDL_CreateWindow(
        "Chaos Game",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        0);

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
            SCREEN_WIDTH,
            SCREEN_HEIGHT);

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

void createCanvas() {
    init();
    createWindow();
    createRenderer();
    createTexture();
}

void create_random_corners(vertex points[]) {
    for (int i=0; i<3; i++) {
        points[i].x = rand() % SCREEN_WIDTH;
        points[i].y = rand() % SCREEN_HEIGHT;
    }
}

void render_chaos(vertex D[]) {
    // set render color
    SDL_SetRenderDrawColor(renderer, palette[15].red, palette[15].green, palette[15].blue, palette[15].alpha);

    // render to texture (VRAM)
    SDL_SetRenderTarget(renderer, texture);

    // midpoint holder
    vertex m = vertex();

    // step 0: assign random point to act_point
    vertex act_point = D[rand() % 3];

    for (int j=0; j<100000; j++) {
        // 1 step: choose another point, find midpoint and draw, reassign act_point

        // choose another point
        vertex other_point = D[rand() % 3];

        // find midpoint
        find_midpoint(act_point, other_point, &m);

        // plot m
        plot_point(m);

        // m becomes new act_point
        act_point = m;

        // 2 step: repeat

        // get event
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) break;
    }

    SDL_SetRenderTarget(renderer, NULL);

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

void delete_chaos() {
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
