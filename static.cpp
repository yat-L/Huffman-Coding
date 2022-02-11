#include "node.h"
#define SIZE 256


int main() {
  cout << "Static main" << endl;
  /////////////////////
  priority_queue<Node *, vector<Node *>, compareNode> Q;
  //////////////////////

  uint8_t *codewordTable = new uint8_t[SIZE];
  unsigned char *x = std::fill_n(codewordTable, SIZE, 1);


  Node *testA = new Node(6, 0, NULL, NULL);
  Node *testB = new Node(2, 1, NULL, NULL);
  Node *testC = new Node(2, 2, NULL, NULL);
  Node *testD = new Node(1, 3, NULL, NULL);
  Node *CD = merge(testD, testC);
  Node *BCD = merge(testB, CD);
  Node *ABCD = merge(BCD, testA);

  ABCD->traverse(0,codewordTable);
  for (int i = 0; i < 255; i++) {
    //  cout << std::bitset<8>(codewordTable[i]) << endl;
  }

  uint8_t testByte1 = 254;
  uint8_t testByte2 = 5;
  traverse_de(ABCD, testByte1, 7);
  traverse_de(ABCD, testByte2, 7);
}
