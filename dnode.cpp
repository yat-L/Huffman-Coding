#include "dnode.h"
#include <cstdint>
#include <vector>

void swap(Node *node1, Node *node2) {
  Node *node1Parent = node1->parent;
  Node *node2Parent = node2->parent;

  if (node1Parent->left == node1) {
    node1Parent->left = node2;
  } else {
    node1Parent->right = node2;
  }
  node2->parent = node1Parent;

  if (node2Parent->left == node2) {
    node2Parent->left = node1;
  } else {
    node2Parent->right = node1;
  }
  node1->parent = node2Parent;
}

vector<Node*> expandNYT(Node *nyt, uint8_t symbol, vector<Node *> &symbolList,
                vector<Node *> &numberList) {
  Node *newSymbol = new Node(0, symbol, (nyt->number) - 1, NULL, NULL, nyt);
  Node *newNYT = new Node(0, 0, (nyt->number) - 2, NULL, NULL, nyt);
  nyt->left = newNYT;
  nyt->right = newSymbol;
  symbolList[symbol] = newSymbol;
  numberList[newSymbol->number] = newSymbol;
  numberList[newNYT->number] = newNYT;

  vector<Node*> result = {newNYT,newSymbol};
  return result;
}
