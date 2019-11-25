#include <iostream>
#include "bagging.cpp"


int main(){
  vector<vector<char>> dataset = openFile("./data/votesData.txt");

  pair<vector<double>, double> ret = crossValidate(10, dataset);

  for(int i = 0; i < 10; i++){
    cout << ret.first[i] << endl;
  }


  pair<vector<double>, double> retBagging = crossValidateWithBagging(10, dataset, 20, 15, 50);

  for(int i = 0; i < 10; i++){
    cout << retBagging.first[i] << endl;
  }

  cout << "without bagging : average: " << ret.second << endl;
  cout << "with bagging : average: " << retBagging.second << endl;

  return 0;
}
