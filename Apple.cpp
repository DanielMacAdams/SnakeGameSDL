#include "snake_game.h"

Apple::Apple()
    : position{400, 400}, colour{0xa6, 0x08, 0x08, 0xFF}, is_eaten{false}
{}

void Apple::draw(SDL_Renderer* renderer, bool (&grid)[ROWS][COLS]){

    std::vector<SDL_Point> v_points{};
    for (int row = 0; row < ROWS; ++row){
        for (int col = 0; col < COLS; ++col){
            if(!grid[row][col]){
                printf("SDL POINT: {%d, %d}\n", row*grid_square_size, col*grid_square_size);
                // Remember: SDL_Point.y corresponds to the row, SDL_Point.x to the column
                v_points.push_back({col*grid_square_size, row*grid_square_size});
            }
        }
    }

    int rnd{std::rand() % v_points.size()};
    SDL_assert(rnd >= 0 && rnd < v_points.size());
    SDL_Point rnd_point{v_points[rnd]};

    position.x = rnd_point.x;
    position.y = rnd_point.y;

    printf("SPAWN POINT: {%d, %d}\n", position.x, position.y);

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