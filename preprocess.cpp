#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<random>

using namespace std;

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



//despite the name, it fills in the value with the max frequency

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

template <class T>
void printData(const vector<vector<T>>& dataset){
  int numRows = dataset.size();
  int numCols = dataset[0].size();

  for(int row = 0; row < numRows; row++){
    for(int col = 0; col < numCols; col++){
      cout << dataset[row][col] << " ";
    }
    cout << endl;
  }

  cout << endl;

}

void shuffle(vector<vector<char>>& dataset){
  srand(time(NULL));
  unsigned seed = rand();
  int size = dataset.size();

  shuffle(dataset.begin(), dataset.end(), default_random_engine(seed));
}

void saveDataset(string filename, vector<vector<char>>& dataset){

    ofstream outFile;

    string line;

    outFile.open(filename);
    int i = 0, j = 0;
    int col = dataset[0].size();
    int row = dataset.size();

    while (outFile) {
        if(i == row)
          break;

        while(j < col){
          line += dataset[i][j++];
          line += " ";
        }
        i++;
        j = 0;

        outFile << line << endl;
        line = "";
    }

}

vector<vector<char>> openFile(string filename){
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
  }

  fillMax(dataset);

  file.close();

  saveDataset("./data/cleanedDataset.txt", dataset);
  return dataset;
}
