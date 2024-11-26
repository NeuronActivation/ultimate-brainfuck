CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -pedantic
TARGET = ultimate_brainfuck
SRC = main.cc bf_interpreter.cc

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
