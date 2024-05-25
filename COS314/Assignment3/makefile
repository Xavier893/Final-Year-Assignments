# Compiler and flags
CXX = g++  # Use g++ as the C++ compiler
CXXFLAGS = -std=c++11 -Wall  # Use C++11 standard, enable all warnings

# Targets
all: neural_network   # Default target to build the executable

neural_network: ANN.cpp
	$(CXX) $(CXXFLAGS) ANN.cpp -o neural_network

run: neural_network
	./neural_network

clean:
	rm -f neural_network
