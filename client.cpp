#include <iostream>
#include "bagging.cpp"


int main(){
  vector<vector<char>> dataset = openFile("./data/votesData.txt");

  vector<string> labels{"recall: ", "precision: ", "F1: "};

  pair<vector<double>, double> ret = crossValidate(5, dataset);

  cout << "without bagging (averages): \naccuracy: " << ret.second << endl;
  for(int i = 0; i < ret.first.size(); i++){
    cout << labels[i] << ret.first[i] << endl;
  }

  cout << endl;

  pair<vector<double>, double> retBagging = crossValidateWithBagging(5, dataset, 15, 50);

  cout << "with bagging (averages):\naccuracy: " << retBagging.second << endl;
  for(int i = 0; i < retBagging.first.size(); i++){
    cout << labels[i] << retBagging.first[i] << endl;
  }

  cout << endl;


  return 0;
}
