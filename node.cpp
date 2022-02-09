#include "node.h"
#include <algorithm>
#include <cstdint>
#define SIZE 256

uint8_t *codewordTable = new uint8_t[SIZE];
unsigned char* x = std::fill_n(codewordTable, SIZE, 1);

void Node::traverse( uint8_t progress) {
  if (left == NULL && right == NULL) {
    codewordTable[symbol] = progress;
  }

  uint8_t goLeft = (progress << 1) | 1;
  uint8_t goRight = progress << 1;

  if (left != NULL) {
    left->traverse( goLeft);
  }
  if (right != NULL) {
    right->traverse( goRight);
  }
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
  for(int i = 0 ; i < 255 ; i++){
    cout << std::bitset<8>(codewordTable[i]) << endl;
  }
}
