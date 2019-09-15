#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

#include "palette.h"

SDL_Event event;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;

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
vertex project(double x, double y);
void  test_projection();

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int begin_x;
extern int end_x;
extern int begin_y;
extern int end_y;

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
        printf("error initializing SDL: %s\n", SDL_GetError());
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

void plot_point(vertex p) {
    if (p.x <= SCREEN_WIDTH && p.y <= SCREEN_HEIGHT) {
        SDL_RenderDrawPoint(renderer, p.x, p.y);
    } else {
        printf("skip rendering out of screen (%d, %d)\n", p.x, p.y);
    }
}

void draw_line(vertex begin, vertex end) {
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

        plot_point(vertex(x, y));

        // get event
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) break;
    }

    renderToScreen();
}

vertex project(double x, double y) {
    int sx, sy;
    sx = int((x - begin_x) * SCREEN_WIDTH  / (end_x - begin_x));
    sy = int((y - end_y)   * SCREEN_HEIGHT / (begin_y - end_y));

    printf("project: (%f,%f) -> (%d,%d)\n", x, y, sx, sy);
    return vertex(sx, sy);
}

void test_projection() {
	int begin_x = -100; int end_x=100;
	int begin_y = -100; int end_y=100;
	//int x = 5, y = 10;
	//int x = 0, y = 0;
	int x = 1, y = 1;
	vertex screen_coord = project(x, y);
	printf("mathematical coord: begin_x: %d, end_x: %d, begin_y: %d, end_y: %d\n", begin_x, end_x, begin_y, end_y);
	printf("screen coord: begin_x: %d, end_x: %d, begin_y: %d, end_y: %d\n", 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
	printf("mathematical coord: (%d,%d), screen coord: (%d,%d)\n", x,y,screen_coord.x, screen_coord.y);
	printf("x-unit: %f, y-unit: %f\n", (1.)*SCREEN_WIDTH/(end_x-begin_x), (1.)*SCREEN_HEIGHT/(end_y-begin_y));
}

