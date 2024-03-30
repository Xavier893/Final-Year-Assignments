#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <random>
#include <ctime>
#include <cmath>
#include <limits>
#include <chrono>

using namespace std;

//! Variables used for ILS and SA averages
vector<int> ilsAverage;
vector<int> saAverage;

const int NUM_CAMPUSES = 5;
const int MAX_ITERATIONS = 1000;
const double T0 = 1000.0;
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

void applyPerturbation(vector<int>& route) {
    random_device rd;
    mt19937 g(rd());
    shuffle(route.begin() + 1, route.end(), g);
}

//! Hill Climbing algorithm
vector<int> hillClimb(const vector<int>& solution) {
    vector<int> currentSolution = solution;
    int currentDistance = calculateDistance(currentSolution);
    bool improved = true;

    while (improved) {
        improved = false;
        for (size_t i = 1; i < currentSolution.size() - 1; i++) {
            for (size_t j = i + 1; j < currentSolution.size(); j++) {

                // Vector storing new solution
                vector<int> newSolution = currentSolution;

                // Shuffling of campuses 
                random_device rd;
                mt19937 g(rd());
                shuffle(newSolution.begin() + 1, newSolution.end() - 1, g); // Shuffle the middle of the route to check for improvements

                int newDistance = calculateDistance(newSolution);

                if (newDistance < currentDistance) {
                    currentSolution = newSolution;
                    currentDistance = newDistance;
                    improved = true;
                }
            }
        }
    }

    return currentSolution;
}

// Main Iterated Local Search function
vector<int> iteratedLocalSearch() {
    //* Step 1: Generate initial solution
    vector<int> initialSolution = generateInitialSolution();

    //* Step 2+3: Apply Local Search to initial solution and find local optimum
    vector<int> improvedSolution = hillClimb(initialSolution);

    //! Termination Criteria = MAX_ITERATIONS
    for (int i = 0; i < MAX_ITERATIONS; i++) {
        //* Step 4: Perturb solution
        applyPerturbation(improvedSolution);

        //* Step 5: Apply Local Search to perturbed solution
        vector<int> newSolution = hillClimb(improvedSolution);

        //* Update current solution if new solution is better
        if (calculateDistance(newSolution) < calculateDistance(improvedSolution))
            improvedSolution = newSolution;

        ilsAverage.push_back(calculateDistance(improvedSolution));
    }

    return improvedSolution;
}

vector<int> simulatedAnnealing() {
    //* Step 1: Generate initial solution
    vector<int> currentSolution = generateInitialSolution();
    vector<int> bestSolution = currentSolution;

    double T = T0;
    int t = 1;
    int N = 100;

    while (t <= MAX_ITERATIONS) {
        //* Step 2: Generate neighbour
        vector<int> nextSolution = currentSolution;
        applyPerturbation(nextSolution); //! Using perturbation function

        //* Step 3: Get delta cost
        int deltaCost = calculateDistance(nextSolution) - calculateDistance(currentSolution);

        //* Accept or reject next state based on probability
        if (deltaCost < 0 || exp(-deltaCost / T) > (double)rand() / RAND_MAX) 
            currentSolution = nextSolution;
        
        //* Update current if needed
        if (calculateDistance(currentSolution) < calculateDistance(bestSolution))
            bestSolution = currentSolution;

        t++; // t = t + 1
        T = T0 / log(t+1); // T = T_0 / log(t+1)

        //* No improvement for N iterations
        //* The number of iterations since the last improvement in the best solution is a multiple of N (t % N == 0).
        //* The distance of the current solution is greater than or equal to the distance of the best solution (no improvement for N iterations).
        if (t % N == 0 && calculateDistance(currentSolution) >= calculateDistance(bestSolution))
            break;
    }

    return bestSolution;
}

int main() {
    vector<int> bestRouteILS = iteratedLocalSearch();
    
    // Print the best route found for ILS
    cout << "Best Route (ILS with Hill Climbing): ";
    for (size_t i = 0; i < bestRouteILS.size(); i++) {
        if (i < bestRouteILS.size() -1)
            cout << campuses[bestRouteILS[i]] << " -> ";
        else 
            cout << campuses[bestRouteILS[i]] << " -> Hatfield" << endl;
    }
    cout << "Shortest Distance: " << calculateDistance(bestRouteILS) << endl;

    vector<int> bestRouteSA = iteratedLocalSearch();
    
    // Print the best route found for SA
    cout << "Best Route (Simulated Annealing): ";
    for (size_t i = 0; i < bestRouteSA.size(); i++) {
        if (i < bestRouteSA.size() -1)
            cout << campuses[bestRouteSA[i]] << " -> ";
        else 
            cout << campuses[bestRouteSA[i]] << " -> Hatfield" << endl;
    }
    cout << "Shortest Distance: " << calculateDistance(bestRouteSA) << endl;

    return 0;
}