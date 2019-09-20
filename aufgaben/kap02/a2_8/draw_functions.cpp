#include "graphic_api.h"

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

// mathematical frame
const double pi = 3.1415926535;
int begin_x = -100, end_x   = 100;
int begin_y = -100,    end_y   = 100;
int mx = 0, my = 0;
int a = 3,  b = 2;

vertex e(double x);
void plot_functions();
void plot_function(vertex (*f) (double x), pixel_32 color);
void plot_coordinates();
void plot_units();
void plot_arrows();
double get_inc();

int main(int argc, char **argv) {
    createCanvas();

    //test_projection();

    printf("begin_x: %d, end_x: %d, begin_y: %d, end_y: %d\n", begin_x, end_x, begin_y, end_y);

    plot_coordinates();
    plot_units();
    plot_arrows();
    plot_functions();

    while(true) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) break;
    }

    quit();
    return 0;
}

void plot_functions() {
    plot_function(e,  pixel_32(255, 255, 0  )); // blue
}

void plot_function(vertex (*f) (double x), pixel_32 color) {
    double step = get_inc();

    setColor(color);

    renderToVRAM();

    for (double x = begin_x + step; x <= end_x; x += step) {
        vertex r2 = e(x);
        plot_point(project(r2.x, r2.y));
    }

    renderToScreen();
}

void plot_coordinates() {
    setColor(pixel_32(150, 150, 150));
    // x-coordinate
    draw_line(project(begin_x, 0), project(end_x, 0));
    // y-coordinate
    draw_line(project(0, begin_y), project(0, end_y));
}

void plot_units() {
  for (int x=begin_x; x < end_x; x++) {
    vertex x_base = project(x,0);
        draw_line(vertex(x_base.x, x_base.y - 3), vertex(x_base.x, x_base.y + 3));
  }
  for (int y=begin_y; y<end_y; y++) {
    vertex y_base = project(0, y);
        draw_line(vertex(y_base.x - 3, y_base.y), vertex(y_base.x + 3, y_base.y));
  }
}

void plot_arrows() {
    // x arrow
    vertex x_top = project(end_x, 0);
    vertex x_bottom = project(end_x, 0);
    vertex x_right = project(end_x, 0);

    x_top.x -= 11; x_top.y += 4;
    x_bottom.x -= 11; x_bottom.y -= 4;

    draw_line(x_top, x_right);
    draw_line(x_bottom, x_right);

    // y arrow
    vertex y_left = project(0, end_y);
    vertex y_right = project(0, end_y);
    vertex y_top = project(0, end_y);

    y_left.x -= 4; y_left.y += 11;
    y_right.x += 4; y_right.y += 11;

    draw_line(y_left, y_top);
    draw_line(y_right, y_top);
}

vertex e(double alpha) {
    return vertex(
        b * cos(alpha) + mx,
        a * sin(alpha) + my
    );
}

double get_inc() {
    return 1.0*(end_x - begin_x) / SCREEN_WIDTH;
}
