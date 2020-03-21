#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "field.hpp"
#include "consts.hpp"
#include "res_path.hpp"
#include "cleanup.hpp"
#include "utils.hpp"

void renderScene(SDL_Renderer *renderer, SDL_Texture *backgroundImage)
{
    // Render the scene
    SDL_RenderClear(renderer);
    renderTexture(backgroundImage, renderer, 0, 0);
    SDL_RenderPresent(renderer);
}


int main()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logSDLError(std::cout, "SDL_Init");
        return 1;
    }

    if ((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
        logSDLError(std::cout, "IMG_Init");
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("pacman", 100, 100, SCREEN_WIDTH,
            SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        logSDLError(std::cout, "CreateWindow");
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        logSDLError(std::cout, "CreateRenderer");
        cleanup(window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    const std::string resPath = getResourcePath();
    SDL_Texture *backgroundImage = loadTexture(resPath + "background.png", renderer);
    if (backgroundImage == nullptr) {
        cleanup(renderer, window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e))
            if (e.type == SDL_QUIT)
                quit = true;

        renderScene(renderer, backgroundImage);
        SDL_Delay(100);
    }

    cleanup(backgroundImage, renderer, window);
    IMG_Quit();
    SDL_Quit();
    return 0;
}