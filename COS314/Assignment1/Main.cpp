#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <ctime>
#include <cmath>
#include <limits>

using namespace std;

// Define constants
const int NUM_CAMPUSES = 5;
const string campuses[NUM_CAMPUSES] = {"Hatfield", "Hillcrest", "Groenkloof", "Prinsof", "Mamelodi"};
const int distances[NUM_CAMPUSES][NUM_CAMPUSES] = {
    {0, 15, 20, 22, 30},
    {15, 0, 10, 12, 25},
    {20, 10, 0, 8, 22},
    {22, 12, 8, 0, 18},
    {30, 25, 22, 18, 0}
};

vector<int> generateInitialSolution() {
    vector<int> solution;
    for (int i = 0; i < 5; i++) 
        solution.push_back(i);
    
    //* Generate random
    random_device rd;
    mt19937 g(rd()); 
    
    //* Shuffle random solution (except Hatfield)
    shuffle(solution.begin() + 1, solution.end(), g);

    return solution;
}

// Calculate total distance for a route
int calculateDistance(const vector<int>& route) {
    int totalDistance = 0;
    for (int i = 0; i < route.size() - 1; ++i) {
        totalDistance += distances[route[i]][route[i+1]];
    }
    totalDistance += distances[route.back()][route[0]];  // Return to starting Hatfield
    return totalDistance;
}

//! Hill Climbing algorithm

vector<int> hillClimb(const vector<int>& solution, int perturbations) {
    vector<int> currentSolution = solution;
    int currentCost = calculateDistance(currentSolution);
    bool improved = true;
    int counter = 0;

    while (improved) {
        improved = false;
        for (int i = 1; i < currentSolution.size() - 1; i++) {
            for (int j = i + 1; i < currentSolution.size(); j++) {
                vector<int> newSolution = currentSolution;
                swap(newSolution[i], newSolution[j]);
                int newCost = calculateDistance(newSolution);
                if (newCost < currentCost) {
                    currentSolution = newSolution;
                    currentCost = newCost;
                    improved = true;
                }
            }
        } 

        // Perturbation part of hill climbing
        counter++;

        //! Choosing only to perform perturbation when counter is divisible by perturbations
        //TODO: see if you can optimize this to only perturbate if the shuffle is better than the current solution
        if (counter % perturbations == 0) {
            random_device rd;
            mt19937 g(rd()); 
            shuffle(currentSolution.begin() + 1, currentSolution.end() - 1, g);
            currentCost = calculateDistance(currentSolution);
        }
    }

    return currentSolution;
}

// Main Iterated Local Search function
vector<int> iteratedLocalSearch(int maxIterations, int perturbations) {
    vector<int> currentSolution = generateInitialSolution();
    int currentCost = calculateDistance(currentSolution);

    //* Variables to store the best solution and cost
    vector<int> bestSolution = currentSolution;
    int bestCost = currentCost;

    for (int i = 0; i < maxIterations; i++) {
        currentSolution = hillClimb(currentSolution, perturbations);
        currentCost = calculateDistance(currentSolution);

        if (currentCost < bestCost) {
            bestSolution = currentSolution;
            bestCost = currentCost;
        }
    }

    cout << "I am here\n";

    return bestSolution;
}

int main() {
    srand((time(nullptr))); // Random seed generator

    int maxIterations = 2000;
    int perturbations = 50;

    vector<int> bestRoute = iteratedLocalSearch(maxIterations, perturbations);

    // Print the best route found
    cout << "Best Route (ILS with Tabu Search): ";
    for (int campusIndex : bestRoute) {
        cout << campuses[campusIndex] << " ";
    }
    cout << endl;
    cout << "Shortest Distance: " << calculateDistance(bestRoute) << endl;

    return 0;
}