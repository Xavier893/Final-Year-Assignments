#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct DataPoint {
  vector<double> features = vector<double>(8);
  int class_label;
};

void normalizeData(vector<DataPoint> &data, const vector<double> &minValues,
                   const vector<double> &maxValues) {
  // HACK: normalize features
  for (DataPoint &point : data) {
    for (int i = 0; i < 8; i++) {
      double range = maxValues[i] - minValues[i];
      if (range != 0.0) {
        point.features[i] = (point.features[i] - minValues[i]) / range;
      } else {
        point.features[i] = 0.5;
      }
    }
  }
}

void calculateMinMaxValues(const vector<DataPoint> &data,
                           vector<double> &minValues,
                           vector<double> &maxValues) {
  for (const DataPoint &point : data) {
    for (int i = 0; i < 8; i++) {
      minValues[i] = min(minValues[i], point.features[i]);
      maxValues[i] = max(maxValues[i], point.features[i]);
    }
  }
}

vector<DataPoint> readData(const string &filename) {
  vector<DataPoint> data;
  ifstream file(filename);

  string line;
  getline(file, line);

  while (getline(file, line)) {
    DataPoint point;
    stringstream ss(line);

    for (int i = 0; i < 8; i++) {
      getline(ss, line, ',');
      point.features[i] = stod(line);
    }
    getline(ss, line, ',');
    point.class_label = stoi(line);

    data.push_back(point);
  }

  file.close();
  return data;
}

class NeuralNetwork {
public:
  NeuralNetwork(int inputSize, int hiddenSize, int outputSize,
                double learningRate)
      : inputSize(inputSize), hiddenSize(hiddenSize), outputSize(outputSize),
        learningRate(learningRate) {

    random_device rd;
    mt19937 gen(rd());
    // HACK: random values between -0.5 and 0.5 for the weights and biases
    uniform_real_distribution<> dis(-0.5, 0.5);

    weightsInputHidden =
        vector<vector<double>>(inputSize, vector<double>(hiddenSize));
    weightsHiddenOutput =
        vector<vector<double>>(hiddenSize, vector<double>(outputSize));
    biasHidden = vector<double>(hiddenSize);
    biasOutput = vector<double>(outputSize);

    for (int i = 0; i < hiddenSize; i++) {
      for (int j = 0; j < outputSize; j++) {
        weightsHiddenOutput[i][j] = dis(gen);
      }
      biasHidden[i] = dis(gen);
    }

    for (int i = 0; i < outputSize; i++) {
      biasOutput[i] = dis(gen);
    }
  }

  double sigmoid(double x) { return 1.0 / (1.0 + exp(-x)); }

  double sigmoidDerivative(double x) { return sigmoid(x) * (1.0 - sigmoid(x)); }

  void trainNeuralNetwork(vector<DataPoint> &trainingData, int epochs) {
    vector<double> minValues(8, numeric_limits<double>::max());
    vector<double> maxValues(8, numeric_limits<double>::lowest());
    calculateMinMaxValues(trainingData, minValues, maxValues);

    for (int epoch = 0; epoch < epochs; epoch++) {
      random_device rd;
      mt19937 g(rd());
      shuffle(trainingData.begin(), trainingData.end(), g);
      double error = 0.0;
      hiddenLayerOutput = vector<double>(hiddenSize, 0.0);
      predictedOutput = vector<double>(outputSize, 0.0);

      for (const DataPoint &point : trainingData) {

        // TODO: Step 3: Feedforward learning

        for (int i = 0; i < hiddenSize; i++) {
          double sum = biasHidden[i];
          for (int j = 0; j < inputSize; j++) {
            sum += point.features[j] * weightsInputHidden[j][i];
          }
          hiddenLayerOutput[i] = sigmoid(sum);
        }

        for (int i = 0; i < outputSize; i++) {
          double sum = biasOutput[i];
          for (int j = 0; j < hiddenSize; j++) {
            sum += hiddenLayerOutput[j] * weightsHiddenOutput[j][i];
          }
          predictedOutput[i] = sigmoid(sum);
        }

        int predictedClass = (predictedOutput[0] > 0.50) ? 1 : 0;

        // TODO: Step 4: Backpropagation of error

        // HACK: Output layer error calc
        outputErrors = vector<double>(outputSize);
        for (int i = 0; i < outputSize; i++) {
          outputErrors[i] = (point.class_label - predictedClass) *
                            sigmoidDerivative(predictedOutput[i]); // FIX:
        }

        // HACK: HIdden layer error calc
        hiddenErrors = vector<double>(hiddenSize);
        for (int i = 0; i < hiddenSize; i++) {
          double errorSum = 0.0;
          for (int j = 0; j < outputSize; j++) {
            errorSum += outputErrors[j] * weightsHiddenOutput[i][j];
          }
          hiddenErrors[i] = errorSum * sigmoidDerivative(hiddenLayerOutput[i]);
        }

        // TODO: Step 5: Update weights
        for (int i = 0; i < outputSize; i++) {
          biasOutput[i] += learningRate * outputErrors[i];
          for (int j = 0; j < hiddenSize; j++) {
            weightsHiddenOutput[j][i] +=
                learningRate * outputErrors[i] * hiddenLayerOutput[j];
          }
        }

        for (int i = 0; i < hiddenSize; i++) {
          biasHidden[i] += learningRate * hiddenErrors[i];
          for (int j = 0; j < inputSize; j++) {
            weightsInputHidden[j][i] +=
                learningRate * hiddenErrors[i] * point.features[j];
          }
        }

        // TODO: Error calc
        double instanceError = 0.0;
        for (int i = 0; i < outputSize; i++) {
          double err = point.class_label - predictedOutput[i];
          instanceError += err * err;
        }
        instanceError /= outputSize;
        error += instanceError;
      }
      error /= trainingData.size();
      cout << "Epoch " << epoch + 1 << ": Error = " << error << endl;
    }
  }

  int predict(const DataPoint &point) {
    vector<double> hiddenOutput(hiddenSize);
    for (int i = 0; i < hiddenSize; i++) {
      double sum = biasHidden[i];
      for (int j = 0; j < inputSize; j++) {
        sum += point.features[j] * weightsInputHidden[j][i];
      }
      hiddenOutput[i] = sigmoid(sum);
    }

    vector<double> outputLayerOutput(outputSize);
    for (int i = 0; i < outputSize; i++) {
      double sum = biasOutput[i];
      for (int j = 0; j < hiddenSize; j++) {
        sum += hiddenOutput[j] * weightsHiddenOutput[j][i];
      }
      outputLayerOutput[i] = sigmoid(sum);
    }

    return (outputLayerOutput[0] > 0.50) ? 1 : 0;
  }

private:
  int inputSize;
  int hiddenSize;
  int outputSize;
  double learningRate;
  vector<vector<double>> weightsInputHidden, weightsHiddenOutput;
  vector<double> biasHidden, biasOutput;
  vector<double> predictedOutput, hiddenLayerOutput;
  vector<double> outputErrors, hiddenErrors;
};

int main() {
  vector<DataPoint> trainingData = readData("mushroom_train.csv");

  vector<double> minValues(8, numeric_limits<double>::max());
  vector<double> maxValues(8, numeric_limits<double>::lowest());
  calculateMinMaxValues(trainingData, minValues, maxValues);

  normalizeData(trainingData, minValues, maxValues);

  random_device rd;
  mt19937 g(rd());
  shuffle(trainingData.begin(), trainingData.end(), g);

  int inputSize = 8; // 8 features of training data
  int hiddenSize = 12;
  int outputSize = 1;
  double learningRate = 0.01;
  int epochs = 500;

  NeuralNetwork nn(inputSize, hiddenSize, outputSize, learningRate);
  nn.trainNeuralNetwork(trainingData, epochs);

  // NOTE: Testing data and accuracy
  vector<DataPoint> testingData = readData("mushroom_test.csv");

  normalizeData(testingData, minValues, maxValues);

  int correctPredictions = 0;
  for (const DataPoint &point : testingData) {
    int predictedLabel = nn.predict(point);
    if (predictedLabel == point.class_label) {
      correctPredictions++;
    }
  }
  double accuracy = (double)correctPredictions / testingData.size() * 100.0;
  cout << "Test Accuracy: " << accuracy << "%" << endl;

  return 0;
}
