#ifndef BACKGROUND_HPP
#define BACKGROUND_HPP

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <map>
#include "consts.hpp"

enum FieldPiece {
    END_UP, END_RIGHT, END_DOWN, END_LEFT,
    STRAIGHT_UP_DOWN, STRAIGHT_LEFT_RIGHT,
    TURN_RIGHT_DOWN, TURN_LEFT_DOWN, TURN_LEFT_UP, TURN_RIGHT_UP,
    THREEWAY_UP_RIGHT_DOWN, THREEWAY_UP_LEFT_DOWN, THREEWAY_LEFT_UP_RIGHT, THREEWAY_LEFT_DOWN_RIGHT,
    FREE_TILE
};

static std::map<FieldPiece, int> GetFieldClip = {
    { END_UP, 2 }, { END_RIGHT, 3 }, { END_DOWN, 0 }, { END_LEFT, 1 },
    { STRAIGHT_UP_DOWN, 12 }, { STRAIGHT_LEFT_RIGHT, 13 },
    { TURN_RIGHT_DOWN, 4 }, { TURN_LEFT_DOWN, 5 }, { TURN_LEFT_UP, 6 }, { TURN_RIGHT_UP, 7 },
    { THREEWAY_UP_RIGHT_DOWN, 8 }, { THREEWAY_UP_LEFT_DOWN, 10 },
    { THREEWAY_LEFT_UP_RIGHT, 11 }, { THREEWAY_LEFT_DOWN_RIGHT, 9 },
    { FREE_TILE, 14 }
};

void renderBackground(SDL_Renderer *renderer, SDL_Texture *backgroundImage);

#endif // BACKGROUND_HPP
