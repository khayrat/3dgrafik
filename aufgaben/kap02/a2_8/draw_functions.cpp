#include "graphic_api.h"

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

// mathematical frame
const double pi = 3.1415926535;
double begin_x = -4*pi, end_x   = 4*pi;
//double begin_y = -5,    end_y   = 5;
// makes y- and x-units equal sized (no stratching of the figures):
double begin_y = -5, end_y = ((end_x - begin_x) * SCREEN_HEIGHT) / SCREEN_WIDTH + begin_y;
double mx = 0, my = 0;
double a = 5,  b = 1, c = 3;

vertex e(double x);
vertex k(double x);
vertex g(double x);
void draw_xy();
void draw_functions();
void draw_function(vertex (*f) (double x), double start, double end, pixel_32 color);
double get_inc();

int main(int argc, char **argv) {
    createCanvas();

    //test_projection();

    printf("begin_x: %f, end_x: %f, begin_y: %f, end_y: %f\n", begin_x, end_x, begin_y, end_y);

    setColor(pixel_32(150, 150, 150));

    renderToVRAM();

    draw_xy();
    draw_functions();

    renderToScreen();

    while(true) {
        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) break;
    }

    quit();
    return 0;
}

void draw_functions() {
    draw_function(e, 0, 2*pi,  pixel_32(255, 255,   0)); // yellow
    draw_function(k, 0, 2*pi,  pixel_32(255,   0,   0)); // red
    draw_function(g, 0, 10,    pixel_32(  0, 255, 255)); // blue
}

void draw_function(vertex (*f) (double x), double start, double end, pixel_32 color) {
    double step = get_inc();
    svertex p = project(f(start));

    setColor(color);

    for (double a = start + step; a <= end; a += step) {
        svertex q = project(f(a));
        draw_line(p, q);;
        p = q;
    }
}

vertex e(double alpha) {
    double wx = a * cos(alpha) + mx;
    double wy = b * sin(alpha) + my;
    return vertex(wx, wy);
}

vertex k(double alpha) {
    double wx = c * cos(alpha) + mx;
    double wy = c * sin(alpha) + my;
    return vertex(wx, wy);
}

vertex g(double alpha) {
    double wx = -5 * alpha * 1;
    double wy = -5 * alpha * 1;
    return vertex(wx, wy);
}

double get_inc() {
    return 1.0*(end_x - begin_x) / SCREEN_WIDTH;
}

void draw_xy() {
    // x/y axis
    draw_line( project( begin_x, 0 ), project( end_x, 0 ));
    draw_line( project( 0, begin_y ), project( 0, end_y ));

    // arrows
    svertex right = project( end_x, 0 );
    svertex top = project( 0, end_y );

    draw_line( svertex( right.sx-11, right.sy-5 ), right);
    draw_line( svertex( right.sx-11, right.sy+5 ), right);
    draw_line( svertex( top.sx-5, top.sy+11 ), top);
    draw_line( svertex( top.sx+5, top.sy+11 ), top);

    // units
    printf("begin_x: %d, end_x: %d\n", int(begin_x), int(end_x));
    for( int x = int( begin_x ) ; x <= int( end_x ) ; x++ ) {
        printf("x: %d\n", int(x));
        svertex p = project( x, 0 );

        if( p.sx < right.sx - 11 )
          draw_line( svertex( p.sx, p.sy+3 ), svertex( p.sx, p.sy-3 ));
    }

    for( int y = int( begin_y ) ; y <= int( end_y ) ; y++ ) {
        svertex p = project( 0, y );

        if( p.sy > top.sy + 11 )
          draw_line( svertex( p.sx-3, p.sy ), svertex( p.sx+3, p.sy ));
    }
}
