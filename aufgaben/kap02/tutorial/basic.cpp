#include <SDL2/SDL.h>

int main(int argc, char ** argv)
{
    bool quit = false;
    SDL_Event event;

    // initialize the graphics system
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("error intializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow(
        "Empty Window",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640, 480, 0);

    if (!window) {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    while (!quit) {
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
        }
    }

    // cleanup resources before exiting
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}