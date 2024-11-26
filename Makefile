CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic
TARGET = ultimate-brainfuck
SRC = main.cc bf-interpreter.cc

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
