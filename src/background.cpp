#include "background.hpp"
#include "field.hpp"
#include "utils.hpp"

static inline bool isFree(const std::vector<std::vector<GridMesh>>& gameGrid, int ggX, int ggY)
{
    int width = gameGrid[0].size();
    int heigth = gameGrid.size();

    if ((ggX < 0) || (ggY < 0) || (ggX >= width) || (ggY >= heigth))
        return true;

    return gameGrid[ggY][ggX] == FREE;
}

static inline bool isWall(const std::vector<std::vector<GridMesh>>& gameGrid, int ggX, int ggY)
{
    return !isFree(gameGrid, ggX, ggY);
}

static inline void plotTile(SDL_Renderer *renderer, SDL_Texture *backgroundImage, SDL_Rect clips[16], 
    int ggX, int ggY, FieldPiece tile)
{
    int tgX = transformToTextureGridTip(getAnimationGridTip(transformToAnimationGridCenter(ggX)));
    int tgY = transformToTextureGridTip(getAnimationGridTip(transformToAnimationGridCenter(ggY)));

    renderTexture(backgroundImage, renderer, tgX, tgY, &clips[GetFieldClip[tile]]);
}

void renderBackground(SDL_Renderer *renderer, SDL_Texture *backgroundImage)
{
    auto gameGrid = getGameGrid();

    int width = gameGrid[0].size();
    int heigth = gameGrid.size();

    SDL_Rect clips[16];
    for (int i = 0; i < 16; ++i) {
        clips[i].x = (i % 4) * GAME_GRID_MESH_SIZE;
        clips[i].y = (i / 4) * GAME_GRID_MESH_SIZE;
        clips[i].w = GAME_GRID_MESH_SIZE;
        clips[i].h = GAME_GRID_MESH_SIZE;
    }

    for (int ggY = 0; ggY < heigth; ++ggY)
        for (int ggX = 0; ggX < width; ++ggX) {
            /* Some non-classical cases are not considered. */
            if (isFree(gameGrid, ggX, ggY)) {
                plotTile(renderer, backgroundImage, clips, ggX, ggY, FREE_TILE);
                continue;
            }

            bool upWall        = isWall(gameGrid, ggX, ggY - 1);
            bool downWall      = isWall(gameGrid, ggX, ggY + 1);
            bool leftWall      = isWall(gameGrid, ggX - 1, ggY);
            bool rightWall     = isWall(gameGrid, ggX + 1, ggY);
            bool upLeftWall    = isWall(gameGrid, ggX - 1, ggY - 1);
            bool upRightWall   = isWall(gameGrid, ggX + 1, ggY - 1);
            bool downLeftWall  = isWall(gameGrid, ggX - 1, ggY + 1);
            bool downRightWall = isWall(gameGrid, ggX + 1, ggY + 1);

            int wallCount = upWall + downWall + leftWall + rightWall;
            int extendedWallCount = wallCount + upLeftWall + upRightWall +
                                    downLeftWall + downRightWall;

            switch (wallCount) {
            case 1:
                if (upWall) {
                    plotTile(renderer, backgroundImage, clips, ggX, ggY, END_UP);
                    break;
                }
                if (downWall) {
                    plotTile(renderer, backgroundImage, clips, ggX, ggY, END_DOWN);
                    break;
                }
                if (leftWall) {
                    plotTile(renderer, backgroundImage, clips, ggX, ggY, END_LEFT);
                    break;
                }
                if (rightWall) {
                    plotTile(renderer, backgroundImage, clips, ggX, ggY, END_RIGHT);
                    break;
                }
                break;
            case 2:
                if (upWall && downWall) {
                    plotTile(renderer, backgroundImage, clips, ggX, ggY, STRAIGHT_UP_DOWN);
                    break;
                }
                if (leftWall && rightWall) {
                    plotTile(renderer, backgroundImage, clips, ggX, ggY, STRAIGHT_LEFT_RIGHT);
                    break;
                }
                if (rightWall && downWall) {
                    plotTile(renderer, backgroundImage, clips, ggX, ggY, TURN_RIGHT_DOWN);
                    break;
                }
                if (leftWall && downWall) {
                    plotTile(renderer, backgroundImage, clips, ggX, ggY, TURN_LEFT_DOWN);
                    break;
                }
                if (leftWall && upWall) {
                    plotTile(renderer, backgroundImage, clips, ggX, ggY, TURN_LEFT_UP);
                    break;
                }
                if (rightWall && upWall) {
                    plotTile(renderer, backgroundImage, clips, ggX, ggY, TURN_RIGHT_UP);
                    break;
                }
                break;
            case 3:
                switch (extendedWallCount) {
                case 3:
                case 4:
                    if (!upWall) {
                        plotTile(renderer, backgroundImage, clips, ggX, ggY, THREEWAY_LEFT_DOWN_RIGHT);
                        break;
                    }
                    if (!downWall) {
                        plotTile(renderer, backgroundImage, clips, ggX, ggY, THREEWAY_LEFT_UP_RIGHT);
                        break;
                    }
                    if (!leftWall) {
                        plotTile(renderer, backgroundImage, clips, ggX, ggY, THREEWAY_UP_RIGHT_DOWN);
                        break;
                    }
                    if (!rightWall) {
                        plotTile(renderer, backgroundImage, clips, ggX, ggY, THREEWAY_UP_LEFT_DOWN);
                        break;
                    }
                    break;
                case 5:
                case 6:
                    if (!upWall) {
                        plotTile(renderer, backgroundImage, clips, ggX, ggY, STRAIGHT_LEFT_RIGHT);
                        break;
                    }
                    if (!downWall) {
                        plotTile(renderer, backgroundImage, clips, ggX, ggY, STRAIGHT_LEFT_RIGHT);
                        break;
                    }
                    if (!leftWall) {
                        plotTile(renderer, backgroundImage, clips, ggX, ggY, STRAIGHT_UP_DOWN);
                        break;
                    }
                    if (!rightWall) {
                        plotTile(renderer, backgroundImage, clips, ggX, ggY, STRAIGHT_UP_DOWN);
                        break;
                    }
                    break;
                case 7:
                    if (!upLeftWall && !upRightWall) {
                        plotTile(renderer, backgroundImage, clips, ggX, ggY, THREEWAY_LEFT_DOWN_RIGHT);
                        break;
                    }
                    if (!upLeftWall && !downLeftWall) {
                        plotTile(renderer, backgroundImage, clips, ggX, ggY, THREEWAY_UP_RIGHT_DOWN);
                        break;
                    }
                    if (!downLeftWall && !downRightWall) {
                        plotTile(renderer, backgroundImage, clips, ggX, ggY, THREEWAY_LEFT_UP_RIGHT);
                        break;
                    }
                    if (!upRightWall && !downRightWall) {
                        plotTile(renderer, backgroundImage, clips, ggX, ggY, THREEWAY_UP_LEFT_DOWN);
                        break;
                    }
                    break;
                default:
                    break;
                }
                break;
            case 4:
                if (!upLeftWall && !upRightWall) {
                    plotTile(renderer, backgroundImage, clips, ggX, ggY, THREEWAY_LEFT_UP_RIGHT);
                    break;
                }
                if (!upLeftWall && !downLeftWall) {
                    plotTile(renderer, backgroundImage, clips, ggX, ggY, THREEWAY_UP_LEFT_DOWN);
                    break;
                }
                if (!downLeftWall && !downRightWall) {
                    plotTile(renderer, backgroundImage, clips, ggX, ggY, THREEWAY_LEFT_DOWN_RIGHT);
                    break;
                }
                if (!upRightWall && !downRightWall) {
                    plotTile(renderer, backgroundImage, clips, ggX, ggY, THREEWAY_UP_RIGHT_DOWN);
                    break;
                }
                break;
            default:
                break;
            }
        }
}
