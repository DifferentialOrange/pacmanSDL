#include <fstream>
#include <string>
#include "res_path.hpp"
#include "map.hpp"

// Implying our map has boundary on the rims
// 0 -- free
// 1 -- wall

static std::vector<std::vector<int>> readMapFromFile(const std::string& fileName)
{
    const std::string filePath = getResourcePath() + fileName;

    std::fstream mapFile(filePath);
    int mapWidth, mapHeigth;

    mapFile >> mapWidth >> mapHeigth;

    std::vector<std::vector<int>> map(mapHeigth, std::vector<int>(mapWidth));

    for (int i = 0; i < mapWidth; ++i)
        for (int j = 0; j < mapHeigth; ++j)
            mapFile >> map[i][j];

    return map;
}

std::vector<std::vector<int>> getMap()
{
    auto rawMap = readMapFromFile("map.txt");

    int rawMapWidth  = rawMap[0].size();
    int rawMapHeight = rawMap.size();
    int scaledMapWidth  = rawMapWidth * MAP_SCALE;
    int scaledMapHeight = rawMapHeight * MAP_SCALE;

    std::vector<std::vector<int>> scaledMap(scaledMapHeight, std::vector<int>(scaledMapWidth));

    for (int rawI = 0; rawI < rawMapWidth; ++rawI)
        for (int rawJ = 0; rawJ < rawMapHeight; ++rawJ)
            for (int scI = 0; scI < MAP_SCALE; ++scI)
                for (int scJ = 0; scJ < MAP_SCALE; ++scJ)
                    scaledMap[MAP_SCALE * rawI + scI][MAP_SCALE * rawJ + scJ] = rawMap[rawI][rawJ];

    return scaledMap;
}

/* 
 * Check if we can go in the scaled map
 * (posX, posY) is the position of center of character
 */
bool canGo(const std::vector<std::vector<int>>& map, int posX, int posY, direction dir)
{
    bool can = true;

    switch (dir) {
    case UP:
        for (int i = - MAP_SCALE / 2; i < MAP_SCALE / 2; ++i)
            if (map[posY - MAP_SCALE][posX + i] == 1)
                can = false;
        break;
    case DOWN:
        for (int i = - MAP_SCALE / 2; i < MAP_SCALE / 2; ++i)
            if (map[posY + MAP_SCALE][posX + i] == 1)
                can = false;
        break;
    case LEFT:
        for (int i = - MAP_SCALE / 2; i < MAP_SCALE / 2; ++i)
            if (map[posY + i][posX - MAP_SCALE] == 1)
                can = false;
        break;
    case RIGHT:
        for (int i = - MAP_SCALE / 2; i < MAP_SCALE / 2; ++i)
            if (map[posY + i][posX + MAP_SCALE] == 1)
                can = false;
        break;
    case STOP:
        break;
    }

    return can;
}

int transformToCenter(int x)
{
    return x * MAP_SCALE + MAP_SCALE / 2;
}

int transformToTip(int x)
{
    return x - MAP_SCALE / 2;
}

int getCoordinate(int x)
{
    return x * TEXTURE_STEP;
}
