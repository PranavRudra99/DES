#include <iostream>
#include <fstream>
#include <string>
#include<iomanip>
#include<sstream>

#include "util.h"

using namespace std;

string AFRfileName = "AFR.txt";

string getVariation(string hashInput, int index){
  string variation = "";
  if(index != 0){
    variation += hashInput.substr(0, index);
  }
  char character = hashInput.at(index);
  if(character == '0'){
    variation += "1";
  }
  else{
    variation += "0";
  }
  variation += hashInput.substr(index + 1);
  return variation;
}

int numOfFlips(string binary1, string binary2){
  int count = 0;
  for(int i = 0; i < 32; i++){
    char bit1 = binary1.at(i);
    char bit2 = binary2.at(i);
    if(bit1 != bit2){
      count++;
    }
  }
  return count;
}

float calculateAverageFlipRate(string hashInput){
  int length = hashInput.length();
  string originalHashOutput = getMTUHash(hashInput);
  int totalDifferenceCount = 0;
  int totalLength = 0;
  for(int i = 0; i < length; i++){
    string variation = getVariation(hashInput, i);
    string variationHashOutput = getMTUHash(variation);
    int currentVariationHashDiff = numOfFlips(originalHashOutput, variationHashOutput);
    totalDifferenceCount += currentVariationHashDiff;
    totalLength += 32;
  }
  double ratio = (float)totalDifferenceCount/(float)totalLength;
  ratio = ratio*100;
  return ratio;
}

string getFlipRateWithFixedPrecision(float ratio){
  stringstream stream;
  stream << fixed << setprecision(1) << ratio;
  string result = stream.str() + "%";
  return result;
}

int main(int argc, char** argv) {
  if(argc > 1){
    inputFileName = argv[1];
  }
  string hashInput = readFile(inputFileName);
  if(isInvalidInput(hashInput)){
    cout << "Empty File or Length Error" << endl;
    return -1;
  }
  float flipRate = calculateAverageFlipRate(hashInput);
  string result = getFlipRateWithFixedPrecision(flipRate);
  writeFile(AFRfileName, result);
}
