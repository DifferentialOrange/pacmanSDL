#ifndef FIELD_HPP
#define FIELD_HPP

#include <array>
#include <map>
#include "consts.hpp"

static std::map<int, Block> FileToBlock = { {0, FREE}, {1, WALL} };

using BlockField = std::array<std::array<Block, BLOCK_WIDTH>, BLOCK_HEIGHT>;

bool canGo(int x, int y);

#endif // FIELD_HPP
