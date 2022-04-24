#include <bitset>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

class Node {  // node class
public:
  int freq;  // freqeunce of the symbol in file
  uint8_t symbol;  // symbol in 8 bits
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

  void traverse(unsigned long progress,int length, vector<vector<unsigned long>>&);
};

class compareNode {  // compare class for priority queue 
public:
  bool operator()(const Node *n1, const Node *n2) {

      return n1->freq > n2->freq;
  }
};

Node *merge(Node *leftChild, Node *rightChild);

std::vector<uint8_t> traverse_de(Node *root, uint8_t byte, int numberofSymbol);
