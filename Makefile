CXX = g++
CXXFLAGS = -Wall -g

SRC = src/main.cpp src/parser.cpp src/execute.cpp src/builtins.cpp
OBJ = $(SRC:.cpp=.o)

myshell: $(OBJ)
	$(CXX) $(OBJ) -o myshell

clean:
	rm -f $(OBJ) myshell
