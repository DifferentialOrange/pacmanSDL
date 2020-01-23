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

void renderScene(SDL_Renderer *renderer, SDL_Texture *image, int scaledGamePosX, int scaledGamePosY)
{
    // Render the scene
    SDL_RenderClear(renderer);
    renderTexture(image, renderer,
        getCoordinate(transformToTip(scaledGamePosX)),
        getCoordinate(transformToTip(scaledGamePosY)));
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
    SDL_Texture *image = loadTexture(resPath + "image.png", renderer);
    if (image == nullptr) {
        cleanup(image, renderer, window);
        IMG_Quit();
        SDL_Quit();
        return 1;
    }

    std::vector<std::vector<int>> AGField = getAnimtaionGridField();

    unsigned gamePosX = 1;
    unsigned gamePosY = 1;

    unsigned scaledGamePosX = transformToCenter(gamePosX);
    unsigned scaledGamePosY = transformToCenter(gamePosY);

    bool quit = false;
    direction dir = STAY;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;

            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                case SDLK_UP:
                    if (canGo(AGField, scaledGamePosX, scaledGamePosY, UP))
                        dir = UP;
                    else
                        dir = STAY;
                    break;

                case SDLK_DOWN:
                    if (canGo(AGField, scaledGamePosX, scaledGamePosY, DOWN))
                        dir = DOWN;
                    else
                        dir = STAY;
                    break;

                case SDLK_LEFT:
                    if (canGo(AGField, scaledGamePosX, scaledGamePosY, LEFT))
                        dir = LEFT;
                    else
                        dir = STAY;
                    break;

                case SDLK_RIGHT:
                    if (canGo(AGField, scaledGamePosX, scaledGamePosY, RIGHT))
                        dir = RIGHT;
                    else
                        dir = STAY;
                    break;

                case SDLK_ESCAPE:
                    quit = true;
                    break;

                default:
                    break;
                }
            }
        }

        switch (dir) {
        case UP:
            for (int i = 0; i < ANIMATION_FIELD_SCALE; ++i) {
                scaledGamePosY -= 1;
                renderScene(renderer, image, scaledGamePosX, scaledGamePosY);
                SDL_Delay(100);
            }
            break;
        case DOWN:
            for (int i = 0; i < ANIMATION_FIELD_SCALE; ++i) {
                scaledGamePosY += 1;
                renderScene(renderer, image, scaledGamePosX, scaledGamePosY);
                SDL_Delay(100);
            }
            break;
        case LEFT:
            for (int i = 0; i < ANIMATION_FIELD_SCALE; ++i) {
                scaledGamePosX -= 1;
                renderScene(renderer, image, scaledGamePosX, scaledGamePosY);
                SDL_Delay(100);
            }
            break;
        case RIGHT:
            for (int i = 0; i < ANIMATION_FIELD_SCALE; ++i) {
                scaledGamePosX += 1;
                renderScene(renderer, image, scaledGamePosX, scaledGamePosY);
                SDL_Delay(100);
            }
            break;
        case STAY:
            renderScene(renderer, image, scaledGamePosX, scaledGamePosY);
            break;
        }
        dir = STAY;
    }

    cleanup(image, renderer, window);
    IMG_Quit();
	SDL_Quit();
	return 0;
}