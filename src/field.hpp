#ifndef MAP_HPP
#define MAP_HPP

#include <vector>
#include "consts.hpp"

static const int SCREEN_WIDTH  = 525;
static const int SCREEN_HEIGHT = 675;

/*
 * We will deal with three levels of grid for the game field.
 * 1. Game grid (gg) -- the size of the one square mesh is equal to the size of Pacman itself.
 * Classic Pacman field is 21x27 in game grid terms.
 * field.txt file corresponds one number to one game grid mesh.
 * 2. Animation grid (ag) -- every square game grid mesh partitioned into 
 * ANIMATION_GRID_SCALE * ANIMATION_GRID_SCALE meshes to provide smooth movement animation.
 * 3. Texture grid (tg) -- every grid mesh represents exactly one pixel.
 */

static const int GAME_GRID_MESH_SIZE = 25;

static const int ANIMATION_GRID_SCALE = 5;
static_assert( ANIMATION_GRID_SCALE % 2 == 1, "Animation grid scale should be an odd number");

static const int ANIMATION_GRID_MESH_SIZE = GAME_GRID_MESH_SIZE / ANIMATION_GRID_SCALE;
static_assert( GAME_GRID_MESH_SIZE % ANIMATION_GRID_SCALE == 0, "Animation grid scale should divide game grid mesh size");

enum GridMesh { FREE, WALL };

std::vector<std::vector<GridMesh>> getAnimationGrid();
std::vector<std::vector<GridMesh>> getGameGrid();
bool canGo(const std::vector<std::vector<GridMesh>>& animationGrid, int agX, int agY, Direction direction);

int transformToAnimationGridCenter(int ggX);
int getAnimationGridTip(int agX);
int transformToTextureGridTip(int agX);

#endif // MAP_HPP
