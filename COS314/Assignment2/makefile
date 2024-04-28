TARGET = knapsack
CC = g++
CFLAGS = -std=c++17 -O2  # Common flags

all: $(TARGET)

$(TARGET): Main.cpp
	$(CC) $(CFLAGS) Main.cpp -o $(TARGET)

run:
	./knapsack

clean:
	rm -f *.o $(TARGET) 

valgrind: $(TARGET) # Valgrind target dependency
	valgrind --leak-check=full ./$(TARGET)	