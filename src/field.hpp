#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include "consts.hpp"

static const int SCREEN_WIDTH  = 450;
static const int SCREEN_HEIGHT = 450;

static const int TEXTURE_SIZE  = 90;

static const int ANIMATION_FIELD_SCALE = 9;
// Ensure it is a whole number
static const int TEXTURE_STEP = TEXTURE_SIZE / ANIMATION_FIELD_SCALE;

std::vector<std::vector<int>> getAnimtaionGridField();
bool canGo(const std::vector<std::vector<int>>& field, int posX, int posY, direction dir);

int transformToCenter(int x);
int transformToTip(int x);
int getCoordinate(int x);

#endif // MAP_HPP
