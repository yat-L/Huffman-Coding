#include "dnode.h"
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <math.h>
#include <string>
#include <vector>
#define SIZE 256

void fileError(FILE *fp, string filename);
void writeFile(vector<unsigned long>, bool, FILE *);
void compress(string inputFile, string outputFile);
vector<unsigned long> leafToRoot(Node *leaf);
void update(vector<Node *> &symbolList, vector<Node *> &numberList, Node *Y);
  void checkAndSwap(vector<Node *> & numberList, Node * current);
  vector<unsigned long> fixedCode(uint8_t symbol);

  int main() { compress("long", "com"); }

  void compress(string inputFile, string outputFile) {
    vector<Node *> symbolList(SIZE, NULL);
    vector<Node *> numberList(2 * SIZE + 1 + 1, NULL);
    FILE *fpI = fopen(inputFile.c_str(), "rb");
    FILE *fpO = fopen(outputFile.c_str(), "wb");
    Node *NYT = new Node(0, 0, 2 * SIZE + 1, NULL, NULL, NULL);
    numberList[2 * SIZE + 1] = NYT;
    Node *Y = NULL;
    bool firstTime = true;

    uint8_t currentByte = 0;
    // read byte from input until EOF
    while (fread(&currentByte, sizeof(uint8_t), 1, fpI) == 1) {
      if (symbolList[currentByte] == NULL) {             // new symbol appeared
        if (firstTime) {                                 // first symbol
          writeFile(fixedCode(currentByte), false, fpO); // outpu fixed code
          firstTime = false;
        } else {
          vector<unsigned long> nytCode = leafToRoot(NYT);
          writeFile(nytCode, false, fpO);
          writeFile(fixedCode(currentByte), false, fpO); // outpu fixed code
        }
        vector<Node *> leftandRight =
            expandNYT(NYT, currentByte, symbolList, numberList);
        NYT = leftandRight[0];
        Y = leftandRight[1];
      } else {
        vector<unsigned long> temp = leafToRoot(symbolList[currentByte]);
        writeFile(temp, false, fpO);
      }
      update(symbolList, numberList, Y);
    }
    vector<unsigned long> empty(0); // writing the last data from buffer
    writeFile(empty, true, fpO);
  }

  vector<unsigned long> fixedCode(uint8_t symbol) {
    unsigned long length = log2(symbol) + 1; // length of fixed code, binary
    vector<unsigned long> result = {symbol, length};
    return result;
  }

  void update(vector<Node *> & symbolList, vector<Node *> & numberList,
              Node * Y) {
    Node *current = Y;
    while (current != NULL) {
      if (current->parent == NULL) { // if root node
        current->freq++;
      } else {
        checkAndSwap(numberList, current);
        current->freq++;
      }
      current = current->parent;
    }
  }
  void checkAndSwap(vector<Node *> & numberList, Node * current) {
    for (int i = 2 * SIZE + 1; i >= 1; i--) {
      if (numberList[i] != NULL) {
        bool higherNumber = numberList[i]->number > current->number;
        bool sameWeight = numberList[i]->freq == current->freq;
        bool notParent = current->parent != numberList[i];
        if (higherNumber && sameWeight && notParent) {
          swap(current, numberList[i]);
          return;
        }
      }
    }
    return;
  }

  vector<unsigned long> leafToRoot(Node * leaf) {
    unsigned long codeword = 0;
    unsigned long pos = 0;
    Node *current = leaf;
    while (current != NULL) {
      Node *parent = current->parent;  //BUGGGG What if parent is NULL
      if (parent->left == current) { // if is left child
        codeword = codeword | (0 << pos);
      } else { // if is right child
        codeword = codeword | (1 << pos);
      }
      pos++;
      current = current->parent;
    }

    vector<unsigned long> result = {codeword, pos};
    return result;
  }

  // write codeword into file byte by byte
  void writeFile(vector<unsigned long> codeword, bool finish, FILE *fp) {
    static uint8_t buffer = 0; // buffer, 8 bit
    static int pos = 0;        // current position of buffer

    // if all symbol is finished and there's something left in buffer
    if (finish) {
      fputc(buffer, fp);
    } else {
      for (int i = codeword[1] - 1; i >= 0; i--) {
        uint8_t bit = (codeword[0] >> i) & 1; // take msb bit
        bit <<= pos;                          // move to right postion
        buffer |= bit;                        // add into buffer
        pos++;
        if (pos == 8) {      // if buffer is full
          fputc(buffer, fp); // write buffer to file
          buffer = 0;        // reset
          pos = 0;
        }
      }
    }
  }
