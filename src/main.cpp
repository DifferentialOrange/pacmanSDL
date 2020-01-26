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
#include "background.hpp"

void renderScene(SDL_Renderer *renderer, SDL_Texture *image, SDL_Texture *backgroundImage, 
    int agX, int agY, SDL_Rect *clip = nullptr)
{
    // Render the scene
    SDL_RenderClear(renderer);
    renderBackground(renderer, backgroundImage);
    renderTexture(image, renderer,
        transformToTextureGridTip(getAnimationGridTip(agX)),
        transformToTextureGridTip(getAnimationGridTip(agY)),
        clip);
    SDL_RenderPresent(renderer);
}

SDL_Rect *getPacmanClip(SDL_Rect clips[5], Direction direction = STAY)
{
    static bool mouthOpened = true;

    if (mouthOpened == false) {
        mouthOpened = true;
        return &clips[0];
    } else {
        mouthOpened = false;
        switch (direction) {
        case UP:
            return &clips[2];
        case DOWN:
            return &clips[4];
        case LEFT:
            return &clips[3];
        case RIGHT:
            return &clips[1];
        case STAY:
            return &clips[1];
        }
    }

    return nullptr;
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

    SDL_Texture *pacman = loadTexture(resPath + "pacman.png", renderer);
    if (pacman == nullptr) {
        cleanup(backgroundImage, renderer, window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    // Setup the clips for pacman
    SDL_Rect clips[5];
    for (int i = 0; i < 5; ++i) {
        clips[i].x = i * GAME_GRID_MESH_SIZE;
        clips[i].y = 0;
        clips[i].w = GAME_GRID_MESH_SIZE;
        clips[i].h = GAME_GRID_MESH_SIZE;
    }
    SDL_Rect *pacmanClip = &clips[1];

    std::vector<std::vector<GridMesh>> animationGrid = getAnimationGrid();

    /* Starting point. */
    int ggX = 10;
    int ggY = 15;

    int agX = transformToAnimationGridCenter(ggX);
    int agY = transformToAnimationGridCenter(ggY);

    bool quit = false;
    Direction direction = STAY;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;

            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_UP:
                    direction = UP;
                    break;

                case SDLK_DOWN:
                    direction = DOWN;
                    break;

                case SDLK_LEFT:
                    direction = LEFT;
                    break;

                case SDLK_RIGHT:
                    direction = RIGHT;
                    break;

                case SDLK_ESCAPE:
                    quit = true;
                    break;

                default:
                    break;
                }
            }
        }


        switch (direction) {
        case UP:
            if (canGo(animationGrid, agX, agY, UP)) {
                agY -= 1;
                pacmanClip = getPacmanClip(clips, direction);
            }
            break;
        case DOWN:
            if (canGo(animationGrid, agX, agY, DOWN)) {
                agY += 1;
                pacmanClip = getPacmanClip(clips, direction);
            }
            break;
        case LEFT:
            if (canGo(animationGrid, agX, agY, LEFT)) {
                agX -= 1;
                pacmanClip = getPacmanClip(clips, direction);
            }
            break;
        case RIGHT:
            if (canGo(animationGrid, agX, agY, RIGHT)) {
                agX += 1;
                pacmanClip = getPacmanClip(clips, direction);
            }
            break;
        case STAY:
            break;
        }

        renderScene(renderer, pacman, backgroundImage, agX, agY, pacmanClip);
        SDL_Delay(100);
    }

    cleanup(pacman, backgroundImage, renderer, window);
    IMG_Quit();
	SDL_Quit();
	return 0;
}