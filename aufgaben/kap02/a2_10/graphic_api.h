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

void draw_line(svertex begin, svertex end);
void plot_point(svertex);
svertex project(vertex p);
svertex project( double x, double y ) {  return project(vertex(x, y));  }
void  test_projection();

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern double begin_x;
extern double end_x;
extern double begin_y;
extern double end_y;

void createWindow() {
    // create a window on the screen
    window = SDL_CreateWindow(
        "OpenGL",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH,
        SCREEN_HEIGHT,
        SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);

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

void plot_point(svertex p) {
    if (p.sx <= SCREEN_WIDTH && p.sy <= SCREEN_HEIGHT) {
        SDL_RenderDrawPoint(renderer, p.sx, p.sy);
    } else {
        printf("skip rendering out of screen (%d, %d)\n", p.sx, p.sy);
    }
}

void draw_line(svertex begin, svertex end) {
    int x = begin.sx;
    int y = begin.sy;
    int x_step = 1;
    int y_step = 1;
    int delta_x = end.sx - begin.sx;
    int delta_y = end.sy - begin.sy;
    printf("begin: (%d, %d), end: (%d, %d), delta_x: %d, delta_y: %d\n",
        begin.sx, begin.sy, end.sx, end.sy, delta_x, delta_y);

    // case end point is right of start point
    if (delta_x < 0) {
        delta_x = -delta_x;
        x_step  = -x_step;
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
        printf("fast_increasing line\n");
        steps = delta_y;
    } else {
        printf("slow_increasing line\n");
        steps = delta_x;
    }

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
        } else { // slow_increasing
            x += x_step;
            e += delta_y; // corresponds to (t+1)*delta_y

            // are we still in the same pixel-row?
            if (e >= delta_x) {
                e -= delta_x;
                y += y_step;
            }
        }

        plot_point(svertex(x, y));

        // get event
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) break;
    }
}

svertex project(vertex p) {
    int sx, sy;
    sx = int((p.x - begin_x) * (double (SCREEN_WIDTH  - 1) / (end_x - begin_x)));
    sy = int((p.y - end_y)   * (double (SCREEN_HEIGHT - 1) / (begin_y - end_y)));

    printf("project: (%f, %f) -> (%d, %d)\n", p.x, p.y, sx, sy);
    return svertex(sx, sy);
}

void test_projection() {
  double begin_x = -100, end_x=100;
  double begin_y = -100, end_y=100;
  //double x = 5, y = 10;
  double x = 0, y = 0;
  //double x = 1, y = 1;
  svertex screen_coord = project(x, y);
  printf("mathematical coord: begin_x: %f, end_x: %f, begin_y: %f, end_y: %f\n", begin_x, end_x, begin_y, end_y);
  printf("screen coord: begin_x: %d, end_x: %d, begin_y: %d, end_y: %d\n", 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
  printf("mathematical coord: (%f,%f), screen coord: (%d,%d)\n", x, y, screen_coord.sx, screen_coord.sy);
  printf("x-unit: %f, y-unit: %f\n", (1.)*SCREEN_WIDTH/(end_x-begin_x), (1.)*SCREEN_HEIGHT/(end_y-begin_y));
}

