#include "snake_game.h"

Snake::Snake()
    : direction{DOWN}, head{0, 0}, tail{0, 0}, body{}, colour{0x63, 0x27, 0x8f, 0xFF}
{
    body.push_front(head);
}

void Snake::draw(SDL_Renderer* renderer, SDL_Point* back) const{

    if (back != nullptr){ // we do need to cover the tail, else it means an apple was eaten
        //we'll need to draw the background over the tail 
        SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
        SDL_Rect rect{back->x, back->y, grid_square_size, grid_square_size};
        SDL_RenderFillRect(renderer, &rect);
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
        //upper horizontal
        SDL_RenderDrawLine(renderer, back->x, back->y, back->x + grid_square_size, back->y);
        //lower horizontal
        SDL_RenderDrawLine(renderer, back->x, back->y + grid_square_size, back->x + grid_square_size, back->y + grid_square_size);
        //left vertical
        SDL_RenderDrawLine(renderer, back->x, back->y, back->x, back->y + grid_square_size);
        //right vertical
        SDL_RenderDrawLine(renderer, back->x + grid_square_size, back->y, back->x + grid_square_size, back->y + grid_square_size);
    }

    //Draw new head
    SDL_SetRenderDrawColor(renderer, colour.r, colour.g, colour.b, colour.a);
    SDL_Rect fillRect{head.x, head.y, grid_square_size, grid_square_size};
    SDL_RenderFillRect(renderer, &fillRect);
    //Draw outline
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    //upper horizontal
    SDL_RenderDrawLine(renderer, head.x, head.y, head.x + grid_square_size, head.y);
    //lower horizontal
    SDL_RenderDrawLine(renderer, head.x, head.y + grid_square_size, head.x + grid_square_size, head.y + grid_square_size);
    //left vertical
    SDL_RenderDrawLine(renderer, head.x, head.y, head.x, head.y + grid_square_size);
    //right vertical
    SDL_RenderDrawLine(renderer, head.x + grid_square_size, head.y, head.x + grid_square_size, head.y + grid_square_size);
}