#include "node.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <sys/types.h>
#define SIZE 256
#define BYTE_SIZE 8

uint8_t *codewordTable = new uint8_t[SIZE];
unsigned char *x = std::fill_n(codewordTable, SIZE, 1);
int numberofSymbol = 7;

void Node::traverse(uint8_t progress) {
  if (left == NULL && right == NULL) {
    codewordTable[symbol] = progress;
  }

  uint8_t goLeft = (progress << 1) | 1;
  uint8_t goRight = (progress << 1) | 0;

  if (left != NULL) {
    left->traverse(goLeft);
  }
  if (right != NULL) {
    right->traverse(goRight);
  }
}

uint8_t traverse_de(Node *root, uint8_t byte) {
  static Node *currentPtr = root;
  int count = BYTE_SIZE;
  while (count >0 && numberofSymbol > 0 ) {
    uint8_t lsb = byte & 1;
    byte = byte >> 1;
    if (lsb == 0) {
      currentPtr = currentPtr->right;
    } else {
      currentPtr = currentPtr->left;
    }
    if (currentPtr->left == NULL && currentPtr->right == NULL) {
      cout << unsigned(currentPtr->symbol) << endl;
      numberofSymbol --;
      currentPtr = root;
    }
    count --;
  }
  return 1;
}

Node *merge(Node *leftChild, Node *rightChild) {
  Node *result =
      new Node(leftChild->freq + rightChild->freq, 0, leftChild, rightChild);
  return result;
}

int main() {
  /////////////////////
  priority_queue<Node *, vector<Node *>, compareNode> Q;
  //////////////////////
  Node *testA = new Node(6, 0, NULL, NULL);
  Node *testB = new Node(2, 1, NULL, NULL);
  Node *testC = new Node(2, 2, NULL, NULL);
  Node *testD = new Node(1, 3, NULL, NULL);
  Node *CD = merge(testD, testC);
  Node *BCD = merge(testB, CD);
  Node *ABCD = merge(BCD, testA);

  ABCD->traverse(0);
  for (int i = 0; i < 255; i++) {
    cout << std::bitset<8>(codewordTable[i]) << endl;
  }
  uint8_t testByte1 = 254;
  uint8_t testByte2 = 5;
  traverse_de(ABCD, testByte1);
  traverse_de(ABCD, testByte2);
}
