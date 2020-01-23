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

    std::vector<std::vector<GridMesh>> gameGrid(gridHeigth, std::vector<GridMesh>(gridWidth));

    for (int i = 0; i < gridWidth; ++i)
        for (int j = 0; j < gridHeigth; ++j) {
            int GridMeshValue;
            gridFile >> GridMeshValue;

            switch(GridMeshValue) {
            case 0: 
                gameGrid[i][j] = FREE;
                break;
            case 1: 
                gameGrid[i][j] = WALL;
                break;
            }
        }

    return gameGrid;
}

std::vector<std::vector<GridMesh>> getAnimationGrid()
{
    auto gameGrid = readGameGrid("field.txt");

    int gameGridWidth = gameGrid[0].size();
    int gameGridHeight = gameGrid.size();
    int animationGridWidth = gameGridWidth * ANIMATION_GRID_SCALE;
    int animationGridHeight = gameGridHeight * ANIMATION_GRID_SCALE;

    std::vector<std::vector<GridMesh>> animationGrid(animationGridHeight, std::vector<GridMesh>(animationGridWidth));

    for (int ggI = 0; ggI < gameGridWidth; ++ggI)
        for (int ggJ = 0; ggJ < gameGridHeight; ++ggJ)
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
            if (animationGrid[AGposY - ANIMATION_GRID_SCALE][AGposX + i] == WALL)
                can = false;
        break;
    case DOWN:
        for (int i = - ANIMATION_GRID_SCALE / 2; i < ANIMATION_GRID_SCALE / 2; ++i)
            if (animationGrid[AGposY + ANIMATION_GRID_SCALE][AGposX + i] == WALL)
                can = false;
        break;
    case LEFT:
        for (int i = - ANIMATION_GRID_SCALE / 2; i < ANIMATION_GRID_SCALE / 2; ++i)
            if (animationGrid[AGposY + i][AGposX - ANIMATION_GRID_SCALE] == WALL)
                can = false;
        break;
    case RIGHT:
        for (int i = - ANIMATION_GRID_SCALE / 2; i < ANIMATION_GRID_SCALE / 2; ++i)
            if (animationGrid[AGposY + i][AGposX + ANIMATION_GRID_SCALE] == WALL)
                can = false;
        break;
    case STAY:
        break;
    }

    return can;
}

int transformToCenter(int x)
{
    return x * ANIMATION_GRID_SCALE + ANIMATION_GRID_SCALE / 2;
}

int transformToTip(int x)
{
    return x - ANIMATION_GRID_SCALE / 2;
}

int getCoordinate(int x)
{
    return x * ANIMATION_GRID_MESH_SIZE;
}
