#ifndef CONSTS_HPP
#define CONSTS_HPP

static const int SCREEN_WIDTH   = 450;
static const int SCREEN_HEIGHT  = 450;

static const int TEXTURE_SIZE  = 90;

static const int MAP_SCALE = 3;
// Ensure it is a whole number
static const int TEXTURE_STEP = TEXTURE_SIZE / MAP_SCALE;

enum direction { UP, DOWN, LEFT, RIGHT, STOP };

#endif // CONSTS_HPP
