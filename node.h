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

  void traverse(uint8_t progress, uint8_t* table);

};

  class compareNode {
  public:
    bool operator()(const Node *n1, const Node *n2) {

      return n1->freq > n2->freq;
    }
};

Node *merge(Node *leftChild, Node *rightChild);


std::vector<uint8_t> traverse_de(Node *root, uint8_t byte, int numberofSymbol);
