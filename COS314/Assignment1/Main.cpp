#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <ctime>
#include <cmath>
#include <limits>

using namespace std;

const int NUM_CAMPUSES = 5;
const int MAX_ITERATIONS = 1000;
const int MAX_IMPROVEMENTS = 100;
const string campuses[NUM_CAMPUSES] = {"Hatfield", "Hillcrest", "Groenkloof", "Prinshof", "Mamelodi"};

// Cost matrix
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
    for (size_t i = 0; i < route.size() - 1; ++i) {
        totalDistance += distances[route[i]][route[i+1]];
    }
    totalDistance += distances[route.back()][route[0]];  // Return to starting Hatfield
    return totalDistance;
}

//! Hill Climbing algorithm
vector<int> hillClimb(const vector<int>& solution) {
    vector<int> currentRoute = solution;
    int currentDistance = calculateDistance(currentRoute);
    //! This variable will check for consecutive improvements
    int improvements = 0;

    while (improvements < MAX_IMPROVEMENTS) {
        bool improved = false;
        for (size_t i = 1; i < currentRoute.size() - 1; i++) {
            for (size_t j = i + 1; j < currentRoute.size(); j++) {
                vector<int> newRoute = currentRoute;
                swap(newRoute[i], newRoute[j]);
                int newDistance = calculateDistance(newRoute);
                if (newDistance < currentDistance) {
                    currentRoute = newRoute;
                    currentDistance = newDistance;
                    improved = true; //* Improvement made
                    improvements = 0; //* Reset consecutive iterations without improvement
                }
            }
        }
        //* If no improvement -> no swap made
        //! Terminations basically, if it reaches MAX_IMPROVEMENTS then stuck on local optima
        if (!improved)
            improvements++;
    }

    return currentRoute; 
}

// Main Iterated Local Search function
vector<int> iteratedLocalSearch() {
    vector<int> currentSolution = generateInitialSolution();
    vector<int> bestSolution = currentSolution;
    int bestDistance = calculateDistance(bestSolution);
    //* Random seed generation
    default_random_engine rng(time(nullptr));

    for (int i = 0; i < MAX_ITERATIONS; i++) {
        //Perturbation routes by swapping two campuses
        vector<int> perturbedRoute = bestSolution;
        // Range of 1 to NUM_CAMPUSES - 1
        uniform_int_distribution<int> distribution(1, NUM_CAMPUSES - 1); //! Exclude Hatfield and last index
        // Generate two random indexes in the distribution
        int index1 = distribution(rng);
        int index2 = distribution(rng);
        // Perform perturbation
        swap(perturbedRoute[index1], perturbedRoute[index2]);
        
        // Perform HillClimb on perturbed solution
        vector<int> newRoute = hillClimb(perturbedRoute);
        int newDistance = calculateDistance(newRoute);

        // Accept if newRoute is an improvement
        if (newDistance < bestDistance) {
            bestSolution = newRoute;
            bestDistance = newDistance;
        }
    }
    return bestSolution;
}

int main() {
    vector<int> bestRoute = iteratedLocalSearch();
    
    // Print the best route found
    cout << "Best Route (ILS with Hill Climbing): ";
    for (size_t i = 0; i < bestRoute.size(); i++) {
        if (i < bestRoute.size() -1)
            cout << campuses[i] << " -> ";
        else 
            cout << campuses[i] << " -> Hatfield";
    }

    cout << endl;
    cout << "Shortest Distance: " << calculateDistance(bestRoute) << endl;

    return 0;
}