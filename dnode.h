#include <cstddef>
#include <cstdint>
#include <vector>

using namespace std;

class Node { // node class
public:
  int freq; // freqeunce of the symbol in file
  int number; // number of node
  uint8_t symbol; // symbol in 8 bits
  Node *left;  // left child
  Node *right; // right child
  Node *parent;  // parent node
  Node(int freqI, uint8_t symbolI, int numberI, Node *leftI, Node *rightI,
       Node *parentI) {
    freq = freqI;
    number = numberI;
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

vector<Node *> expandNYT(Node *nyt, uint8_t symbol, vector<Node *> &symbolList,
                         vector<Node *> &numberList);
void swap(Node *, Node *);
