#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string inputFileName = "Hashin.txt";
string out1FileName = "Out1.txt";
string outFinalFileName = "OutFinal.txt";
int blockSize = 32;
int expandedBlockSize = 48;
int expansionTable[8][6] = {
  {32, 1, 2, 3, 4, 5},
  {4, 5, 6, 7, 8, 9},
  {8, 9, 10, 11, 12, 13},
  {12, 13, 14, 15, 16, 17},
  {16, 17, 18, 19, 20, 21},
  {20, 21, 22, 23, 24, 25},
  {24, 25, 26, 27, 28, 29},
  {28, 29, 30, 31, 32, 1}
};

int substitutionTable[4][16] = {
  {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
  {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
  {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
  {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
};

string getCorrespondingBinary(int val){
  switch(val){
    case 0:
      return "0000";
    case 1:
      return "0001";
    case 2:
      return "0010";
    case 3:
      return "0011";
    case 4:
      return "0100";
    case 5:
      return "0101";
    case 6:
      return "0110";
    case 7:
      return "0111";
    case 8:
      return "1000";
    case 9:
      return "1001";
    case 10:
      return "1010";
    case 11:
      return "1011";
    case 12:
      return "1100";
    case 13:
      return "1101";
    case 14:
      return "1110";
    default:
      return "1111";
    }
}

int getBinaryValue(char binary){
  if(binary == '1'){
    return 1;
  }
  return 0;
}

string readFile(string fileName){
  ifstream inputFileStream;
  string hashInput = "";
  inputFileStream.open(inputFileName);
  if(inputFileStream.good()){
    inputFileStream >> hashInput;
  }
  inputFileStream.close();
  return hashInput;
}

bool isInvalidInput(string hashInput){
  int length = hashInput.length();
  if(length == 0 || length % 32 != 0){
    return true;
  }
  return false;
}

int getNumberOfBlocks(string hashInput){
  int length = hashInput.length();
  return (int)(length/blockSize);
}

string* partitionBlocks(string hashInput, int numOfBlocks){
  string* blocks = new string[numOfBlocks];
  for(int i = 0; i < numOfBlocks; i++){
    blocks[i] = hashInput.substr(i*blockSize, blockSize);
  }
  return blocks;
}

string performExpansion(string currentBlock){
  string expandedBlock="";
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 6; j++){
      int index = expansionTable[i][j];
      expandedBlock += currentBlock.at(index - 1);
    }
  }
  return expandedBlock;
}

void writeFile(string fileName, string binary){
  ofstream OutputFile(fileName);
  OutputFile << binary;
  OutputFile.close();
}

string performSubstitution(string expandedBlock){
  string substitute = "";
  string subBlock = "";
  for(int index = 0; index < 8; index++){
    subBlock = expandedBlock.substr(index*6, 6);
    int i = 2*getBinaryValue(subBlock.at(0)) + getBinaryValue(subBlock.at(5));
    int j = stoi(subBlock.substr(1, 4), 0, 2);
    int value = substitutionTable[i][j];
    substitute += getCorrespondingBinary(value);
  }
  return substitute;
}

int getXOR(int a, int b){
  if((a == 0 && b == 0) || (a == 1 && b == 1)){
    return 0;
  }
  else{
    return 1;
  }
}

string calculateXOR(string block1, string block2){
  if(block1.length() == 0){
    return block2;
  }
  if(block2.length() == 0){
    return block1;
  }
  string result = "";
  for(int i = 0; i < 32; i++){
    int bit1 = getBinaryValue(block1.at(i));
    int bit2 = getBinaryValue(block2.at(i));
    result += to_string(getXOR(bit1, bit2));
  }
  return result;
}

string calculateXORofOtherBlocks(string *blocks, int index, int numOfBlocks){
  string XORResult = "";
  if(numOfBlocks == 1){
    XORResult = blocks[0];
  }
  for(int i = 0; i < numOfBlocks; i++){
    if(i != index){
      XORResult = calculateXOR(XORResult, blocks[i]);
    }
  }
  return XORResult;
}

string* performESOperation(string* blocks, int numOfBlocks){
  string* result = new string[numOfBlocks];
  string expandedBlock;
  for(int i = 0; i < numOfBlocks; i++){
    expandedBlock = performExpansion(blocks[i]);
    result[i] = performSubstitution(expandedBlock);
  }
  return result;
}

string calculateXORofAllBlocks(string *blocks, int numOfBlocks){
  string XORResult = "";
  for(int i = 0; i < numOfBlocks; i++){
    XORResult = calculateXOR(XORResult, blocks[i]);
  }
  return XORResult;
}

string *performSingleRoundOperations(string* blocks, int numOfBlocks){
  string* XORBlocks = new string[numOfBlocks];
  string* ESBlocks = performESOperation(blocks, numOfBlocks);
  for(int i = 0; i < numOfBlocks; i++){
    XORBlocks[i] = calculateXORofOtherBlocks(ESBlocks, i, numOfBlocks);
  }
  return XORBlocks;
}

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
  cout << result << endl;
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
