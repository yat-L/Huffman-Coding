/*****************************************************
COMP4160 Assignment 1
Student Name: Ho Yat Lui
Student Number: 7780269
 *****************************************************/
#include "dnode.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#define SIZE 256
#define BYTE 8

void fileError(FILE *fp, string filename);
void writeFile(vector<unsigned long> codeword, int maxSymbol, FILE *fp,
               bool nyt);
void compress(string inputFile, string outputFile);
vector<unsigned long> leafToRoot(Node *leaf);
void update(vector<Node *> &symbolList, vector<Node *> &numberList, Node *Y);
void checkAndSwap(vector<Node *> &numberList, Node *current);
vector<unsigned long> fixedCode(uint8_t symbol);
void decompress(string inputFile, string outputFile);

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

// decompress inputFile to outputFile
void decompress(string inputFile, string outputFile) {
  vector<Node *> symbolList(SIZE, NULL);             // list of node by symbol
  vector<Node *> numberList(2 * SIZE + 1 + 1, NULL); // list of node by number
  FILE *fpI = fopen(inputFile.c_str(), "rb");        // open files
  FILE *fpO = fopen(outputFile.c_str(), "wb");
  fileError(fpI, inputFile);
  fileError(fpO, outputFile);

  int numberOfSymbol = 0;
  fread(&numberOfSymbol, sizeof(int), 1, fpI); // read the number of symbol

  // init NYT node, Y node and root
  Node *NYT = new Node(0, 0, (2 * SIZE) + 1, NULL, NULL, NULL);
  numberList[2 * SIZE + 1] = NYT;
  Node *Y = NULL;
  Node *root = NYT;

  uint8_t currentByte = 0;  // the byte currently reading
  Node *currentNode = root; // the node ptr for current node

  // when NYT reached , 8 bits of data will be ignored
  // so that the next eight bit is the fixed code,
  // no movement on the tree
  // since first symbol is always fixed code, it's set
  // when initialize
  int ignoreTimes = BYTE;
  // buffer when reading bit for fixed code
  uint8_t fixedCodeBuffer = 0;

  // read each byte in the compressed file
  while (fread(&currentByte, sizeof(uint8_t), 1, fpI) == 1) {
    // for each bit and until all symbol decoded
    for (int i = 0; (i < BYTE) && (numberOfSymbol > 0); i++) {
      uint8_t lsb = currentByte & 1; // get the lsb
      currentByte >>= 1;             // shift the byte
      if (ignoreTimes == 0) {        // if ifnore is not set
        if (lsb == 1) {              // if lsb = 1, traverse right
          currentNode = currentNode->right;
        } else { // else traverse left
          currentNode = currentNode->left;
        }
        if (currentNode->left == NULL &&
            currentNode->right == NULL) { // if reached leaf node
          if (currentNode == NYT) {       // if leaf node is NYT
            ignoreTimes = BYTE;           // set ingore to 8
          } else {                        // if leaf node is not NYT
            // output symbol
            fwrite(&(currentNode->symbol), sizeof(uint8_t), 1, fpO);
            numberOfSymbol--;                  // decrement numberOfSymbol
            Y = currentNode;                   // set Y to this node
            currentNode = root;                // set currentNode back to root
            update(symbolList, numberList, Y); // update tree
          }
        }
      } else { // reading fixedcode, if ingoreTimes is set
        fixedCodeBuffer <<= 1;
        fixedCodeBuffer |= lsb; // put the bits in buffer
        if (ignoreTimes == 1) { // if buffer is filled
          // output the fixed code
          fwrite(&fixedCodeBuffer, sizeof(uint8_t), 1, fpO);
          numberOfSymbol--;   // decremetn numberOfSymbol
          currentNode = root; // set curretNode back to root
          // create new node for new symbol and new NYT node
          vector<Node *> leftandRight =
              expandNYT(NYT, fixedCodeBuffer, symbolList, numberList);
          NYT = leftandRight[0]; // set the maccordingly
          Y = leftandRight[1];
          update(symbolList, numberList, Y); // update tree
        }
        ignoreTimes--; // decrease ignoreTimes until zero
      }
    }
  }

  fclose(fpI);
  fclose(fpO);
}

// compress inputFile into outputFile
void compress(string inputFile, string outputFile) {
  vector<Node *> symbolList(SIZE, NULL);             // symbol list
  vector<Node *> numberList(2 * SIZE + 1 + 1, NULL); // number list
  FILE *fpI = fopen(inputFile.c_str(), "rb");
  FILE *fpO = fopen(outputFile.c_str(), "wb");
  fileError(fpI, inputFile);
  fileError(fpO, outputFile);
  Node *NYT = new Node(0, 0, 2 * SIZE + 1, NULL, NULL, NULL);
  numberList[2 * SIZE + 1] = NYT;
  Node *Y = NULL;
  // special case for first byte
  bool firstTime = true;

  fseek(fpI, 0L, SEEK_END);               // pointer to the end
  int fileSize = ftell(fpI);              // file size
  rewind(fpI);                            // rewind pointer to the start
  fwrite(&fileSize, sizeof(int), 1, fpO); // write number of symbol in file

  uint8_t currentByte = 0; // current byte we are working on
  // read byte from input until EOF
  while (fread(&currentByte, sizeof(uint8_t), 1, fpI) == 1) {
    if (symbolList[currentByte] == NULL) { // new symbol appeared
      if (firstTime) {                     // first symbol
        writeFile(fixedCode(currentByte), fileSize, fpO,
                  false);  // output fixed code
        firstTime = false; // unset firstTime
      } else {
        // get the NYT code right now
        vector<unsigned long> nytCode = leafToRoot(NYT);
        // write NYT code
        writeFile(nytCode, fileSize, fpO, true);
        // write the fixed code for this byte
        writeFile(fixedCode(currentByte), fileSize, fpO,
                  false); // output fixed code
      }
      // create new node for NYT and new symbol
      vector<Node *> leftandRight =
          expandNYT(NYT, currentByte, symbolList, numberList);
      NYT = leftandRight[0];
      Y = leftandRight[1];
    } else { // if no new symbol
      // get codeword for this symbol
      vector<unsigned long> temp = leafToRoot(symbolList[currentByte]);
      Y = symbolList[currentByte];           // set Y to this node for update
      writeFile(temp, fileSize, fpO, false); // write codeword
    }
    update(symbolList, numberList, Y); // update the tree
  }

  fclose(fpI);
  fclose(fpO);
}

// return fixedCode for the symbol in vector
vector<unsigned long> fixedCode(uint8_t symbol) {
  vector<unsigned long> result = {symbol, BYTE}; // reutrn in vector form
  return result;
}

// update the tree, check for swapping subtree and increment weight
void update(vector<Node *> &symbolList, vector<Node *> &numberList, Node *Y) {
  Node *current = Y;               // start at the Y node
  while (current != NULL) {        // goes up the tree until root
    if (current->parent == NULL) { // if it is root node
      current->freq++;             // add weight
    } else {
      // check if swap is needed, then swap it
      checkAndSwap(numberList, current);
      current->freq++; // add weight
    }
    current = current->parent; // go to parent node
  }
}

// check if subtree need swapping, if needed, swap it
void checkAndSwap(vector<Node *> &numberList, Node *current) {
  // start from the top of numberList
  for (int i = 2 * SIZE + 1; i >= 1; i--) {
    if (numberList[i] != NULL) { // if that node exist
      // if the node has high number
      bool higherNumber = numberList[i]->number > current->number;
      // if the node has same weight
      bool sameWeight = numberList[i]->freq == current->freq;
      // if the node is not the original's parent
      bool notParent = current->parent != numberList[i];
      if (higherNumber && sameWeight && notParent) {
        // if all 3 satify , swap
        swap(current, numberList[i]);
        return; // return so no multiple sapping
      }
    }
  }
  return;
}

// go from leaf to root, record the codeword on the way
vector<unsigned long> leafToRoot(Node *leaf) {
  unsigned long codeword = 0; // codeword that will be returned
  unsigned long pos = 0;
  Node *current = leaf;
  while (current->parent != NULL) {
    Node *parent = current->parent; // go to parent node
    if (parent->left == current) {  // if currentNode is left child of parent
      codeword = codeword | (0 << pos); // record it in codeword
    } else {                            // if currentNode is right child
      codeword = codeword | (1 << pos);
    }
    pos++;
    current = current->parent; // go to parent
  }

  // return vector, pos store the number of bit for
  // this codeword
  vector<unsigned long> result = {codeword, pos};
  return result;
}

// write codeword into file byte by byte
void writeFile(vector<unsigned long> codeword, int maxSymbol, FILE *fp,
               bool nyt) {
  static uint8_t buffer = 0;    // buffer, 8 bit
  static int pos = 0;           // current position of buffer
  static int count = maxSymbol; // number of symbol left

  for (int i = codeword[1] - 1; i >= 0; i--) {
    uint8_t bit = (codeword[0] >> i) & 1; // take msb bit
    bit <<= pos;                          // move to right postion
    buffer |= bit;                        // add into buffer
    pos++;
    if (pos == BYTE) {   // if buffer is full
      fputc(buffer, fp); // write buffer to file
      buffer = 0;        // reset
      pos = 0;
    }
  }
  // if the bit to write is NYT symbol, don't count toward maxSymbol
  if (!nyt) {
    count--;
  }

  // if all symbol is finished and there's something left in buffer
  if (count == 0) {
    fputc(buffer, fp);
  }
}

// check for file error
void fileError(FILE *fp, string filename) {
  if (fp == NULL) {
    cout << "Cannot open file: " << filename << endl;
    exit(1); // exit the code
  }
}
