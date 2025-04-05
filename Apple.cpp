#include "snake_game.h"

Apple::Apple()
    : position{500, 500}, colour{0xa6, 0x08, 0x08, 0xFF}, is_eaten{false}
{}

void Apple::draw(SDL_Renderer* renderer){

    int size = 10;
    position.x = (std::rand() % size) * grid_square_size;
    position.y = (std::rand() % size) * grid_square_size;

    SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
    SDL_Rect fillRect{position.x, position.y, grid_square_size, grid_square_size};
    SDL_RenderFillRect(renderer, &fillRect);

    //Draw outline
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    //upper horizontal
    SDL_RenderDrawLine(renderer, position.x, position.y, position.x + grid_square_size, position.y);
    //lower horizontal
    SDL_RenderDrawLine(renderer, position.x, position.y + grid_square_size, position.x + grid_square_size, position.y + grid_square_size);
    //left vertical
    SDL_RenderDrawLine(renderer, position.x, position.y, position.x, position.y + grid_square_size);
    //right vertical
    SDL_RenderDrawLine(renderer, position.x + grid_square_size, position.y, position.x + grid_square_size, position.y + grid_square_size);  
}