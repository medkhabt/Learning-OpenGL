INC=-I./include

C_SRC=src/gl.c
CPP_SRC=src/myprog.cpp

# I shouldn't forget about the debugging flag. 
CFLAGS=-Wall -g
CPPFLAGS=-Wall -g


LIBS=-lglfw -framework Cocoa -framework OpenGL -framework IOKit


default: all 

all: run 

glad.o : $(C_SRC)
	clang $(CFLAGS) $(INC) -c $< -o $@
myprog.o : $(CPP_SRC)
	clang++ $(CPPFLAGS) $(INC) -c $< -o $@

myprog: glad.o myprog.o 
	clang++ $(CPPFLAGS) -o myprog glad.o myprog.o $(LIBS) 

run : myprog
	./myprog

clean: 
	rm myprog
