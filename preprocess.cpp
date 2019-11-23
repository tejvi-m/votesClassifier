#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<random>

using namespace std;

int computeHammingDistance(vector<char> point1, vector<char> point2){
  //shoudl i include the target variabel?
  //if comparing with a missign vlaue, dont coutn the missing value if the
  //target values dont match

  //not tested.
  int size = point1.size();
  int mismatches = 0;
  for(int i = 0; i < size; i++){

    //should only be run on imputed data? not sure how to handle this
    //greater the distance, the more dissimilar they are
    //also looking at the label for this?

    if(!(point1[i] == point2[i] || point1[i] == '?' || point2[i] == '?')) mismatches++;
  }
  return mismatches;
}

vector<bool> getMax(const vector<vector<char>>& dataset){

  int numRows = dataset.size();
  int numCols = dataset[0].size() - 1;

  vector<float> percentages(numCols, 0);
  vector<bool> maxVals(numCols, false);

  for(int col = 0; col < numCols; col++){
    for(int row = 0; row < numRows; row++){
      if(dataset[row][col] != '?'){
        if(dataset[row][col] == 'y') percentages[col] += 1;
      }
    }
  }

  for(int i = 0; i < numCols; i++){
    percentages[i] /= float(numRows);
    if(percentages[i] > 0.5) maxVals[i] = true;
  }

  return maxVals;
}

void fillMax(vector<vector<char>>& dataset){
    vector<bool> maxVals = getMax(dataset);

    int numRows = dataset.size();
    int numCols = dataset[0].size();

    for(int row = 0; row < numRows; row++){
      for(int col = 0; col < numCols - 1; col++){
        if (dataset[row][col] == '?'){
          if(maxVals[col]) dataset[row][col] = 'y';
          else dataset[row][col] = 'n';
        }
      }
    }
}

void printData(const vector<vector<char>>& dataset){
  int numRows = dataset.size();
  int numCols = dataset[0].size();

  for(int row = 0; row < numRows; row++){
    for(int col = 0; col < numCols; col++){
      cout << dataset[row][col] << " ";
    }
    cout << endl;
  }

}

void shuffleAndSplit(vector<vector<char>>& dataset, vector<vector<char>>& train, vector<vector<char>>& test, int split){

  srand(time(NULL));
  unsigned seed = rand();
  int size = dataset.size();

  shuffle(dataset.begin(), dataset.end(), default_random_engine(seed));

  copy(dataset.begin(), dataset.begin() + size - (dataset.size() * split) / 100, train.begin());
  copy(dataset.begin() + size - (dataset.size() * split) / 100, dataset.end(), test.begin());

}

vector<vector<vector<char>>> openFile(string filename){
  ifstream file;
  string data;

  vector<vector<char>> dataset;
  vector<char> temp;
  file.open(filename);

  while(file >> data){
    //data contains the full line
    int i = 0;
    temp = {};

    while(data[i] != '\0'){
          if(data[i] == 'd' || data[i] == 'r'){
            temp.push_back(data[i]);
            break;
          }
          else if(data[i] != ',' && data[i] != ' ') temp.push_back(data[i]);
          i++;
    }

    dataset.push_back(temp);
    // count++;
  }

  fillMax(dataset);



  //sanity check for hamming distance function
  // cout << computeHammingDistance(dataset[1], dataset[2]);
  // for(int i = 0; i < dataset[1].size(); i++){
  //   cout << dataset[1][i] << " " << dataset[2][i] << endl;
  // }

  file.close();

  int split = 20;
  vector<vector<char>> train(dataset.size() - (dataset.size() * split) / 100), test((dataset.size() * split) / 100);

  //TODO return this train and test and do further ops on them

  // shuffleAndSplit(dataset, train, test, 20);
  // printData(train);
  // cout<<"test" << endl << endl<< endl << endl << endl;
  // printData(test);
  //
  // cout << train.size() << " " << test.size();
  vector<vector<vector<char>>> final;
  final.push_back(dataset);
  final.push_back(train);
  final.push_back(test);

  return final;
}

int KFold(int n, int split, vector<vector<char>>& dataset){
  vector<vector<char>> train(dataset.size() - (dataset.size() * split) / 100), test((dataset.size() * split) / 100);
  for(int i = 0; i < n; i++){

    shuffleAndSplit(dataset, train, test, 20);
  }

  return 0;
}
//
// int main(){
//   openFile("./data/votesData.txt");
//   return 0;
// }
