#include "graphic_api.h"

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

// mathematical frame
const double pi = 3.1415926535;
double begin_x = -30, end_x   = 30;
double begin_y = -23,    end_y   = 23;
double mx = 0, my = 0;
double r = 1;

vertex s1(double x);
vertex s2(double x);
vertex s3(double x);
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
    draw_function(s1, 0, 50,  pixel_32(255, 255,   0)); // yellow
    draw_function(s2, 0, 50,  pixel_32(255,   0,   0)); // red
    draw_function(s3, 0, 50,  pixel_32(0,   0,   255)); // blue
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

vertex s1(double alpha) {
    double wx = 0.5 * alpha * r * cos(alpha) + mx;
    double wy = 0.4 * alpha * r * sin(alpha) + my;
    return vertex(wx, wy);
}

vertex s2(double alpha) {
    double wx = -0.5 * alpha * r * cos(alpha) + mx;
    double wy = -0.2 * alpha * r * sin(alpha) + my;
    return vertex(wx, wy);
}

vertex s3(double alpha) {
    double wx = 0.2 * alpha * r * cos(alpha) + mx;
    double wy = 0.2 * alpha * r * sin(alpha) + my;
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
