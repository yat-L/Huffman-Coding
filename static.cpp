#include "node.h"
#include <assert.h>
#include <cstdint>
#include <cstdio>
#include <vector>
#define SIZE 256


uint8_t *codewordTable = new uint8_t[SIZE];
unsigned char *x = std::fill_n(codewordTable, SIZE, 1);

int *freqTable = new int[SIZE];

int main() {
  cout << "Static main" << endl;
  /////////////////////
  priority_queue<Node *, vector<Node *>, compareNode> priQ;
  //////////////////////

  FILE *fp = fopen("test.txt", "rb");
  assert(fp);
  int c = getc(fp);
  while (c != EOF) {
    printf("%d \n", c);
    freqTable[c]++;

    c = getc(fp);
  }

  for (int i = 0; i < 255; i++) {
    cout << i << ": " << freqTable[i] << endl;
  }

  // Node *testA = new Node(6, 0, NULL, NULL);
  // Node *testB = new Node(2, 1, NULL, NULL);
  // Node *testC = new Node(2, 2, NULL, NULL);
  // Node *testD = new Node(1, 3, NULL, NULL);
  // Node *CD = merge(testD, testC);
  // Node *BCD = merge(testB, CD);
  // Node *ABCD = merge(BCD, testA);

  //  ABCD->traverse(0, codewordTable);
  for (int i = 0; i < 255; i++) {
    // cout << std::bitset<8>(codewordTable[i]) << endl;
  }

  // uint8_t testByte1 = 254;
  // uint8_t testByte2 = 5;
  // std::vector<uint8_t> test1 = traverse_de(ABCD, testByte1, 7);
  // std::vector<uint8_t> test2 = traverse_de(ABCD, testByte2, 7);
}

