#include "node.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>
#define SIZE 256

Node *buildingTree(int *);
void init(int *, string);
void writeFile(uint8_t, int, FILE *);
int coutBits(uint8_t);
void compress(string,string);
void writeMeta(FILE *fp, int *freqTable, int totalSymbol, int appearedSymbol);
void decompress(string, string);
void fileError(FILE* fp,string filename);

int main(int argc, char** argv) {
  if( argc < 4){
    cout << "Please provide command." << endl;
    return 1;
  }
  if( *argv[1] == 'c'){
    compress(argv[2],argv[3]);
    return 0;
  }
  if (*argv[1] == 'd') {
    decompress(argv[2], argv[3]);
    return 0;
  }
  else {
    cout << "Wrong Command." << endl;
    return 1;
  }

}

void decompress(string inputFileName, string outputFileName){
  cout << "Decompressing file " << inputFileName << endl << endl;
  cout << "Alphabatic Frequencies:" << endl;

  FILE* fpI = fopen(inputFileName.c_str(), "rb");
  fileError(fpI, inputFileName);

  int *freqTable = new int[SIZE];
  std::fill_n(freqTable, SIZE, 0);
  int totalSybol = 0;
  int appearedSybol = 0;

  fread(&totalSybol, sizeof(int), 1, fpI);
  fread(&appearedSybol, sizeof(int), 1, fpI);
  //cout << "Decompress total: " << totalSybol << endl;
  //cout << "Decompress appeared: " << appearedSybol << endl;
  for(int i = 0 ; i < appearedSybol ; i++){
    uint8_t tempSymbol = 0;
    int tempFreq = 0;
    fread(&tempSymbol, sizeof(uint8_t), 1, fpI);
    fread(&tempFreq, sizeof(int), 1, fpI);
    freqTable[tempSymbol] = tempFreq;
  }
  for(int i = 0 ; i < SIZE ; i++){
    if (freqTable[i] != 0) {
      cout << i << ": " << freqTable[i] << endl;
    }
  }

  Node *root = buildingTree(freqTable);

  //string newFileName = inputFileName.replace(inputFileName.size()-4,4,"puff");
  FILE *fpO = fopen(outputFileName.c_str(), "wb");

  uint8_t currentByte = 0;
  while (fread(&currentByte, sizeof(uint8_t), 1, fpI) == 1) {
    std::vector<uint8_t> resultingBytes = traverse_de(root, currentByte, totalSybol);
    for(uint8_t i : resultingBytes){
      fwrite(&i, sizeof(uint8_t), 1, fpO);
    }
  }

  fclose(fpI);
  fclose(fpO);
  cout << "Decompression completed.Result is stored in file " << outputFileName << endl;
}

void compress(string inputFileName, string outputFileName) {
  cout << "Compressing file " << inputFileName << endl << endl;
  cout << "Alphabatic Frequencies:" << endl;
  uint8_t *codewordTable = new uint8_t[SIZE];
  std::fill_n(codewordTable, SIZE, 1);
  int *freqTable = new int[SIZE];
  int totalSybol = 0;
  float averageLength = 0;
  int appearedSymbol = 0;

  init(freqTable, inputFileName);

  for (int i = 0; i < SIZE; i++) {
    if (freqTable[i] != 0) {
      totalSybol += freqTable[i];
      appearedSymbol++;
      cout << i << ": " << freqTable[i] << endl;
    }
  }
  Node *root = buildingTree(freqTable);
  root->traverse(0, codewordTable);

  cout << "\nSymbol and their codewords: " << endl;
  for (int i = 0; i < SIZE; i++) {
    if (codewordTable[i] != 1) {
      cout << "symbol:" << i << ", code:" << std::bitset<8>(codewordTable[i])
           << endl;
      averageLength += ((float)freqTable[i] / (float)totalSybol) *
                       coutBits(codewordTable[i]);
    }
  }

  printf("\nAverage codeword length is %.2f bits.\n", averageLength);

  //string outputName = fileName + ".huff";

  FILE *fpO = fopen(outputFileName.c_str(), "wb");
  fileError(fpO, outputFileName);
  FILE *fpI = fopen(inputFileName.c_str(), "rb");
  fileError(fpI, inputFileName);

  writeMeta(fpO, freqTable, totalSybol, appearedSymbol);

  int c = fgetc(fpI);
  while (c != EOF) {
    writeFile(codewordTable[c], totalSybol, fpO);
    c = fgetc(fpI);
  }

  fclose(fpI);
  fclose(fpO);

  cout << "Compression completed. Result is stored in file " << outputFileName
       << endl;
}

void writeMeta(FILE *fp, int *freqTable, int totalSymbol, int appearedSymbol) {
  fwrite(&totalSymbol, sizeof(int),1, fp);  // write total number of symbol
  fwrite(&appearedSymbol, sizeof(int), 1, fp); // write total number of symbol
  for(int i = 0 ; i < SIZE ; i++){
    if (freqTable[i] != 0){
      uint8_t sybol = i;
      fwrite(&sybol, sizeof(uint8_t), 1,fp);
      fwrite(&freqTable[i], sizeof(int), 1, fp);
    }
  }
}

void writeFile(uint8_t codeword, int maxSymbol, FILE *fp) {
  static uint8_t buffer = 0;
  static int pos = 0;
  static int count = maxSymbol;

  if (codeword == 0) {
    pos++;
    if (pos == 8) {
      fputc(buffer, fp);
      buffer = 0;
      pos = 0;
    }
  } else {
    for (int i = coutBits(codeword) - 1; i >= 0; i--) {
      uint8_t msb = (codeword >> i) & 1;
      buffer = (msb << pos) | buffer;
      pos++;
      if (pos == 8) {
        fputc(buffer, fp);
        buffer = 0;
        pos = 0;
      }
    }
  }

  count--;
  if (count == 0) {
    // cout << "last byte" << endl;
    fputc(buffer, fp);
  }

  // cout << "buffer after each write: " << std::bitset<8>(buffer) << endl;
  // cout << "pos: " << pos << endl;
  // cout << "count: " << count << endl;
}

int coutBits(uint8_t i) {
  int result;
  if (i == 0) {
    return 1;
  }
  for (result = 0; i > 0; result++) {
    i >>= 1;
  }
  return result;
}
void init(int *freq, string fileName) {
  FILE *fp = fopen(fileName.c_str(), "rb");
  fileError(fp, fileName);
  int c = fgetc(fp);
  while (c != EOF) {
    freq[c]++;
    c = fgetc(fp);
  }
  fclose(fp);
}

Node *buildingTree(int *freq) {
  priority_queue<Node *, vector<Node *>, compareNode> priQ;
  Node *result;
  for (int i = 0; i < SIZE; i++) {
    if (freq[i] != 0) {
      Node *temp = new Node(freq[i], i, NULL, NULL);
      priQ.push(temp);
    }
  }
  while (priQ.size() > 1) {
    Node *temp1 = priQ.top();
    priQ.pop();
    Node *temp2 = priQ.top();
    priQ.pop();
    Node *newRoot = merge(temp1, temp2);
    priQ.push(newRoot);
  }
  result = priQ.top();

  return result;
}

void fileError(FILE* fp, string filename){
  if (fp == NULL) {
    cout << "Cannot open file: " << filename << endl;
    exit(1);
  }
}
