#include <fstream>
#include <string>
#include "res_path.hpp"
#include "field.hpp"

static std::vector<std::vector<gridMesh>> readGameGrid(const std::string& fileName)
{
    const std::string filePath = getResourcePath() + fileName;

    std::fstream fieldFile(filePath);
    int fieldWidth, fieldHeigth;

    fieldFile >> fieldWidth >> fieldHeigth;

    std::vector<std::vector<gridMesh>> field(fieldHeigth, std::vector<gridMesh>(fieldWidth));

    for (int i = 0; i < fieldWidth; ++i)
        for (int j = 0; j < fieldHeigth; ++j) {
            int gridMeshValue;
            fieldFile >> gridMeshValue;

            switch(gridMeshValue) {
            case 0: 
                field[i][j] = FREE;
                break;
            case 1: 
                field[i][j] = WALL;
                break;
            }
        }

    return field;
}

std::vector<std::vector<gridMesh>> getAnimationGrid()
{
    auto gameGridField = readGameGrid("field.txt");

    int rawFieldWidth  = gameGridField[0].size();
    int rawFieldHeight = gameGridField.size();
    int scaledFieldWidth  = rawFieldWidth * ANIMATION_GRID_SCALE;
    int scaledFieldHeight = rawFieldHeight * ANIMATION_GRID_SCALE;

    std::vector<std::vector<gridMesh>> scaledField(scaledFieldHeight, std::vector<gridMesh>(scaledFieldWidth));

    for (int rawI = 0; rawI < rawFieldWidth; ++rawI)
        for (int rawJ = 0; rawJ < rawFieldHeight; ++rawJ)
            for (int scI = 0; scI < ANIMATION_GRID_SCALE; ++scI)
                for (int scJ = 0; scJ < ANIMATION_GRID_SCALE; ++scJ)
                    scaledField[ANIMATION_GRID_SCALE * rawI + scI][ANIMATION_GRID_SCALE * rawJ + scJ] = gameGridField[rawI][rawJ];

    return scaledField;
}

/* 
 * Check if we can go in the animation grid field.
 * (AGposX, AGposY) is the position of center of character.
 * Now we imply our field has boundary on the rims, REWORK LATER.
 */
bool canGo(const std::vector<std::vector<gridMesh>>& field, int AGposX, int AGposY, direction dir)
{
    bool can = true;

    switch (dir) {
    case UP:
        for (int i = - ANIMATION_GRID_SCALE / 2; i < ANIMATION_GRID_SCALE / 2; ++i)
            if (field[AGposY - ANIMATION_GRID_SCALE][AGposX + i] == WALL)
                can = false;
        break;
    case DOWN:
        for (int i = - ANIMATION_GRID_SCALE / 2; i < ANIMATION_GRID_SCALE / 2; ++i)
            if (field[AGposY + ANIMATION_GRID_SCALE][AGposX + i] == WALL)
                can = false;
        break;
    case LEFT:
        for (int i = - ANIMATION_GRID_SCALE / 2; i < ANIMATION_GRID_SCALE / 2; ++i)
            if (field[AGposY + i][AGposX - ANIMATION_GRID_SCALE] == WALL)
                can = false;
        break;
    case RIGHT:
        for (int i = - ANIMATION_GRID_SCALE / 2; i < ANIMATION_GRID_SCALE / 2; ++i)
            if (field[AGposY + i][AGposX + ANIMATION_GRID_SCALE] == WALL)
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
