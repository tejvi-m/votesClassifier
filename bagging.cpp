#include <iostream>
#include <vector>
#include <random>
#include <iterator>
#include <algorithm>
#include <experimental/algorithm>
// // #include <sample>
#include "preprocess.cpp"
// #include "naiveBayes.cpp"

using namespace std;

vector<vector<vector<char>>> getBaggedData(const vector<vector<char>>& trainData, int numBags, int sampleSize){
    vector<vector<vector<char>>> bags;

    for(int i = 0; i < numBags; i++){
      vector<vector<char>> bag;
      std::experimental::fundamentals_v1::sample(trainData.begin(), trainData.end(), back_inserter(bag), sampleSize, mt19937{random_device{}()});
      bags.push_back(bag);
    }

    return bags;
}

char voteAndPredict(const vector<char>& testData, const vector<vector<vector<pair<double, double>>>>& probs){
    vector<char> predictions;
    int bags = probs.size();

    for(int i = 0; i < bags; i++){
      predictions.push_back(predict(testData, probs[i]));
    }

    if(count(predictions.begin(), predictions.end(), 'd') > (bags / 2 + 1)) return 'd';
    else return 'r';
}

double learnFromBagsAndEvaluate(const vector<vector<char>>& train, const vector<vector<char>>& testData, int bags, int sampleSize){

  vector<vector<vector<char>>> trainData = getBaggedData(train, bags, sampleSize);
  vector<vector<vector<pair<double, double>>>> probabilities;
  vector<char> predictions;
  int count = 0;
  double score;

  for(int i = 0; i < bags; i++){
    vector<vector<pair<double, double>>> probs = getProbabilities(trainData[i]);
    probabilities.push_back(probs);
  }

  for(int i = 0; i < testData.size(); i++){
    if(voteAndPredict(testData[i], probabilities) == testData[i][testData[0].size() - 1]) count++;
  }


  score = count / (double) testData.size();

  return score;

}
//
// int main(){
//   vector<vector<char>> dataset = openFile("./data/votesData.txt");
//   int split = 20;
//   int x = (dataset.size() * split) / 100;
//   vector<vector<char>> train(dataset.size() - x + 1);
//   vector<vector<char>> test(x);
//   shuffleAndSplit(dataset, train, test, 20);
//   // vector<vector<vector<char>>> bags = getBaggedData(train, 5, 100);
//   double score = learnFromBagsAndEvaluate(train, test, 5, 100);
//
//   cout << score << endl;
// }
