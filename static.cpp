/*****************************************************
COMP4160 Assignment 1
Student Name: Ho Yat Lui
Student Number: 7780269
 *****************************************************/
#include "node.h"
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>
#define SIZE 256

Node *buildingTree(int *);
void init(int *, string);
void writeFile(vector<unsigned long>, int, FILE *);
void compress(string, string);
void writeMeta(FILE *fp, int *freqTable, int totalSymbol, int appearedSymbol);
void decompress(string, string);
void fileError(FILE *fp, string filename);
void bin(vector<unsigned long>);

int main(int argc, char **argv) {
  if (argc < 4) { // if too little arguments
    cout << "Please provide command." << endl;
    return 1;
  }
  if (*argv[1] == 'c') {        // if with 'c' flag, compress
    compress(argv[2], argv[3]); // argv[2] = input, argv[3] = output
    return 0;
  }
  if (*argv[1] == 'd') {          // if with 'd' flag, decompress
    decompress(argv[2], argv[3]); // argv[2] = input, argv[3] = output
    return 0;
  } else { // if nothing apply
    cout << "Wrong Command." << endl;
    return 1;
  }
}

//decompress inputFileName to outputFileName
void decompress(string inputFileName, string outputFileName) {
  cout << "Decompressing file " << inputFileName << endl << endl;
  cout << "Alphabatic Frequencies:" << endl;

  FILE *fpI = fopen(inputFileName.c_str(), "rb"); // open file input
  fileError(fpI, inputFileName);                  // make sure file exist

  int *freqTable = new int[SIZE];  // freqeuncey table
  std::fill_n(freqTable, SIZE, 0); // fill with 0
  int totalSybol = 0;
  int appearedSybol = 0;

  fread(&totalSybol, sizeof(int), 1, fpI); // read number of symbol from file
  // read number of symbol in alphabet that appeared
  fread(&appearedSybol, sizeof(int), 1, fpI);

  for (int i = 0; i < appearedSybol; i++) { // read freqeuncey table from file
    uint8_t tempSymbol = 0;
    int tempFreq = 0;
    fread(&tempSymbol, sizeof(uint8_t), 1, fpI);
    fread(&tempFreq, sizeof(int), 1, fpI);
    freqTable[tempSymbol] = tempFreq;
  }
  for (int i = 0; i < SIZE; i++) { // print freqeuncey table
    if (freqTable[i] != 0) {
      cout << i << ": " << freqTable[i] << endl;
    }
  }

  Node *root = buildingTree(freqTable); // build tree

  FILE *fpO = fopen(outputFileName.c_str(), "wb"); // open output file

  uint8_t currentByte = 0;
  // read byte from input until EOF
  while (fread(&currentByte, sizeof(uint8_t), 1, fpI) == 1) {
    // traverse coding tree to decode
    vector<uint8_t> resultingBytes = traverse_de(root, currentByte, totalSybol);
    for (uint8_t i : resultingBytes) {
      // output decoded byte to file
      fwrite(&i, sizeof(uint8_t), 1, fpO);
    }
  }

  fclose(fpI);
  fclose(fpO);
  cout << "Decompression completed.Result is stored in file " << outputFileName
       << endl;
}

//compress inputFileName into outputFileName
void compress(string inputFileName, string outputFileName) {
  cout << "Compressing file " << inputFileName << endl << endl;
  cout << "Alphabatic Frequencies:" << endl;
  // 2d array for codewordtable, vec[0] is codeword
  // vec[1] is length of codeword
  vector<vector<unsigned long>> codewordTable(SIZE,
                                              vector<unsigned long>(2, 0));

  int *freqTable = new int[SIZE];
  int totalSybol = 0;
  float averageLength = 0;
  int appearedSymbol = 0;

  init(freqTable, inputFileName); // initialize freqency table

  for (int i = 0; i < SIZE; i++) { // print freqency table
    if (freqTable[i] != 0) {
      totalSybol += freqTable[i];
      appearedSymbol++;
      cout << i << ": " << freqTable[i] << endl;
    }
  }
  Node *root = buildingTree(freqTable); // building code tree
  // traverse codetree to fill codewordTable
  root->traverse(0, 0, codewordTable);

  cout << "\nSymbol and their codewords: " << endl;
  for (int i = 0; i < SIZE; i++) { // print codewordTable
    if (codewordTable[i][1] != 0) {
      cout << "symbol:" << i << ", code:";
      bin(codewordTable[i]); // print binary
      cout << endl;
      //sum averageLength
      averageLength +=
          ((float)freqTable[i] / (float)totalSybol) * codewordTable[i][1];
    }
  }

  printf("\nAverage codeword length is %.2f bits.\n", averageLength);

  // string outputName = fileName + ".huff";

  FILE *fpO = fopen(outputFileName.c_str(), "wb"); // open files
  fileError(fpO, outputFileName);
  FILE *fpI = fopen(inputFileName.c_str(), "rb");
  fileError(fpI, inputFileName);

  writeMeta(fpO, freqTable, totalSybol, appearedSymbol);//write metadata

  int c = fgetc(fpI);
  while (c != EOF) { //for each symbol in input
    //write the right code to file
    writeFile(codewordTable[c], totalSybol, fpO); 
    c = fgetc(fpI);
  }

  fclose(fpI);
  fclose(fpO);

  cout << "Compression completed. Result is stored in file " << outputFileName
       << endl;
}

// write meta data including freqeuncey table, total number of symbol,
// and number of appeared symbol in the file
void writeMeta(FILE *fp, int *freqTable, int totalSymbol, int appearedSymbol) {
  fwrite(&totalSymbol, sizeof(int), 1, fp);    // write total number of symbol
  fwrite(&appearedSymbol, sizeof(int), 1, fp); // write total number of appeared symbol
  for (int i = 0; i < SIZE; i++) {  // write frequency table
    if (freqTable[i] != 0) {
      uint8_t sybol = i;
      fwrite(&sybol, sizeof(uint8_t), 1, fp);
      fwrite(&freqTable[i], sizeof(int), 1, fp);
    }
  }
}

//write codeword into file byte by byte
void writeFile(vector<unsigned long> codeword, int maxSymbol, FILE *fp) {
  static uint8_t buffer = 0;  // buffer, 8 bit
  static int pos = 0;  // current position of buffer
  static int count = maxSymbol;  // number of symbol left

  for (int i = codeword[1] - 1; i >= 0; i--) {
    uint8_t bit = (codeword[0] >> i) & 1;  // take msb bit
    bit <<= pos;  // move to right postion
    buffer |= bit;  // add into buffer
    pos++; 
    if (pos == 8) {  // if buffer is full
      fputc(buffer, fp); // write buffer to file
      buffer = 0;  // reset
      pos = 0;
    }
  }
  count--;
  // if all symbol is finished and there's something left in buffer
  if (count == 0) {  
    fputc(buffer, fp);
  }
}

// print number in binary according to the length
void bin(vector<unsigned long> vec) { 
  for (int i = vec[1] - 1; i >= 0; i--) {
    int result = (vec[0] >> i) & 1;
    cout << result;
  }
}

// fill up freqeuncy table
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

//building code tree
Node *buildingTree(int *freq) {
  priority_queue<Node *, vector<Node *>, compareNode> priQ;//init priority queue
  Node *result;  // resulting tree's root node
  for (int i = 0; i < SIZE; i++) { // create all leaf node, add the queue
    if (freq[i] != 0) {
      Node *temp = new Node(freq[i], i, NULL, NULL);
      priQ.push(temp);
    }
  }
  while (priQ.size() > 1) { //while there's more than 1 tree in queue
    Node *temp1 = priQ.top(); // take 2 tree out
    priQ.pop();
    Node *temp2 = priQ.top();
    priQ.pop();
    Node *newRoot = merge(temp1, temp2); //merge them
    priQ.push(newRoot); // push back into queue
  }

  assert(priQ.size() == 1);

  result = priQ.top(); //return the only tree left

  return result;
}

// check for file error
void fileError(FILE *fp, string filename) {
  if (fp == NULL) {
    cout << "Cannot open file: " << filename << endl;
    exit(1);
  }
}
