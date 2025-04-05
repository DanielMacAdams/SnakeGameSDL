#include "snake_game.h"

bool init(SDL_Window* &window, SDL_Renderer* &renderer);
void close(SDL_Window* &window, SDL_Renderer* &renderer);

int main(){

    bool grid_occupied[ROWS][COLS]{};
    int score = 1;

    SDL_Window* window = NULL;
    SDL_Renderer* renderer = NULL;

    if(init(window, renderer) == false){
        return -1;
    }

    //draw starting snake
    Snake snake{};
    snake.draw(renderer, nullptr);
    grid_occupied[0][0] = true;

    //draw starting apple
    Apple apple{};
    apple.draw(renderer);
    
    SDL_RenderPresent(renderer);

    //use some timers
    uint32_t time = SDL_GetTicks();

    //main loop
    bool quit = false;
    SDL_Event event{};
    while(!quit){
        //Handle events on queue
        while(SDL_PollEvent(&event) != 0){
            if(event.type == SDL_QUIT){
                quit = true;
            } else if (event.type == SDL_KEYDOWN){
                switch (event.key.keysym.sym)
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

            // Step 1. move head forward
            // Step 2. check for collision (can check for collision last because apple should never spawn inside snake)
            // Step 3. if apple is eaten now, do not move tail
            // Step 4. if apple is not eaten, move tail

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
            SDL_Point* back = nullptr;
            if (snake.head.x == apple.position.x && snake.head.y == apple.position.y){
                apple.is_eaten = true;
                score += 1;
            } else {
                grid_occupied[snake.tail.y / grid_square_size][snake.tail.x / grid_square_size] = false;
                back = &snake.body.back();
                snake.body.pop_back();
                snake.tail = snake.body.back();
            }
            snake.draw(renderer, back);

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

            if (apple.is_eaten) {
                apple.is_eaten = false;
                apple.draw(renderer);
            }            

        }
        SDL_RenderPresent(renderer);

    }

    close(window, renderer);

    return 0;
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