INC=-I./include -I/usr/local/include/freetype2

C_SRC=$(wildcard src/*.c)
CPP_SRC=$(wildcard src/*.cpp)
CPP_TST=$(wildcard tests/*.cpp)

# I shouldn't forget about the debugging flag. 
CFLAGS=-Wall 
CDEBUG=-lprofiler -g
CPPFLAGS= -std=c++11 -Wall 
CPPDEBUG= -O0 -lprofiler -g


LIBS=-lglfw -framework Cocoa -framework OpenGL -framework IOKit -lfreetype


C_OBJS=$(C_SRC:.c=.o)
CPP_OBJS=$(CPP_SRC:.cpp=.o)
CPP_TST_OBJS=$(CPP_TST:.cpp=.o)

default: all 

all: clean run 

%.o: %.c 
	clang   $(CFLAGS)  $(INC) -c $< -o $@

profile : $(C_OBJS)
	clang++ $(CPPFLAGS) $(CPPDEBUG) $(INC) -o myprog $(C_OBJS) $(CPP_SRC) $(LIBS) 

build : $(C_OBJS)
	clang++ $(CPPFLAGS) -O3 $(INC) -o myprog $(C_OBJS) $(CPP_SRC) $(LIBS) 2>&1 | less

build_test: $(C_OBJS)
	clang++ $(CPPFLAGS) $(INC) -o mytest $(C_OBJS) $(CPP_TST) $(SRC) 

run : build 
	./myprog

test: build_test
	./mytest

clean: 
	rm -f  myprog mytest $(C_OBJS) $(CPP_OBJS) $(CPP_TST_OBJS)
