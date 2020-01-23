#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include "field.hpp"
#include "consts.hpp"
#include "res_path.hpp"
#include "cleanup.hpp"

/**
* Log an SDL error with some error message to the output stream of our choice
* @param os The output stream to write the message to
* @param msg The error message to write, format will be msg error: SDL_GetError()
*/
void logSDLError(std::ostream &os, const std::string &msg)
{
    os << msg << " error: " << SDL_GetError() << std::endl;
}

/**
* Loads an image into a texture on the rendering device
* @param file The image file to load
* @param ren The renderer to load the texture onto
* @return the loaded texture, or nullptr if something went wrong.
*/
SDL_Texture* loadTexture(const std::string &file, SDL_Renderer *ren)
{
    SDL_Texture *texture = IMG_LoadTexture(ren, file.c_str());
    if (texture == nullptr)
        logSDLError(std::cout, "LoadTexture");

    return texture;
}

/**
* Draw an SDL_Texture to an SDL_Renderer at some destination rect
* taking a clip of the texture if desired
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param dst The destination rectangle to render the texture to
* @param clip The sub-section of the texture to draw (clipping rect)
*       default of nullptr draws the entire texture
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr)
{
    SDL_RenderCopy(ren, tex, clip, &dst);
}

/**
* Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
* the texture's width and height and taking a clip of the texture if desired
* If a clip is passed, the clip's width and height will be used instead of
*   the texture's
* @param tex The source texture we want to draw
* @param ren The renderer we want to draw to
* @param x The x coordinate to draw to
* @param y The y coordinate to draw to
* @param clip The sub-section of the texture to draw (clipping rect)
*       default of nullptr draws the entire texture
*/
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr)
{
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    if (clip != nullptr) {
        dst.w = clip->w;
        dst.h = clip->h;
    } else {
        SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    }
    renderTexture(tex, ren, dst, clip);
}

void renderScene(SDL_Renderer *renderer, SDL_Texture *image, int agX, int agY)
{
    // Render the scene
    SDL_RenderClear(renderer);
    renderTexture(image, renderer,
        transformToTextureGridTip(getAnimationGridTip(agX)),
        transformToTextureGridTip(getAnimationGridTip(agY)));
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
    SDL_Texture *pacman = loadTexture(resPath + "pacman.png", renderer);
    if (pacman == nullptr) {
        cleanup(pacman, renderer, window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

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
                    if (canGo(animationGrid, agX, agY, UP))
                        direction = UP;
                    else
                        direction = STAY;
                    break;

                case SDLK_DOWN:
                    if (canGo(animationGrid, agX, agY, DOWN))
                        direction = DOWN;
                    else
                        direction = STAY;
                    break;

                case SDLK_LEFT:
                    if (canGo(animationGrid, agX, agY, LEFT))
                        direction = LEFT;
                    else
                        direction = STAY;
                    break;

                case SDLK_RIGHT:
                    if (canGo(animationGrid, agX, agY, RIGHT))
                        direction = RIGHT;
                    else
                        direction = STAY;
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
            for (int i = 0; i < ANIMATION_GRID_SCALE; ++i) {
                agY -= 1;
                renderScene(renderer, pacman, agX, agY);
                SDL_Delay(30);
            }
            break;
        case DOWN:
            for (int i = 0; i < ANIMATION_GRID_SCALE; ++i) {
                agY += 1;
                renderScene(renderer, pacman, agX, agY);
                SDL_Delay(30);
            }
            break;
        case LEFT:
            for (int i = 0; i < ANIMATION_GRID_SCALE; ++i) {
                agX -= 1;
                renderScene(renderer, pacman, agX, agY);
                SDL_Delay(30);
            }
            break;
        case RIGHT:
            for (int i = 0; i < ANIMATION_GRID_SCALE; ++i) {
                agX += 1;
                renderScene(renderer, pacman, agX, agY);
                SDL_Delay(30);
            }
            break;
        case STAY:
            renderScene(renderer, pacman, agX, agY);
            break;
        }
        direction = STAY;
    }

    cleanup(pacman, renderer, window);
    IMG_Quit();
	SDL_Quit();
	return 0;
}