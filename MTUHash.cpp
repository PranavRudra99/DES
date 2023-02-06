#include <iostream>
#include <fstream>
#include <string>

#include "util.h"

using namespace std;

string MTUHash(string hashInput){
  int numOfBlocks = getNumberOfBlocks(hashInput);
  string* blocks = partitionBlocks(hashInput, numOfBlocks);
  string result = "";
  for(int i = 0; i < 16; i++){
    blocks = performSingleRoundOperations(blocks, numOfBlocks);
    if(i == 0){
      writeFile(out1FileName, blocks[0]);
    }
  }
  result = calculateXORofAllBlocks(blocks, numOfBlocks);
  writeFile(outFinalFileName, result);
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
  MTUHash(hashInput);
}
