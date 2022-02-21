#include "node.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <sys/types.h>
#include <vector>
#define SIZE 256
#define BYTE_SIZE 8

void Node::traverse(uint8_t progress, uint8_t *table) {
  if (left == NULL && right == NULL) {
    table[symbol] = progress;
  }

  uint8_t goLeft = (progress << 1) | 1;
  uint8_t goRight = (progress << 1) | 0;

  if (left != NULL) {
    left->traverse(goLeft, table);
  }
  if (right != NULL) {
    right->traverse(goRight, table);
  }
}

std::vector<uint8_t> traverse_de(Node *root, uint8_t byte, int numberofSymbol) {
  static Node *currentPtr = root;
  static int remainingSymbol = numberofSymbol;
  std::vector<uint8_t> result;
  int count = BYTE_SIZE;
  while (count > 0 && remainingSymbol > 0) {
    uint8_t lsb = byte & 1;
    byte = byte >> 1;
    if (lsb == 0) {
      currentPtr = currentPtr->right;
    } else {
      currentPtr = currentPtr->left;
    }
    if (currentPtr->left == NULL && currentPtr->right == NULL) {
      result.push_back(currentPtr->symbol);
      //cout << unsigned(currentPtr->symbol) << endl;
      remainingSymbol--;
      currentPtr = root;
    }
    count--;
  }
  return result;
}

Node *merge(Node *leftChild, Node *rightChild) {
  Node *result =
      new Node(leftChild->freq + rightChild->freq, 0, leftChild, rightChild);
  return result;
}
