#include <iostream>
#include "bagging.cpp"


int main(){
  vector<vector<char>> dataset = openFile("./data/votesData.txt");

  pair<vector<double>, double> ret = crossValidate(5, dataset);

  for(int i = 0; i < ret.first.size(); i++){
    cout << ret.first[i] << endl;
  }

  cout << endl;

  pair<vector<double>, double> retBagging = crossValidateWithBagging(5, dataset, 15, 50);

  for(int i = 0; i < retBagging.first.size(); i++){
    cout << retBagging.first[i] << endl;
  }

  cout << endl;

  cout << "without bagging : average: " << ret.second << endl;
  cout << "with bagging : average: " << retBagging.second << endl;

  return 0;
}
