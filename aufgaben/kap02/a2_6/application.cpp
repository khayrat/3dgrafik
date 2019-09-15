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
        SDL_Delay(000);
    }

    quit();
    return 0;
}

void draw_line(vertex begin, vertex end) {
    vertex point = vertex();
    int x = begin.x;
    int y = begin.y;
    int x_step = 1;
    int y_step = 1;
    int delta_x = end.x - begin.x;
    int delta_y = end.y - begin.y;
    printf("begin: (%d, %d), end: (%d, %d), delta_x: %d, delta_y: %d\n",
        begin.x, begin.y, end.x, end.y, delta_x, delta_y);

    // case end point is right of start point
    if (delta_x < 0) {
        delta_x = -delta_x;
        x_step  = -1;
    }

    // falling line
    if (delta_y < 0) {
        delta_y = -delta_y;
        y_step  = -y_step;
    }

    // fast increasing line
    bool fast_increasing = false;
    if (delta_y > delta_x) {
        // make it to slow increasing line by switching deltas
        fast_increasing = true;
    }

    int e = 0; // our nominator (t*delta_y)
    int steps = 0;
    if (fast_increasing) {
        steps = delta_y;
    } else {
        steps = delta_x;
    }


    renderToVRAM();

    printf("steps: %d, delta_x: %d, delta_y: %d\n", steps, delta_x, delta_y);
    plot_point(begin);
    for (int i = 0; i < steps; i++) {
        if (fast_increasing) {
            y += y_step;
            e += delta_x; // corresponds to (t+1)*delta_y

            // are we still in the same pixel-row?
            if (e >= delta_y) {
                e -= delta_y;
                x += x_step;
            }
        } else {
            x += x_step;
            e += delta_y; // corresponds to (t+1)*delta_y

            // are we still in the same pixel-row?
            if (e >= delta_x) {
                e -= delta_x;
                y += y_step;
            }
        }

        point.x = x; point.y = y;
        plot_point(point);

        // get event
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) break;
    }

    renderToScreen();
}

void plot_point(vertex p) {
    printf("plotting (%d, %d)\n", p.x, p.y);
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
