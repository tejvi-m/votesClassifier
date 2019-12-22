#include <iostream>
#include <vector>
#include <random>
#include <iterator>
#include <algorithm>
#include <experimental/algorithm>
#include "preprocess.cpp"
#include "naiveBayes.cpp"

using namespace std;

vector<vector<vector<char>>> getBaggedData(const vector<vector<char>>& trainData, int numBags, int sampleSize){
    vector<vector<vector<char>>> bags;

    for(int i = 0; i < numBags; i++){
      vector<vector<char>> bag;
      std::experimental::fundamentals_v2::sample(trainData.begin(), trainData.end(), back_inserter(bag), sampleSize, mt19937{random_device{}()});
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

pair<double, vector<vector<int>>> learnFromBagsAndEvaluate(const vector<vector<char>>& train, const vector<vector<char>>& testData, int bags, int sampleSize){

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
    char p = voteAndPredict(testData[i], probabilities);
    predictions.push_back(p);
    if(p == testData[i][testData[0].size() - 1]) count++;
  }


  score = count / (double) testData.size();

  return make_pair(score, getConfusionMatrix(testData, predictions));

}

pair<vector<double>, double> crossValidateWithBagging(int n, vector<vector<char>>& dataset, int bags, int sampleSize){
    vector<double> scores;
    int x = (dataset.size() /  n);

    vector<double> metrics, precisions, recalls, F1s;
    pair<double, vector<vector<int>>> ret;

    shuffle(dataset);

    vector<vector<char>> train;
    vector<vector<char>> test;
    
    // TODO rewrite this part - have separate vectors for each of the splits
    for(int i = 0; i < n; i++){
      for(int j = 0; j < dataset.size(); j++){
        if(j >= i * x && j < i * x + x){
          test.push_back(dataset[j]);
        }
        else{
          train.push_back(dataset[j]);
        }
      }


      ret = learnFromBagsAndEvaluate(train, test, bags, sampleSize);
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
