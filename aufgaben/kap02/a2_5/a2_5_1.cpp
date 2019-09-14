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
void setColor(pixel_32 color);
void renderToVRAM();
void renderToScreen();
void quit();

void get_point(vertex a, vertex b, double delta, vertex* point);
void draw_line(vertex begin, vertex end);
void plot_point(vertex);

int main(int argc, char **argv) {
    createCanvas();

    pixel_32 color = pixel_32(150, 150, 150);
    setColor(color);

    // set initial start and end point
    vertex start_point = vertex(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);
    vertex end_point   = vertex(rand() % SCREEN_WIDTH, rand() % SCREEN_HEIGHT);

    while(true) {
        printf("#############\n");
        printf("start_point: (%d, %d)\n", start_point.x, start_point.y);
        printf("end_point  : (%d, %d)\n", end_point.x, end_point.y);
        printf("#############\n");

        draw_line(start_point, end_point);

        // old end point becomes new start point
        start_point.x = end_point.x;
        start_point.y = end_point.y;

        // choose new end point
        end_point.x = rand() % SCREEN_WIDTH;
        end_point.y = rand() % SCREEN_HEIGHT;

        if (event.type == SDL_QUIT) break;

        // wait a few seconds
        SDL_Delay(200);
    }

    quit();
    return 0;
}

void get_point(vertex a, vertex b, double delta, vertex* p) {
    int v_x = b.x - a.x;
    int v_y = b.y - a.y;
    p->x = a.x + delta * (float) v_x;
    p->y = a.y + delta * (float) v_y;

    printf("(a: (%d, %d), b: (%d, %d), delta: %f -> (%d, %d)\n", a.x, a.y, b.x, b.y, delta, p->x, p->y);
}

void draw_line(vertex begin, vertex end) {
    vertex point_holder;
    double detail_level = 0.001;

    renderToVRAM();

    for (double delta = 0; delta < 1; delta += detail_level) {
        get_point(begin, end, delta, &point_holder);
        plot_point(point_holder);

        // get event
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) break;
    }

    renderToScreen();
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

void setColor(pixel_32 color) {
    SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, color.alpha);
}

void renderToVRAM() {
    // render to texture (VRAM)
    printf("renderToVRAM...\n");
    SDL_SetRenderTarget(renderer, texture);
}

void renderToScreen() {
    printf("renderToScreen...\n");
    // render to screen
    SDL_SetRenderTarget(renderer, NULL);

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}
