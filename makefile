SOURCES = Graph.cpp Algorithm.cpp main.cpp
HEADERS = Graph.hpp Algorithm.hpp data_structures.hpp
OBJECTS = $(SOURCES:.cpp=.o)
TARGET = program
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET)

Algorithm.o: Algorithm.cpp Algorithm.hpp data_structures.hpp
	$(CXX) $(CXXFLAGS) -c Algorithm.cpp

Graph.o: Graph.cpp Graph.hpp data_structures.hpp
	$(CXX) $(CXXFLAGS) -c Graph.cpp

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

clean:
	rm -f $(OBJECTS) $(TARGET)

run: $(TARGET)
	./$(TARGET)
