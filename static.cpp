#include "node.h"
#include <assert.h>
#include <cstdint>
#include <cstdio>
#include <vector>
#define SIZE 256

Node *buildingTree(int *);
void init(int *);
void writeFile(uint8_t, int, FILE*);
int coutBits(uint8_t);

int main() {
  cout << "Static main" << endl;
  /////////////////////
  // priority_queue<Node *, vector<Node *>, compareNode> priQ;
  uint8_t *codewordTable = new uint8_t[SIZE];
  unsigned char *x = std::fill_n(codewordTable, SIZE, 1);
  int *freqTable = new int[SIZE];
  //////////////////////
  init(freqTable);
  Node *root = buildingTree(freqTable);
  root->traverse(0, codewordTable);

  // Node *testA = new Node(6, 0, NULL, NULL);
  // Node *testB = new Node(2, 1, NULL, NULL);
  // Node *testC = new Node(2, 2, NULL, NULL);
  // Node *testD = new Node(1, 3, NULL, NULL);
  // Node *CD = merge(testD, testC);
  // Node *BCD = merge(testB, CD);
  // Node *ABCD = merge(BCD, testA);

  //  ABCD->traverse(0, codewordTable);
  for (int i = 0; i < 255; i++) {
    if (codewordTable[i] != 1) {
      cout << i << ": " << std::bitset<8>(codewordTable[i]) << endl;
      cout << i << ": " << coutBits(codewordTable[i]) << endl;
    }
  }

  FILE *fp = fopen("test.txt.huff", "wb");
  assert(fp);

  writeFile(codewordTable[97],7, fp);
  writeFile(codewordTable[98], 7, fp);
  writeFile(codewordTable[99], 7, fp);
  writeFile(codewordTable[97], 7, fp);
  writeFile(codewordTable[97], 7, fp);
  writeFile(codewordTable[97], 7, fp);
  writeFile(codewordTable[10], 7, fp);

  // uint8_t testByte1 = 254;
  // uint8_t testByte2 = 5;
  // std::vector<uint8_t> test1 = traverse_de(ABCD, testByte1, 7);
  // std::vector<uint8_t> test2 = traverse_de(ABCD, testByte2, 7);
}

void writeFile(uint8_t codeword, int maxSymbol, FILE* fp) {
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
  if(count == 0){
    //cout << "last byte" << endl;
    fputc(buffer, fp);
  }

  //cout << "buffer after each write: " << std::bitset<8>(buffer) << endl;
  //cout << "pos: " << pos << endl;
  //cout << "count: " << count << endl;
}

int coutBits(uint8_t i) {
  int result;
  if(i == 0){
    return 1;
  }
  for (result = 0; i > 0; result++) {
    i >>= 1;
  }
  return result;
}
void init(int *freq) {
  FILE *fp = fopen("test.txt", "rb");
  assert(fp);
  int c = fgetc(fp);
  while (c != EOF) {
    printf("%d \n", c);
    freq[c]++;
    c = fgetc(fp);
  }
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
