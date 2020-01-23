#include <fstream>
#include <string>
#include "res_path.hpp"
#include "field.hpp"

static std::vector<std::vector<GridMesh>> readGameGrid(const std::string& fileName)
{
    const std::string filePath = getResourcePath() + fileName;

    std::fstream gridFile(filePath);
    int gridWidth, gridHeigth;

    gridFile >> gridWidth >> gridHeigth;

    static std::vector<std::vector<GridMesh>> gameGrid(gridHeigth, std::vector<GridMesh>(gridWidth));

    for (auto&& row : gameGrid)
        for (auto&& el : row) {
            int GridMeshValue;
            gridFile >> GridMeshValue;

            switch(GridMeshValue) {
            case 0: 
                el = FREE;
                break;
            case 1: 
                el = WALL;
                break;
            }
        }

    return gameGrid;
}

std::vector<std::vector<GridMesh>> getGameGrid()
{
    return readGameGrid("field.txt");
}

std::vector<std::vector<GridMesh>> getAnimationGrid()
{
    auto gameGrid = readGameGrid("field.txt");

    int gameGridWidth = gameGrid[0].size();
    int gameGridHeight = gameGrid.size();
    int animationGridWidth = gameGridWidth * ANIMATION_GRID_SCALE;
    int animationGridHeight = gameGridHeight * ANIMATION_GRID_SCALE;

    std::vector<std::vector<GridMesh>> animationGrid(animationGridHeight, std::vector<GridMesh>(animationGridWidth));

    for (int ggI = 0; ggI < gameGridHeight; ++ggI)
        for (int ggJ = 0; ggJ < gameGridWidth; ++ggJ)
            for (int agI = 0; agI < ANIMATION_GRID_SCALE; ++agI)
                for (int agJ = 0; agJ < ANIMATION_GRID_SCALE; ++agJ)
                    animationGrid[ANIMATION_GRID_SCALE * ggI + agI][ANIMATION_GRID_SCALE * ggJ + agJ] = gameGrid[ggI][ggJ];

    return animationGrid;
}

/* 
 * Check if we can go in the animation grid field.
 * (agX, agY) is the position of the center of character.
 * Now we imply our field has boundary on the rims, REWORK LATER.
 */
bool canGo(const std::vector<std::vector<GridMesh>>& animationGrid, int agX, int agY, Direction direction)
{
    bool can = true;

    switch (direction) {
    case UP:
        for (int i = - ANIMATION_GRID_SCALE / 2; i < ANIMATION_GRID_SCALE / 2; ++i)
            if (animationGrid[agY - ANIMATION_GRID_SCALE][agX + i] == WALL)
                can = false;
        break;
    case DOWN:
        for (int i = - ANIMATION_GRID_SCALE / 2; i < ANIMATION_GRID_SCALE / 2; ++i)
            if (animationGrid[agY + ANIMATION_GRID_SCALE][agX + i] == WALL)
                can = false;
        break;
    case LEFT:
        for (int i = - ANIMATION_GRID_SCALE / 2; i < ANIMATION_GRID_SCALE / 2; ++i)
            if (animationGrid[agY + i][agX - ANIMATION_GRID_SCALE] == WALL)
                can = false;
        break;
    case RIGHT:
        for (int i = - ANIMATION_GRID_SCALE / 2; i < ANIMATION_GRID_SCALE / 2; ++i)
            if (animationGrid[agY + i][agX + ANIMATION_GRID_SCALE] == WALL)
                can = false;
        break;
    case STAY:
        break;
    }

    return can;
}

/* Obtain center coordinate of entity in animation grid from game grid coordinate. */
int transformToAnimationGridCenter(int ggX)
{
    return ggX * ANIMATION_GRID_SCALE + ANIMATION_GRID_SCALE / 2;
}

/* Obtain left top tip coordinate of entity in animation grid from its center in animation grid coordinate. */
int getAnimationGridTip(int agX)
{
    return agX - ANIMATION_GRID_SCALE / 2;
}

/* Obtain left top tip coordinate of entity in texture grid from animation grid coordinate. */
int transformToTextureGridTip(int agX)
{
    return agX * ANIMATION_GRID_MESH_SIZE;
}
