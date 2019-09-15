#include "graphic_api.h"

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

// mathematical frame
int begin_x = -9;
int end_x   = 14;
int begin_y = -3;
int end_y   = 4;

// coefficients
double a = 0.001;
double b = -0.0093;
double c = -0.06;
double d = 0.464;

double f(double x);
double df(double x);
double ddf(double x);
void plot_functions();
void plot_coordinates();

int main(int argc, char **argv) {
    createCanvas();

    //test_projection();

    printf("begin_x: %d, end_x: %d, begin_y: %d, end_y: %d\n", begin_x, end_x, begin_y, end_y);
    printf("a: %f, b: %f, c: %f, d: %f\n", a, b, c, d);

    plot_coordinates();
    plot_functions();

    while(true) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) break;
    }

    quit();
    return 0;
}

void plot_functions() {
    double inc = 0.01;

    renderToVRAM();

    for (double x = begin_x; x < end_x; x += inc) {
        // red
        setColor(pixel_32(255, 0, 0));
        plot_point(project(x, f(x)));

        // yellow
        setColor(pixel_32(255, 0, 255));
        plot_point(project(x, df(x)));

        // green
        setColor(pixel_32(0, 255, 0));
        plot_point(project(x, ddf(x)));
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

double f(double x) {
    return a*pow(x, 4) + b*pow(x, 3) + c*pow(x, 2) + d*x;
}

double df(double x) {
    return 4*a*pow(x, 3) + 3*b*pow(x, 2) + 2*c*x + d;
}

double ddf(double x) {
    return 3*4*a*pow(x, 2) + 2*3*b*x + 2*c;
}
