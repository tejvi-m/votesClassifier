#include <iostream>
#include <vector>
// #include "preprocess.cpp"
// #include "bagging.cpp"

using namespace std;

vector<vector<pair<double, double>>> getProbabilities(const vector<vector<char>>& dataset){
  int size = dataset.size() - 1;
  int columns = dataset[0].size() - 1;
  vector<vector<pair<double, double>>> probabilities (3, vector<pair<double, double>>(columns, make_pair(0, 0)));

  // first row for the republicans
  // second for the democrats
  // third is for the unconditional probability


  for(int row = 0; row < size; row++){
    for(int col = 0; col < columns; col++){
      if(dataset[row][col] == 'n'){
          if(dataset[row][columns] == 'r'){
              probabilities[0][col].first++;
          }
          else{
            probabilities[1][col].first++;
          }
          probabilities[2][col].first++;
      }


    else{
      if(dataset[row][columns] == 'r'){
          probabilities[0][col].second++;
        }
      else{
        probabilities[1][col].second++;
        }
        probabilities[2][col].second++;
      }
    }
  }

  int d = 0, r = 0;
  for(int i = 0; i < size; i++){
    if(dataset[i][columns] == 'r') r++;
    else d++;
  }

  for(int j = 0; j < columns; j++){
    probabilities[0][j].first /= r;
    probabilities[0][j].second /= r;
  }

  for(int j = 0; j < columns; j++){
    probabilities[1][j].first /= d;
    probabilities[1][j].second /= d;
  }


  for(int j = 0; j < columns; j++){
    probabilities[2][j].first /= size;
    probabilities[2][j].second /= size;
  }

  vector<pair<double, double>> apriori{make_pair(r/(double)(r + d), d/(double)(r+d))};
  probabilities.push_back(apriori);
  
  return probabilities;
}

char predict(const vector<char>& test, const vector<vector<pair<double, double>>>& probs){
  double prob1 = 1;
  for(int i = 0; i < probs[0].size(); i++){
    if(test[i] == 'n'){
      prob1 *= probs[0][i].first;
    }
    else{
      prob1 *= probs[0][i].second;
    }
  }

  double prob2 = 1;
  for(int i = 0; i < probs[0].size(); i++){
    if(test[i] == 'n'){
      prob2 *= probs[1][i].first;
    }
    else{
      prob2 *= probs[1][i].second;
    }
  }


  prob1 = prob1 * probs[3][0].first;
  prob2 = prob2 * probs[3][0].second;

  if(prob1 > prob2) return 'r';
  return 'd';
}


vector<vector<int>> getConfusionMatrix(const vector<vector<char>>& test, const vector<char>& predictions){

  int testSize = test.size();
  vector<vector<int>> matrix(2, vector<int>(2, 0));
  for(int i = 0; i < testSize; i++){
    char p = predictions[i];
    if(p == 'r' && p == test[i][test[0].size() - 1])
    matrix[0][0]++;
    else if(p == 'd' && p == test[i][test[0].size() - 1])
    matrix[1][1]++;
    else if(p == 'r' && p != test[i][test[0].size() - 1])
    matrix[0][1]++;
    else if(p == 'd' && p != test[i][test[0].size() - 1])
    matrix[1][0]++;
  }

  return matrix;
}

pair<double, vector<vector<int>>> evaluate(const vector<vector<char>>& testData, const vector<vector<pair<double, double>>>& probs){
  int count = 0;
  int size = testData.size();
  vector<char> predictions;

  for(int i = 0; i < size; i++){
    char p = predict(testData[i], probs);
    predictions.push_back(p);
    if(p == testData[i][testData[0].size() - 1]) count++;
  }

  return make_pair(count/ (double) size, getConfusionMatrix(testData, predictions));
}


pair<double, vector<vector<int>>> learnAndEvaluate(const vector<vector<char>>& train, const vector<vector<char>>& test){
  vector<vector<pair<double, double>>> probs = getProbabilities(train);
  pair<double, vector<vector<int>>> scores = evaluate(test, probs);
  return scores;
}


vector<double> getMetrics(vector<vector<int>> confMatrix){
    vector<double> metrics;
    double precision, recall, F1;

    //recall: TP / TP + FN
    recall = confMatrix[1][1] / (double)(confMatrix[1][1] + confMatrix[1][0]);
    metrics.push_back(recall);

    // precision: TP / TP + FP
    precision = confMatrix[1][1] / (double)(confMatrix[1][1] + confMatrix[0][1]);
    metrics.push_back(precision);

    // F measure
    F1 = 2 * precision * recall / (precision + recall);
    metrics.push_back(F1);

    return metrics;
}



pair<vector<double>, double> crossValidate(int n, vector<vector<char>>& dataset){
  vector<double> scores;
  int x = (dataset.size() /  n);

  vector<double> metrics, precisions, recalls, F1s;
  pair<double, vector<vector<int>>> ret;


  shuffle(dataset);

  vector<vector<char>> train;
  vector<vector<char>> test;

  // i guess this will function as  an offset of sort?

  for(int i = 0; i < n; i++){

    // vector<vector<char>> train;
    // vector<vector<char>> test;

    for(int j = 0; j < dataset.size(); j++){
      if(j >= i * x && j < i * x + x){
        test.push_back(dataset[j]);
      }
      else{
        train.push_back(dataset[j]);
      }
    }

    ret = learnAndEvaluate(train, test);
    scores.push_back(ret.first);

    metrics = getMetrics(ret.second);
    recalls.push_back(metrics[0]);
    precisions.push_back(metrics[1]);
    F1s.push_back(metrics[2]);


    train.clear();
    test.clear();
  }

  metrics[0] = accumulate(recalls.begin(), recalls.end(), 0.0) / recalls.size();
  metrics[1] = accumulate(precisions.begin(), precisions.end(), 0.0) / precisions.size();
  metrics[2] = accumulate(F1s.begin(), F1s.end(), 0.0) / F1s.size();

  return make_pair(metrics, accumulate(scores.begin(), scores.end(), 0.0) / scores.size()) ;

}
