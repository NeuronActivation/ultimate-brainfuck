C = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic
TARGET = ultimate_brainfuck
SRC = main.c

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC)

clean:
	rm -f $(TARGET)
