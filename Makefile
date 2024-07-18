INC=-I./include -I/usr/local/include/freetype2

C_SRC=$(wildcard src/*.c)
CPP_SRC=$(wildcard src/*.cpp)
CPP_TST=$(wildcard tests/*.cpp)

# I shouldn't forget about the debugging flag. 
CFLAGS=-Wall -g
CPPFLAGS= -std=c++11 -Wall -g


LIBS=-lglfw -framework Cocoa -framework OpenGL -framework IOKit -lfreetype


C_OBJS=$(C_SRC:.c=.o)
CPP_OBJS=$(CPP_SRC:.cpp=.o)
CPP_TST_OBJS=$(CPP_TST:.cpp=.o)

default: all 

all: clean run 

%.o: %.c 
	clang $(CFLAGS) $(INC) -c $< -o $@

%.o: %.cpp 
	clang++ $(CPPFLAGS) $(INC) -c $< -o $@


build : $(C_OBJS) $(CPP_OBJS) 
	clang++ $(CPPFLAGS) -o myprog $(C_OBJS) $(CPP_OBJS) $(LIBS) 

build_test: $(C_OBJS) $(CPP_TST_OBJS) 
	clang++ $(CPPFLAGS) -o mytest $(C_OBJS) $(CPP_TST_OBJS) $(LIBS) 

run : build 
	./myprog

test: build_test
	./mytest

clean: 
	rm -f  myprog mytest $(C_OBJS) $(CPP_OBJS) $(CPP_TST_OBJS)
