#include <cstddef>
#include <cstdint>
#include <vector>

using namespace std;

class Node { // node class
public:
  int freq; // freqeunce of the symbol in file
  int number;
  uint8_t symbol; // symbol in 8 bits
  Node *left;
  Node *right;
  Node *parent;
  Node(int freqI, uint8_t symbolI, int number, Node *leftI, Node *rightI,
       Node *parentI) {
    freq = freqI;
    symbol = symbolI;
    left = leftI;
    right = rightI;
    parent = parentI;
  }
  ~Node() {
    // cout << "Deleting " << freq << endl;
    delete left;
    delete right;
  }
};

Node *expandNYT(Node *nyt);
void swap(Node *, Node *);
