#include "node.h"
#include <algorithm>
#include <cstdint>
#include <cstdio>
#include <sys/types.h>
#include <vector>
#define SIZE 256
#define BYTE_SIZE 8

//
// traverse the codeword tree to obtain each codeword and put it into table for
// encoding
void Node::traverse(unsigned long progress, int length,
                    vector<vector<unsigned long>> &table) {
  if (left == NULL && right == NULL) { // if leaf node reached
    table[symbol][0] = progress;
    table[symbol][1] = length;
  }

  unsigned long goLeft = (progress << 1) | 1; // update progress
  unsigned long goRight = (progress << 1) | 0;

  if (left != NULL) {
    left->traverse(goLeft, length + 1, table); // go to left side
  }
  if (right != NULL) {
    right->traverse(goRight, length + 1, table); // go to right side
  }
}

//
// traverse the codeword tree according to one byte, return a vector of symbol
// decoded
std::vector<uint8_t> traverse_de(Node *root, uint8_t byte, int numberofSymbol) {
  static Node *currentPtr = root; // current node that is pointing to
  static int remainingSymbol =
      numberofSymbol; // remaining symbol need to the decoded
  std::vector<uint8_t> result;
  int count = BYTE_SIZE;
  while (count > 0 &&
         remainingSymbol >
             0) { // while data still left in byte or all symbol decoded
    uint8_t lsb = byte & 1; // take one bit
    byte = byte >> 1;
    if (lsb == 0) {
      currentPtr = currentPtr->right; // if bit is 0 go right
    } else {
      currentPtr = currentPtr->left; // if bit is 1 go left
    }
    if (currentPtr->left == NULL &&
        currentPtr->right ==
            NULL) { // when leaf node reached, put the symbol in vector
      result.push_back(currentPtr->symbol);
      remainingSymbol--;
      currentPtr = root;
    }
    count--;
  }
  return result; // return list of symbol decoded
}

// merge two tree, create new root node for 2 subtree, use in building codeword
// tree
Node *merge(Node *leftChild, Node *rightChild) {
  Node *result =
      new Node(leftChild->freq + rightChild->freq, 0, leftChild, rightChild);
  return result;
}
