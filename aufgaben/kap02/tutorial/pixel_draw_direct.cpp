#include <SDL2/SDL.h>

int main(int argc, char ** argv) {
    bool quit = false;
    bool leftMouseButtonDown = false;
    SDL_Event event;

    // initialize the graphics system
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("error intializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    // create a window on the screen
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

    // create a renderer, which sets up the graphics hardware
    Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, render_flags);

    if (!renderer) {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // texture corresponds to image data loaded into the graphic card memory (VRAM)
    SDL_Texture *texture = SDL_CreateTexture(
            renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_TARGET,
            //SDL_TEXTUREACCESS_STATIC,
            640, 480);

    if (!texture) {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // clear window white
    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0x00);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    // set color to black
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);

    while (!quit) {
        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                quit = true;
                break;
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT)
                    leftMouseButtonDown = false;
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                    leftMouseButtonDown = true;
            case SDL_MOUSEMOTION:
                if (leftMouseButtonDown) {
                    int mouseX = event.motion.x;
                    int mouseY = event.motion.y;
                    SDL_SetRenderTarget(renderer, texture);
                    SDL_RenderDrawPoint(renderer, mouseX, mouseY);
                }
                break;
        }

        SDL_SetRenderTarget(renderer, NULL);
        // double buffering:
        // write to back buffer
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        // switch to for- and background buffer
        SDL_RenderPresent(renderer);
    }

    // cleanup resources before exiting
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
