#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <string>
#include <__algorithm/shuffle.h>

using namespace std;

//! Constants for data
const int numCampuses = 5;
const string campuses[numCampuses] = {
  "Hatfield",
  "Hillcrest",
  "Groenkloof",
  "Prinshof",
  "Mamelodi"
};

const int distances[numCampuses][numCampuses] = {
  {0, 15, 20, 22, 30},
  {15, 0, 10, 12, 25},
  {20, 10, 0, 8, 22},
  {22, 12, 8, 0, 18},
  {30, 25, 22, 18, 0}
};

//* Calculate distance
int getDistance(const vector<int>& vec) {
  int distance = 0;
  for (int i = 0; i < vec.size() - 1; i++) {
    distance += distances[vec[i]][vec[i+1]];
  }
  distance += distances[vec.back()][vec[0]];
  return distance;
}

//* Generate random initial solution
vector<int> generateSolution() {
  vector<int> solution(numCampuses);
  iota(solution.begin(), solution.end(), 0); // Fill solution with 0,1,2,...,numCampuses-1

  cout << "Original solution before shuffling: " << endl;
  for (int i: solution)
    cout << i << " is: " << solution.at(i) << endl;
  // Generate random number generator
  random_device rd;
  mt19937 g(rd());

  // Shuffle solutionn except the starting point
  shuffle(solution.begin() + 1, solution.end(), g);

  cout << "Solution after shuffling: " << endl;
  for (int i: solution)
    cout << i << " is: " << solution.at(i) << endl;

  return solution;
}

vector<int> ILS() {
  vector<int> bestSolution = generateSolution();
  int bestDistance = getDistance(bestSolution);

  return bestSolution;
}

int main () {
  cout << "Hello World!" << endl;
  vector<int> bestRouteILS = ILS();
  // vector<int> bestRouteSA = SA();

  cout << "Best Route (ILS): ";
  for (int campusIndex : bestRouteILS) {
    cout << campuses[campusIndex] << " ";
  }
  cout << endl;
  cout << "Shortest Distance (ILS): " << getDistance(bestRouteILS) << endl;

  // cout << "Best Route (SA): ";
  // for (int campusIndex : bestRouteSA) {
  //   cout << campuses[campusIndex] << " ";
  // }
  // cout << endl;
  // cout << "Shortest Distance (SA): " << calcDistance(bestRouteSA) << endl;

  return 0;
}