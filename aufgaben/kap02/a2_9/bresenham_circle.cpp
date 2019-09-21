#include "graphic_api.h"

int SCREEN_WIDTH = 640;
int SCREEN_HEIGHT = 480;

void draw_circle(svertex p, long r);


double begin_x = 0;
double end_x = 0;
double begin_y = 0;
double end_y = 0;

int main(int argc, char **argv) {
    createCanvas();

    printf("begin_x: %f, end_x: %f, begin_y: %f, end_y: %f\n", begin_x, end_x, begin_y, end_y);

    while(true) {
        renderToVRAM();

        long r = rand() % 100 + 5;
        long sx, sy;

        // pic cooridantes that fit into the screen
        do {
            sx = rand() % (SCREEN_WIDTH  - 2*r);
            sy = rand() % (SCREEN_HEIGHT - 2*r);
        } while( (sx-r) < 0 || (sx+r) >= SCREEN_WIDTH || (sy-r) < 0 || (sy+r) >= SCREEN_HEIGHT );


        uchar red   = rand() % 155 + 100;
        uchar green = rand() % 155 + 100;
        uchar blue  = rand() % 155 + 100;

        setColor(pixel_32(red, green, blue));

        draw_circle( svertex( sx, sy ), r);

        for( double wait = 0 ; wait < 9999999 ; wait++ ) ;

        renderToScreen();

        SDL_PollEvent(&event);
        if (event.type == SDL_QUIT) break;
    }

    quit();
    return 0;
}

void draw_circle( svertex midpoint, long r) {
    long x = 0, y = -r;
    long e = 1 - r;

    while( x <= -y ) {
        plot_point(svertex(midpoint.sx + x, midpoint.sy + y));
        //screen[ (midpoint.sy + y) * x_res + (midpoint.sx + x) ] = c;

        plot_point(svertex(midpoint.sx - x, midpoint.sy + y));
        //screen[ (midpoint.sy + y) * x_res + (midpoint.sx - x) ] = c;

        plot_point(svertex(midpoint.sx + x, midpoint.sy - y));
        //screen[ (midpoint.sy - y) * x_res + (midpoint.sx + x) ] = c;

        plot_point(svertex(midpoint.sx - x, midpoint.sy - y));
        //screen[ (midpoint.sy - y) * x_res + (midpoint.sx - x) ] = c;

        plot_point(svertex(midpoint.sx + y, midpoint.sy + x));
        //screen[ (midpoint.sy + x) * x_res + (midpoint.sx + y) ] = c;

        plot_point(svertex(midpoint.sx - y, midpoint.sy + x));
        //screen[ (midpoint.sy + x) * x_res + (midpoint.sx - y) ] = c;

        plot_point(svertex(midpoint.sx + y, midpoint.sy - x));
        //screen[ (midpoint.sy - x) * x_res + (midpoint.sx + y) ] = c;

        plot_point(svertex(midpoint.sx - y, midpoint.sy - x));
        //screen[ (midpoint.sy - x) * x_res + (midpoint.sx - y) ] = c;

        if( e > 0 ) {
            e += 2 * y + 2;
            y++;
        }

        e += 2 * x + 3;
        x++;
    }
}
