CXX = g++
CXXFLAGS = -Wall -Iinclude

SRC = src/main.cpp src/parser.cpp src/execute.cpp src/builtins.cpp

myshell: $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o myshell

clean:
	rm -f myshell
