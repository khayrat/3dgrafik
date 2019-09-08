#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "screen_interface.h"
#include "palette.h"
#include "svertex.h"

void draw_circle(svertex midpoint, long r, pixel_32 color);

int main(int argc, char* args[]) {
	createWindow(640, 480);

	while (true) {
		if (stop()) break;

		long r = rand() % 100 + 5;
		long sx, sy;

		do {
			sx = rand() % (x_res - 2*r);
			sy = rand() % (y_res - 2*r);
		} while ((sx-r)<0 || (sx+r)>=x_res || (sy-r)<0 || (sy+r) >=y_res);

		pixel_32 color = palette[rand() % 256];

		draw_circle(svertex(sx, sy), r, color);

		//SDL_Delay(500);
	}

	cleanup();
	return 0;
}

void draw_circle(svertex midpoint, long r, pixel_32 color) {
	long x = 0, y = -r;
	long e = 1 - r;

	// initialize vram drawing
	// set color
	SDL_SetRenderDrawColor(renderer, color.red, color.green, color.blue, color.alpha);	

	// connect renderer with the texture aka VRAM
	SDL_SetRenderTarget(renderer, display);

	while(x <= -y) {
		SDL_RenderDrawPoint(renderer, midpoint.x + x, midpoint.y + y);
		SDL_RenderDrawPoint(renderer, midpoint.x - x, midpoint.y + y);
		SDL_RenderDrawPoint(renderer, midpoint.x + x, midpoint.y - y);
		SDL_RenderDrawPoint(renderer, midpoint.x - x, midpoint.y - y);

		SDL_RenderDrawPoint(renderer, midpoint.x + y, midpoint.y + x);
		SDL_RenderDrawPoint(renderer, midpoint.x - y, midpoint.y + x);
		SDL_RenderDrawPoint(renderer, midpoint.x + y, midpoint.y - x);
		SDL_RenderDrawPoint(renderer, midpoint.x - y, midpoint.y - x);

		if (e>0) {
			e += 2*y + 2;
			y++;
		}
		e += 2*x + 3;
		x++;
	}

	// draw vram to screen
	// connect renderer with screen
	SDL_SetRenderTarget(renderer, NULL);

	// bring VRAM closer to the screen display
	SDL_RenderCopy(renderer, display, NULL, NULL);

	// draw to the screen
	SDL_RenderPresent(renderer);
}
