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

void draw_line(vertex begin, vertex end);
void plot_point(vertex);
vertex project(double x, double y);

void  test_projection();

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

void plot_point(vertex p) {
    if (p.x <= SCREEN_WIDTH && p.y <= SCREEN_HEIGHT) {
        SDL_RenderDrawPoint(renderer, p.x, p.y);
    } else {
        printf("skip rendering out of screen (%d, %d)\n", p.x, p.y);
    }
}

vertex project(double x, double y) {
    int sx, sy;
    sx = int((x - begin_x) * SCREEN_WIDTH  / (end_x - begin_x));
    sy = int((y - end_y)   * SCREEN_HEIGHT / (begin_y - end_y));

    printf("project: (%f,%f) -> (%d,%d)\n", x, y, sx, sy);
    return vertex(sx, sy);
}

void test_projection() {
	begin_x = -100; end_x=100;
	begin_y = -100; end_y=100;
	//int x = 5, y = 10;
	//int x = 0, y = 0;
	int x = 1, y = 1;
	vertex screen_coord = project(x, y);
	printf("mathematical coord: begin_x: %d, end_x: %d, begin_y: %d, end_y: %d\n", begin_x, end_x, begin_y, end_y);
	printf("screen coord: begin_x: %d, end_x: %d, begin_y: %d, end_y: %d\n", 0, SCREEN_WIDTH, 0, SCREEN_HEIGHT);
	printf("mathematical coord: (%d,%d), screen coord: (%d,%d)\n", x,y,screen_coord.x, screen_coord.y);
	printf("x-unit: %f, y-unit: %f\n", (1.)*SCREEN_WIDTH/(end_x-begin_x), (1.)*SCREEN_HEIGHT/(end_y-begin_y));
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
