OBJS = snek.cpp

OBJ_NAME = snek
all : $(OBJS)
	g++ $(OBJS) -w -lSDL2 -lSDL2 -lSDL2_ttf -lSDL2_image -o $(OBJ_NAME)
