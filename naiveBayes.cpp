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

  if(prob1 > prob2) return 'r';
  return 'd';
}

double evaluate(const vector<vector<char>>& testData, const vector<vector<pair<double, double>>>& probs){
  int count = 0;
  int size = testData.size();

  for(int i = 0; i < size - 1; i++){
    char p = predict(testData[i], probs);
    if(p == testData[i][testData[0].size() - 1]) count++;
  }

  return count/ (double) size;
}

double learnAndEvaluate(const vector<vector<char>>& train, const vector<vector<char>>& test){
  vector<vector<pair<double, double>>> probs = getProbabilities(train);
  double score = evaluate(train, probs);
  return score;
}


pair<vector<double>, double> crossValidate(int n, vector<vector<char>>& dataset, int split){
  vector<double> scores;
  int x = (dataset.size() * split) / 100;
  vector<vector<char>> train(dataset.size() - x + 1);
  vector<vector<char>> test(x);

  // vector<vector<char>> train, test;
  for(int i = 0; i < n; i++){
    shuffleAndSplit(dataset, train, test, 20);
    scores.push_back(learnAndEvaluate(train, test));
  }

  return make_pair(scores, accumulate(scores.begin(), scores.end(), 0.0) / scores.size()) ;
}


//
// int main(){
//   vector<vector<char>> dataset = openFile("./data/votesData.txt");
//   vector<vector<pair<double, double>>> probs = getProbabilities(dataset);
//
//   pair<vector<double>, double> ret = crossValidate(10, dataset, 20);
//
//   for(int i = 0; i < 10; i++){
//     cout << ret.first[i] << endl;
//   }
//
//   cout << "average: " << ret.second << endl;
//   return 0;
// }
