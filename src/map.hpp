#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include "consts.hpp"

std::vector<std::vector<int>> getMap();
bool canGo(const std::vector<std::vector<int>>& map, int posX, int posY, direction dir);

int transformToCenter(int x);
int transformToTip(int x);
int getCoordinate(int x);

#endif // MAP_HPP
