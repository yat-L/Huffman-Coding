#include "dnode.h"
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <string>
#include <vector>
#define SIZE 256

void fileError(FILE *fp, string filename);
void writeFile(vector<unsigned long>, bool, FILE *);
void compress(string inputFile, string outputFile);
vector<unsigned long> leafToRoot(Node *leaf);

int main() { compress("long", "com"); }

void compress(string inputFile, string outputFile) {
  vector<Node *> symbolList(SIZE, NULL);
  vector<Node *> numberList(2 * SIZE + 1 + 1, NULL);
  FILE *fpI = fopen(inputFile.c_str(), "rb");
  FILE *fpO = fopen(outputFile.c_str(), "wb");

  uint8_t currentByte = 0;
  // read byte from input until EOF
  while (fread(&currentByte, sizeof(uint8_t), 1, fpI) == 1) {
    if (symbolList[currentByte] == NULL) {
      // add to tree
    } else {
      vector<unsigned long> temp = leafToRoot(symbolList[currentByte]);
      writeFile(temp,false,fpO);
    }
  }
  vector<unsigned long> empty(0);  // writing the last data from buffer
  writeFile(empty, true, fpO);
}

vector<unsigned long> leafToRoot(Node *leaf) {
  unsigned long codeword = 0;
  unsigned long pos = 0;
  Node* current = leaf;
  while (current != NULL) {
    Node* parent = current->parent;
    if(parent->left == current){  // if is left child
      codeword = codeword | (0 << pos);
    }
    else{ // if is right child
      codeword = codeword | (1 << pos);
    }
    pos++;
    current = current->parent;
  }

  vector<unsigned long> result = {codeword,pos};
  return result;
}

// write codeword into file byte by byte
void writeFile(vector<unsigned long> codeword, bool finish, FILE *fp) {
  static uint8_t buffer = 0;    // buffer, 8 bit
  static int pos = 0;           // current position of buffer


  // if all symbol is finished and there's something left in buffer
  if (finish) {
    fputc(buffer, fp);
  }
  else{
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
