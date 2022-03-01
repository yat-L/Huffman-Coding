#include "dnode.h"
#include <cstdint>
#include <vector>

// swap the subtree of node1 and node2
void swap(Node *node1, Node *node2) {
  Node *node1Parent = node1->parent;
  Node *node2Parent = node2->parent;

  //if node1 is left child
  if (node1Parent->left == node1) {
    // set node2 to left child
    node1Parent->left = node2;
  } else {
    // set node2 to right child
    node1Parent->right = node2;
  }
  // set parent of node2
  node2->parent = node1Parent;

  // node2 is left child
  if (node2Parent->left == node2) {
    //set node1 to left child
    node2Parent->left = node1;
  } else {
    //set node1 to right child
    node2Parent->right = node1;
  }
  // set parent of node1
  node1->parent = node2Parent;

  //swapping number when subtree swapping
  int temp = node1->number;
  node1->number = node2->number;
  node2->number = temp;
}

vector<Node*> expandNYT(Node *nyt, uint8_t symbol, vector<Node *> &symbolList,
                vector<Node *> &numberList) {
  // new node for storing new symbol
  Node *newSymbol = new Node(0, symbol, (nyt->number) - 1, NULL, NULL, nyt);
  // new node for storing the next NYT symbol
  Node *newNYT = new Node(0, 0, (nyt->number) - 2, NULL, NULL, nyt);
  nyt->left = newNYT; // setup the tree
  nyt->right = newSymbol;
  symbolList[symbol] = newSymbol; // update symbollist
  numberList[newSymbol->number] = newSymbol; // update numberlist
  numberList[newNYT->number] = newNYT;

  vector<Node*> result = {newNYT,newSymbol}; // return in vector form
  return result;
}
