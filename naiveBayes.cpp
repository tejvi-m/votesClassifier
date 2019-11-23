#include <iostream>
#include <vector>
#include "preprocess.cpp"

using namespace std;

vector<vector<pair<double, double>>> getProbabilities(const vector<vector<char>>& dataset){
  int size = dataset.size();
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

    for(int i = 0; i < size; i++){
      char p = predict(testData[i], probs);
      cout << p << " ";
      cout << testData[i][testData[0].size() - 1] << endl;
      if(p == testData[i][testData[0].size() - 1]) count++;
    }

    return count/ (double) size;
}



int main(){
  vector<vector<vector<char>>> data = openFile("./data/votesData.txt");
  vector<vector<char>> dataset = data[0];
  vector<vector<char>> train = data[1];
  vector<vector<char>> test = data[2];
  shuffleAndSplit(dataset, train, test, 20);
  vector<vector<pair<double, double>>> probs = getProbabilities(train);

  cout << probs.size() << endl;
  for(int i = 0; i < probs.size(); i++){
    for(int j = 0; j < probs[0].size(); j++){
      cout << probs[i][j].first << ", " << probs[i][j].second << "   ";
    }
    cout << endl;
  }

  cout << evaluate(test, probs);

  // cout << predict(dataset[105], probs) << endl;
  // cout << dataset[105][dataset[0].size() - 1] << endl;
  //
  // cout << "score: " << evaluate(dataset, probs) << endl;
  // KFold(10, 20, dataset);

  return 0;
}
