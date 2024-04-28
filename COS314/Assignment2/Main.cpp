#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;

struct Item {
  int value;
  int weight;
};

struct ProblemInstance {
  int optimum;
  string name;
  int noItems;
  int maxWeight;
  vector<Item> items;
  vector<vector<int>> population;
};

ProblemInstance loadInstance(const string& filename) {
  ProblemInstance newInstance;

  ifstream infile(filename);
  if (!infile.is_open()) 
    cerr << "Could not open file " << filename << endl;

  string line;

  //* First line
  getline(infile, line);

  istringstream iss(line);
  iss >> newInstance.noItems >> newInstance.maxWeight;

  for (int i = 0; i < newInstance.noItems; i++) {
    Item item;
    getline(infile, line);

    istringstream iss_item(line);
    iss_item >> item.value >> item.weight;

    newInstance.items.push_back(item);
  }

  return newInstance;
}

vector<int> generateRandomChromosome(int numItems, default_random_engine& generator) {
  vector<int> chromosome(numItems, 0);

  for (int i = 0; i < numItems; i++)
    chromosome.at(i) = generator() % 2;

  return chromosome;
}

void initialPopulation(ProblemInstance& instance, int populationSize, default_random_engine& generator) {
  instance.population.clear();
  for (int i = 0; i < populationSize; i++)
    instance.population.push_back(generateRandomChromosome(instance.noItems, generator));
}

int calculateFitness(const vector<int>& chromosome, const ProblemInstance& instance) {
  int totalValue = 0;
  int totalWeight = 0;

  for (int i = 0; i < chromosome.size(); i++) {
    if (chromosome.at(i) == 1) {
      totalValue += instance.items.at(i).value;
      totalWeight += instance.items.at(i).weight;
    }
  }

  if (totalWeight > instance.maxWeight) {
    //! Penalty to reduce totalValue
    double excess = totalWeight - instance.maxWeight;
    totalValue -= excess * 5;
  }

  return totalValue;
}

vector<int> getFitnessValues(const vector<vector<int>>& population, const ProblemInstance& instance) {
  vector<int> fitnessValues;

  for (const vector<int>& chromosome : population)
    fitnessValues.push_back(calculateFitness(chromosome, instance));

  return fitnessValues;
}

vector<vector<int>> selectFitterIndividuals(const vector<vector<int>>& population, const vector<int>& fitnessValues, default_random_engine& generator) {
  int size = 3;
  int populationSize = population.size();
  vector<vector<int>> selectedParents;

  //! CHANGE i++ to i += 2... previous i++ didn't select best parent
  for (int i = 0; i < populationSize; i += 2) {
    vector<int> tournamentIndexes;

    for (int j = 0; j < size; j++) 
      tournamentIndexes.push_back(generator() % populationSize);
    
    int bestParent1 = tournamentIndexes.at(0);
    int bestParent2 = tournamentIndexes.at(1);

    for (int j = 2; j < tournamentIndexes.size(); j++) {
      // cout << "j: " << j << ", tournamentIndexes.size(): " << tournamentIndexes.size() << endl;
      if (fitnessValues.at(tournamentIndexes.at(j)) > fitnessValues.at(bestParent1)) {
        bestParent2 = bestParent1;
        bestParent1 = tournamentIndexes.at(j);
      } else if (fitnessValues.at(tournamentIndexes.at(j)) > fitnessValues.at(bestParent2)) {
        bestParent2 = tournamentIndexes.at(j);
      }

      selectedParents.push_back(population.at(bestParent1));
      selectedParents.push_back(population.at(bestParent2));
    }
  }

  return selectedParents;
}

vector<vector<int>> crossOver(const vector<vector<int>>& selectedParents, default_random_engine& generator) {
  vector<vector<int>> offspring;

  for (int i = 0; i < selectedParents.size() - 1; i += 2) { //! Go through pairs of parents
    int chromosomeLength = selectedParents.at(i).size();
    int crossoverIndex = generator() % chromosomeLength;

    vector<int> offspringOne(chromosomeLength);
    vector<int> offspringTwo(chromosomeLength);

    for (int j = 0; j < chromosomeLength; j++) {
      if (j < crossoverIndex) {
        offspringOne.at(j) = selectedParents.at(i).at(j);
        offspringTwo.at(j) = selectedParents.at(i + 1).at(j);
      } else {
        offspringOne.at(j) = selectedParents.at(i + 1).at(j);
        offspringTwo.at(j) = selectedParents.at(i).at(j);
      }
    }

    offspring.push_back(offspringOne);
    offspring.push_back(offspringTwo);
  }

  //! Case when parents are uneven
  if (selectedParents.size() % 2 == 1) {
    vector<int> leftoverParent = selectedParents.back();
    vector<int> newOffspring;

    for (int i = leftoverParent.size() - 1; i >= 0; i--)
      newOffspring.push_back(leftoverParent.at(i));
    
    offspring.push_back(newOffspring);
  }

  return offspring;
}

void mutate(vector<vector<int>>& offspring, default_random_engine& generator) {
  double mutation = 0.05;

  for (vector<int>& chromosome : offspring) {
    int chromosomeLength = offspring.at(0).size();
    for (int i = 0; i < chromosomeLength; i++) {
      if (generator() / (double) generator.max() < mutation) 
        chromosome.at(i) = chromosome.at(i) == 0 ? 1 : 0;
    }
  }
}

int getWorstFitness(const vector<int>& fitnessValues) {
  int worst = 0;
  for (int i = 1; i < fitnessValues.size(); i++) {
    if (fitnessValues.at(i) < fitnessValues.at(worst))
      worst = i;
  }

  return worst;
}

int selectRandomOffspring(const vector<vector<int>>& offspring, default_random_engine& generator) {
  return generator() % offspring.size();
}

vector<vector<int>> generateNewPopulation(const vector<vector<int>>& oldPopulation, const vector<vector<int>>& offspring, const vector<int>& fitnessScores, default_random_engine& generator) {
 int replaceNumber = 4;
 vector<vector<int>> newPopulation = oldPopulation;

  for (int i = 0; i < replaceNumber; i++) {
    int worstIndex = getWorstFitness(fitnessScores);
    int offspringIndex = selectRandomOffspring(offspring, generator);
    newPopulation.at(worstIndex) = offspring.at(offspringIndex);
  }

  return newPopulation;
}

void printResultRow(const ProblemInstance& instance, const string& algo, int seed, int bestFitness, double runtime) {
  cout << left;
  cout << setw(25) << instance.name;
  cout << setw(15) << algo;
  cout << setw(15) << seed;
  cout << setw(15) << bestFitness;
  cout << setw(15) << instance.optimum;
  cout << setw(15) << runtime << endl;
}

void printResultRowILS(const ProblemInstance& instance, const string& algo, int seed, int bestFitness, double runtime) {
  cout << left;
  cout << setw(25) << "";
  cout << setw(15) << algo;
  cout << setw(15) << seed;
  cout << setw(15) << bestFitness;
  cout << setw(15) << "";
  cout << setw(15) << runtime << endl << endl;
}

void geneticAlgorithm(ProblemInstance& instance, default_random_engine& generator, unsigned long seed) {
  //* Step 1: Initial Population
  int populationSize = 50;
  initialPopulation(instance, populationSize, generator);

  //* Step 2: Calculate Fitness of each individual
  vector<int> fitnessValues = getFitnessValues(instance.population, instance);

  int bestFitness = 0;

  //* Step 3: Start while loop
  int maxGenerations = 100;

  auto startTime = chrono::high_resolution_clock::now();

  for (int i = 0; i < maxGenerations; i++) {
    //* Step 4: Select fitter individuals
    vector<vector<int>> selectedParents = selectFitterIndividuals(instance.population, fitnessValues, generator);

    //* Step 5: Recombine individuals
    vector<vector<int>> offspring = crossOver(selectedParents, generator);

    //* Step 6: Mutate individuals
    mutate(offspring, generator);

    //* Step 7: Evaluate fitness of all individuals
    fitnessValues = getFitnessValues(offspring, instance);

    //* Step 8: Generate a new population
    instance.population = generateNewPopulation(instance.population, offspring, fitnessValues, generator);

    for (int fitness : fitnessValues) {
      if (fitness > bestFitness)
        bestFitness = fitness;
    }
  }

  auto endTime = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
  double runtimeSeconds = duration.count() / 1000000.0;

  printResultRow(instance, "GA", seed, bestFitness, runtimeSeconds);
}

void applyPerturbation(vector<int>& chromosome) {
    random_device rd;
    mt19937 g(rd());
    int numItemsToFlip = g() % chromosome.size() / 2;  

    for (int i = 0; i < numItemsToFlip; i++) {
        int index = g() % chromosome.size();
        chromosome.at(index) = chromosome.at(index) == 0 ? 1 : 0;  
    }
}

vector<int> hillClimb(const vector<int>& chromosome, const ProblemInstance& instance) {
  vector<int> currentSolution = chromosome;
  int currentFitness = calculateFitness(currentSolution, instance);
  bool improved = true;

  while (improved) {
    improved = false;

    for (size_t i = 0; i < currentSolution.size(); i++) {  

      vector<int> newSolution = currentSolution;
      newSolution.at(i) = newSolution.at(i) == 0 ? 1 : 0; 

      int newFitness = calculateFitness(newSolution, instance);

      // cout << "NewFitness is: " << newFitness << " and currentFitness is: " << currentFitness << endl;
      if (newFitness > currentFitness) {
        currentSolution = newSolution;
        currentFitness = newFitness;
        improved = true;
      }
    }
  }

  return currentSolution;
}


void geneticAlgorithmWithILS(ProblemInstance& instance, default_random_engine& generator, unsigned long seed) {
  //* Step 1: Initial Population
  int populationSize = 50;
  initialPopulation(instance, populationSize, generator);

  //* Step 2: Calculate Fitness of each individual
  vector<int> fitnessValues = getFitnessValues(instance.population, instance);

  int bestFitness = 0;

  //* Step 3: Start while loop
  int maxGenerations = 100;

  auto startTime = chrono::high_resolution_clock::now();

  for (int i = 0; i < maxGenerations; i++) {
    //* Step 4: Select fitter individuals
    vector<vector<int>> selectedParents = selectFitterIndividuals(instance.population, fitnessValues, generator);

    //* Step 5: Recombine individuals
    vector<vector<int>> offspring = crossOver(selectedParents, generator);

    //* Step 6: Mutate individuals
    mutate(offspring, generator);

    //! ILS introduced
    for (auto& off: offspring) {
      for (int j = 0; j < 100; j++) {
        applyPerturbation(off);
        hillClimb(off, instance);
      }
    }

    //* Step 7: Evaluate fitness of all individuals
    fitnessValues = getFitnessValues(offspring, instance);

    //* Step 8: Generate a new population
    instance.population = generateNewPopulation(instance.population, offspring, fitnessValues, generator);

    for (int fitness : fitnessValues) {
      if (fitness > bestFitness)
        bestFitness = fitness;
    }
  }

  auto endTime = chrono::high_resolution_clock::now();
  auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime);
  double runtimeSeconds = duration.count() / 1000000.0;

  printResultRowILS(instance, "GA + ILS", seed, bestFitness, runtimeSeconds);
}

int main() {
  //* Random Seed Generation
  unsigned long seed = chrono::system_clock::now().time_since_epoch().count();
  default_random_engine generator(seed);
  // cout << "Seed value: " << seed << endl;

  vector<string> filenames = {
    "f1_l-d_kp_10_269",
    "f2_l-d_kp_20_878",
    "f3_l-d_kp_4_20",
    "f4_l-d_kp_4_11",
    "f5_l-d_kp_15_375",
    "f6_l-d_kp_10_60",
    "f7_l-d_kp_7_50",
    "f8_l-d_kp_23_10000",
    "f9_l-d_kp_5_80",
    "f10_l-d_kp_20_879",
    "knapPI_1_100_1000_1"
  };

  vector<int> knownOptimums = {
    295,
    1024,
    35,
    23,
    4810694,
    52,
    107,
    9767,
    130,
    1025,
    9147
  };

  vector<ProblemInstance> instances;
  vector<ProblemInstance> lsInstances;

  for (int i = 0; i < filenames.size(); i++) {
    ProblemInstance instance = loadInstance(filenames.at(i));
    instance.name = filenames.at(i);
    instance.optimum = knownOptimums.at(i);
    instances.push_back(instance);
    lsInstances.push_back(instance);
  } 

  //* Table Header
  cout << left;
  cout << setw(25) << "Problem Instance";
  cout << setw(15) << "Algorithm";
  cout << setw(15) << "Seed";
  cout << setw(15) << "Best Solution";
  cout << setw(15) << "Known Optimum";
  cout << setw(15) << "Runtime (seconds)" << endl;
  cout << "------------------------------------------------------------------------------------------------------\n";

  for (int i = 0; i < instances.size(); i++) {
    geneticAlgorithm(instances.at(i), generator, seed);
    geneticAlgorithmWithILS(lsInstances.at(i), generator, seed);
  }

  for (ProblemInstance instance: instances) {
  }

  for (ProblemInstance instance: lsInstances) {
  }

  return 0;
}