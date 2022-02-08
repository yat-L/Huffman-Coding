#include "node.h"

void Node::traverse(int *table, uint8_t progress) {
  if (left == NULL && right == NULL) {
    table[symbol] = progress;
  }

  uint8_t goLeft = (progress << 1) | 1;
  uint8_t goRight = progress << 1;

  if (left != NULL) {
    left->traverse(table, goLeft);
  }
  if (right != NULL) {
    right->traverse(table, goRight);
  }
}

int *getCodeTable(Node *root) {
  int *codewordTable = new int[4];
  root->traverse(codewordTable, 0);

  return codewordTable;
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

  int *result = getCodeTable(ABCD);
  for(int i = 0 ; i < 4 ; i++){
    cout << std::bitset<8>(result[i]) << endl;
  }
}
