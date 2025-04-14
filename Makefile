#OBJS specifies which files to compile as part of the project
OBJS = snake_game.cpp Apple.cpp Snake.cpp
OBJS_BINDING = snake_game.cpp Apple.cpp Snake.cpp bindings.cpp
#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -w -g

# $(python3 -m pybind11 --includes)
INC = -I/usr/include/python3.10 -I/home/daniel/.local/lib/python3.10/site-packages/pybind11/include

# $(python3-config --extension-suffix)
EXT = .cpython-310-x86_64-linux-gnu.so

#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = snake_game
OBJ_NAME_BINDING = snakeGame

#This is the target that compiles our executable
all : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME) && ./${OBJ_NAME}

debug : $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

bindings :
	$(CC) -O3 -Wall -shared -std=c++20 -fPIC $(INC) $(OBJS_BINDING) $(LINKER_FLAGS) -o $(OBJ_NAME_BINDING)$(EXT)

clean :
	rm $(OBJ_NAME) $(OBJ_NAME_BINDING)$(EXT)