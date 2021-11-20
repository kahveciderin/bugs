CXX = g++
CPPFLAGS= -Wall -std=c++17 -g
LINKS = -lSDL2 -lSDL2_image
CPP_SRC=$(wildcard *.cpp)
CPP_OBJ=$(CPP_SRC:.cpp=.o)
OBJS= $(CPP_OBJ) $(C_OBJ)

all: $(OBJS)
	$(CXX) $^ $(LINKS) -o main
	$(RM) $(OBJS)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $< -c -o $@

clean:
	$(RM) main $(OBJS)