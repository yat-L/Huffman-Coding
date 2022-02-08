#include <bitset>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

class Node {
public:
  int freq;
  uint8_t symbol;
  Node *left;
  Node *right;
  Node(int freqI, uint8_t symbolI, Node *leftI, Node *rightI) {
    freq = freqI;
    symbol = symbolI;
    left = leftI;
    right = rightI;
  }
  ~Node() {
    // cout << "Deleting " << freq << endl;
    delete left;
    delete right;
  }
  void traverse(int* table, uint8_t progress);
};

class compareNode {
public:
  bool operator()(const Node *n1, const Node *n2) {

    return n1->freq > n2->freq;
  }
};

int *getCodeTable(Node* root);

Node *merge(Node *leftChild, Node *rightChild);
