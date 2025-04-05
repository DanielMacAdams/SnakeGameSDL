#ifndef SNAKE_GAME
#define SNAKE_GAME

#include <SDL2/SDL.h>
#include <iostream>
#include <deque>
#include <unordered_map>

// Global Constants
const int ROWS = 10;
const int COLS = ROWS;
const int grid_square_size = 100;
const int HEIGHT = COLS * grid_square_size;
const int WIDTH = ROWS * grid_square_size;
const SDL_Color background{0x32, 0xa8, 0x52, 0xFF};

// Enums
typedef enum{
    UP,
    DOWN,
    LEFT,
    RIGHT,
}DIRECTION;

// Class Definitions
class Snake{
public:
    uint8_t direction{};
    SDL_Point head{};
    SDL_Point tail{};
    std::deque<SDL_Point> body{};
    SDL_Color colour{}; //{0x63, 0x27, 0x8f, 0xFF};

    Snake();
    void draw(SDL_Renderer* renderer) const;
};

class Apple{
public:
    SDL_Point position{};
    SDL_Color colour{}; //{0xa6, 0x08, 0x08, 0xFF};
    bool is_eaten{};

    Apple();
    void draw(SDL_Renderer* renderer) const;
};

#endif