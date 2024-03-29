# Compiler options
CC = g++
CFLAGS = -Wall -Wextra -g

# Source files
SRCS = Main.cpp

# Executable name
TARGET = main

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

run: $(TARGET)
	./$(TARGET)

valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

clean:
	rm -f $(TARGET)
