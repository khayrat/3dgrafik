#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* args[]) {
	SDL_Texture   *display;
  SDL_Window    *window;
  SDL_Renderer  *renderer;
  int           x;
  int           y;
  int           a = 255;
  int           b = 0;

	char* gl_title = "test";
	long gl_width = 640;
	long gl_height = 320;

  window = SDL_CreateWindow(gl_title, SDL_WINDOWPOS_CENTERED,
                            SDL_WINDOWPOS_CENTERED, gl_width, gl_height, 0);
  renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED |
                                           SDL_RENDERER_TARGETTEXTURE |
                                           SDL_RENDERER_PRESENTVSYNC);
  display = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                              SDL_TEXTUREACCESS_TARGET, gl_width, gl_height);
  while (true)
    {
      SDL_SetRenderTarget(renderer, display);
      SDL_SetRenderDrawColor(renderer, a, b, 0, 255);
      for (x = 0; x < gl_width; ++x)
        {
          for (y = 0; y < gl_height; ++y)
            {
              SDL_RenderDrawPoint(renderer, x, y);
            }
        }
      a = a == 255 ? 0 : 255;
      b = b == 255 ? 0 : 255;
      SDL_SetRenderTarget(renderer, NULL);
      SDL_RenderCopy(renderer, display, NULL, NULL);
      SDL_RenderPresent(renderer);
    }
}
