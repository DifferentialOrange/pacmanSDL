#ifndef CONSTS_HPP
#define CONSTS_HPP

enum Direction { UP, DOWN, LEFT, RIGHT, STAY };
enum Block { FREE, WALL };

static const int FIELD_SCALE  = 16; // Size of each field.txt block
static const int BLOCK_WIDTH  = 41;
static const int BLOCK_HEIGHT = 53;

static const int SCREEN_WIDTH  = BLOCK_WIDTH * FIELD_SCALE;
static const int SCREEN_HEIGHT = BLOCK_HEIGHT * FIELD_SCALE;

#endif // CONSTS_HPP
