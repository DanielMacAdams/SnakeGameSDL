#include <SDL2/SDL.h>
#include <iostream>
#include <deque>
#include <unordered_map>
//#include <random>

const int ROWS = 13;
const int COLS = ROWS;
const int grid_square_size = 100;
const int HEIGHT = COLS * grid_square_size;
const int WIDTH = ROWS * grid_square_size;

bool grid_occupied[ROWS][COLS]{};
//std::unordered_map<int, SDL_Point> available_points{};
int score = 1;

typedef enum{
    UP,
    DOWN,
    LEFT,
    RIGHT,
}DIRECTION;

typedef struct Snake{
    int direction;
    SDL_Point head;
    SDL_Point tail;
    SDL_Point last_tail;
    std::deque<SDL_Point> body;
};

typedef struct Apple{
    SDL_Point pos;
    bool is_eaten;
};

// std::unordered_map<long, SDL_Point> map{};

SDL_Color background{0x32, 0xa8, 0x52, 0xFF};
SDL_Color snake_colour{0x63, 0x27, 0x8f, 0xFF};
SDL_Color apple_colour{0xa6, 0x08, 0x08, 0xFF};

bool init(SDL_Window* &window, SDL_Renderer* &renderer);
void close(SDL_Window* &window, SDL_Renderer* &renderer);
void draw_snake(Snake* snake, SDL_Renderer* &renderer);
void draw_apple(SDL_Point* apple, SDL_Renderer* &renderer);

int main(){

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if(init(window, renderer) == false){
        return -1;
    }

    //draw starting snake
    Snake snake{DOWN, {0, 0}, {0, 0}, {0, 0}, {}};
    snake.body.push_front(snake.head);
    grid_occupied[0][0] = true;
    draw_snake(&snake, renderer);
    
    //draw starting apple
    Apple apple{{5,5}, false};
    draw_apple(&apple.pos, renderer);
    apple.is_eaten = false;

    SDL_RenderPresent(renderer);

    //use some timers
    uint32_t time = SDL_GetTicks();

    //main loop
    bool quit = false;
    SDL_Event e{};
    while(!quit){
        //Handle events on queue
        while(SDL_PollEvent(&e) != 0){
            if(e.type == SDL_QUIT){
                quit = true;
            } else if (e.type == SDL_KEYDOWN){
                switch (e.key.keysym.sym)
                {
                case SDLK_q:
                    quit = true;
                    break;
                
                case SDLK_UP:
                    if(snake.direction != DOWN){snake.direction = UP;}
                    break;

                case SDLK_DOWN:
                    if(snake.direction != UP){snake.direction = DOWN;}
                    break;                

                case SDLK_LEFT:
                    if(snake.direction != RIGHT){snake.direction = LEFT;}
                    break;

                case SDLK_RIGHT:
                    if(snake.direction != LEFT){snake.direction = RIGHT;}
                    break;                    

                default:
                    quit = true;
                    break;
                }
            }          
        }

        uint32_t temp = SDL_GetTicks();
        if (temp - time > 150){
            time = temp;
            if (snake.direction == UP){
                snake.head.y -= grid_square_size;
            } else if (snake.direction == DOWN){
                snake.head.y += grid_square_size;
            } else if (snake.direction == LEFT){
                snake.head.x -= grid_square_size;
            } else if (snake.direction == RIGHT){
                snake.head.x += grid_square_size;
            }

            snake.last_tail = snake.tail;
            grid_occupied[snake.last_tail.y / grid_square_size][snake.last_tail.x / grid_square_size] = false;

            //detect a collision else update head
            if (grid_occupied[snake.head.y / grid_square_size][snake.head.x / grid_square_size]){
                //game over
                printf("Game Over!\n");
                printf("Score: %d\n", score);
                quit = true;
            } else {
                grid_occupied[snake.head.y / grid_square_size][snake.head.x / grid_square_size] = true;
                snake.body.push_front(snake.head);
            }

            //detect if apple is eaten else update tail
            if (snake.head.x == apple.pos.x && snake.head.y == apple.pos.y){
                apple.is_eaten = true;
                score += 1;
                snake.tail = snake.body.back();
                grid_occupied[snake.tail.y / grid_square_size][snake.tail.x / grid_square_size] = true;
            } else {
                snake.body.pop_back();
                snake.tail = snake.body.back();
            }

            //checking the grid
            printf("=========================================\n");
            for (int row = 0; row < ROWS; row++){
                printf("|");
                for (int col = 0; col < COLS; col++){
                    printf(" %d ", grid_occupied[row][col]);
                }
                printf("|\n");
            }
            printf("=========================================\n");

            draw_snake(&snake, renderer);

            if (apple.is_eaten) {
                draw_apple(&apple.pos, renderer);
                apple.is_eaten = false;
            }            

        }
        SDL_RenderPresent(renderer);

    }

    close(window, renderer);

    return 0;
}

void draw_snake(Snake* snake, SDL_Renderer* &renderer){
    
    //we'll need to draw the background over the tail 
    SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
    SDL_Rect rect{snake->last_tail.x, snake->last_tail.y, grid_square_size, grid_square_size};
    SDL_RenderFillRect(renderer, &rect);
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    //upper horizontal
    SDL_RenderDrawLine(renderer, snake->last_tail.x, snake->last_tail.y, snake->last_tail.x + grid_square_size, snake->last_tail.y);
    //lower horizontal
    SDL_RenderDrawLine(renderer, snake->last_tail.x, snake->last_tail.y + grid_square_size, snake->last_tail.x + grid_square_size, snake->last_tail.y + grid_square_size);
    //left vertical
    SDL_RenderDrawLine(renderer, snake->last_tail.x, snake->last_tail.y, snake->last_tail.x, snake->last_tail.y + grid_square_size);
    //right vertical
    SDL_RenderDrawLine(renderer, snake->last_tail.x + grid_square_size, snake->last_tail.y, snake->last_tail.x + grid_square_size, snake->last_tail.y + grid_square_size);

    //Draw new head
    SDL_SetRenderDrawColor(renderer, snake_colour.r, snake_colour.g, snake_colour.b, snake_colour.a);
    SDL_Rect fillRect{snake->head.x, snake->head.y, grid_square_size, grid_square_size};
    SDL_RenderFillRect(renderer, &fillRect);
    //Draw outline
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    //upper horizontal
    SDL_RenderDrawLine(renderer, snake->head.x, snake->head.y, snake->head.x + grid_square_size, snake->head.y);
    //lower horizontal
    SDL_RenderDrawLine(renderer, snake->head.x, snake->head.y + grid_square_size, snake->head.x + grid_square_size, snake->head.y + grid_square_size);
    //left vertical
    SDL_RenderDrawLine(renderer, snake->head.x, snake->head.y, snake->head.x, snake->head.y + grid_square_size);
    //right vertical
    SDL_RenderDrawLine(renderer, snake->head.x + grid_square_size, snake->head.y, snake->head.x + grid_square_size, snake->head.y + grid_square_size);

}

void draw_apple(SDL_Point* apple, SDL_Renderer* &renderer){

    //How to pick a location not occupied by the snake
    


    // Plan 1: 
    // only when the apple is eaten (so in this function), generate an array of available points based on
    // the grid_occupied array's current state. randomly select from this array. This array will need to be dynamically allocated

    // Plan 2:
    // I can use a hashmap and store the key (//I'll just use the row/column information to make a unique key)
    // and the point as the value, this would help avoid some
    // unnecessary bit manipulation

    //can use some profiling tools to see if this is worse performance wise 

    
    //Implementing Plan 1:

    SDL_Point *available_points{new SDL_Point[ROWS*COLS - score]};
    int counter = 0;
    for (int row = 0; row < ROWS; ++row){
        for (int col = 0; col < COLS; ++col){
            if (!grid_occupied[row][col]){
                available_points[counter].x = row * grid_square_size;
                available_points[counter].y = col * grid_square_size;
                ++counter;
            }
        }
    }

    delete[] available_points;

    int range = ROWS;
    apple->x = (std::rand() % range) * grid_square_size;
    apple->y = (std::rand() % range) * grid_square_size;
    
    SDL_SetRenderDrawColor(renderer, apple_colour.r, apple_colour.g, apple_colour.b, apple_colour.a);
    SDL_Rect fillRect{apple->x, apple->y, grid_square_size, grid_square_size};
    SDL_RenderFillRect(renderer, &fillRect);

    //Draw outline
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
    //upper horizontal
    SDL_RenderDrawLine(renderer, apple->x, apple->y, apple->x + grid_square_size, apple->y);
    //lower horizontal
    SDL_RenderDrawLine(renderer, apple->x, apple->y + grid_square_size, apple->x + grid_square_size, apple->y + grid_square_size);
    //left vertical
    SDL_RenderDrawLine(renderer, apple->x, apple->y, apple->x, apple->y + grid_square_size);
    //right vertical
    SDL_RenderDrawLine(renderer, apple->x + grid_square_size, apple->y, apple->x + grid_square_size, apple->y + grid_square_size);    

}

bool init(SDL_Window* &window, SDL_Renderer* &renderer){
    
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL failed to init: %s\n", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL){
        printf("window could not be created: %s\n", SDL_GetError());
        return false;
    } else {
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        if( renderer == NULL )
        {
            printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
            return false;
        }

        //draw background
        SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, background.a);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);

        int start_x{};
        int start_y{};
        int end_x{};
        int end_y{};
        //Draw vertical lines
        for (int i = 0; i < WIDTH; i = i + WIDTH/COLS){
            start_x = i;
            start_y = 0;
            end_x = i;
            end_y = HEIGHT;
            SDL_RenderDrawLine(renderer, start_x, start_y, end_x, end_y);
        }
        //Draw horizontal lines
        for (int i = 0; i < HEIGHT; i = i + HEIGHT/ROWS){
            start_x = 0;
            start_y = i;
            end_x = WIDTH;
            end_y = i;
            SDL_RenderDrawLine(renderer, start_x, start_y, end_x, end_y);
        }
    }

    return true;
}

void close(SDL_Window* &window, SDL_Renderer* &renderer){
    SDL_DestroyRenderer( renderer );
	SDL_DestroyWindow( window );
	window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	// IMG_Quit();
	SDL_Quit();
}