#include <fstream>
#include <string>
#include "res_path.hpp"
#include "field.hpp"

/*
 * We will deal with three levels of grid for the game field.
 * 1. Game grid (GG) -- the size of the one square mesh is equal to the size of Pacman itself.
 * Classic Pacman field is 21x27 in game grid terms.
 * field.txt file corresponds one number to one game grid mesh.
 * 2. Animation grid (AG) -- every square game grid mesh partitioned into 
 * ANIMATION_FIELD_SCALE * ANIMATION_FIELD_SCALE meshes to provide smooth movement animation.
 * 3. Texture grid (TG) -- every grid mesh represents exactly one pixel.
 */

// Implying our field has boundary on the rims
// 0 -- free
// 1 -- wall

static std::vector<std::vector<int>> readGameGridFieldFromFile(const std::string& fileName)
{
    const std::string filePath = getResourcePath() + fileName;

    std::fstream fieldFile(filePath);
    int fieldWidth, fieldHeigth;

    fieldFile >> fieldWidth >> fieldHeigth;

    std::vector<std::vector<int>> field(fieldHeigth, std::vector<int>(fieldWidth));

    for (int i = 0; i < fieldWidth; ++i)
        for (int j = 0; j < fieldHeigth; ++j)
            fieldFile >> field[i][j];

    return field;
}

std::vector<std::vector<int>> getAnimtaionGridField()
{
    auto gameGridField = readGameGridFieldFromFile("field.txt");

    int rawFieldWidth  = gameGridField[0].size();
    int rawFieldHeight = gameGridField.size();
    int scaledFieldWidth  = rawFieldWidth * ANIMATION_FIELD_SCALE;
    int scaledFieldHeight = rawFieldHeight * ANIMATION_FIELD_SCALE;

    std::vector<std::vector<int>> scaledField(scaledFieldHeight, std::vector<int>(scaledFieldWidth));

    for (int rawI = 0; rawI < rawFieldWidth; ++rawI)
        for (int rawJ = 0; rawJ < rawFieldHeight; ++rawJ)
            for (int scI = 0; scI < ANIMATION_FIELD_SCALE; ++scI)
                for (int scJ = 0; scJ < ANIMATION_FIELD_SCALE; ++scJ)
                    scaledField[ANIMATION_FIELD_SCALE * rawI + scI][ANIMATION_FIELD_SCALE * rawJ + scJ] = gameGridField[rawI][rawJ];

    return scaledField;
}

/* 
 * Check if we can go in the animation grid field
 * (posX, posY) is the position of center of character
 */
bool canGo(const std::vector<std::vector<int>>& field, int AGposX, int AGposY, direction dir)
{
    bool can = true;

    switch (dir) {
    case UP:
        for (int i = - ANIMATION_FIELD_SCALE / 2; i < ANIMATION_FIELD_SCALE / 2; ++i)
            if (field[AGposY - ANIMATION_FIELD_SCALE][AGposX + i] == 1)
                can = false;
        break;
    case DOWN:
        for (int i = - ANIMATION_FIELD_SCALE / 2; i < ANIMATION_FIELD_SCALE / 2; ++i)
            if (field[AGposY + ANIMATION_FIELD_SCALE][AGposX + i] == 1)
                can = false;
        break;
    case LEFT:
        for (int i = - ANIMATION_FIELD_SCALE / 2; i < ANIMATION_FIELD_SCALE / 2; ++i)
            if (field[AGposY + i][AGposX - ANIMATION_FIELD_SCALE] == 1)
                can = false;
        break;
    case RIGHT:
        for (int i = - ANIMATION_FIELD_SCALE / 2; i < ANIMATION_FIELD_SCALE / 2; ++i)
            if (field[AGposY + i][AGposX + ANIMATION_FIELD_SCALE] == 1)
                can = false;
        break;
    case STAY:
        break;
    }

    return can;
}

int transformToCenter(int x)
{
    return x * ANIMATION_FIELD_SCALE + ANIMATION_FIELD_SCALE / 2;
}

int transformToTip(int x)
{
    return x - ANIMATION_FIELD_SCALE / 2;
}

int getCoordinate(int x)
{
    return x * TEXTURE_STEP;
}
