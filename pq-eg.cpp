/*A simple demo of using a c++ priority queue with a class instances as nodes*/

#include <iostream>
#include <queue>

using namespace std;
 
#define ROW 9
#define COL 2
 
class SymbolInfo {
 
public:
  uint8_t symbol;
  int isSymbol;
  int frequency;
 
  // this is used to initialize the variables of the class
  SymbolInfo(char symbol, int isSymbol, int frequency)
    : symbol(symbol), isSymbol(isSymbol), frequency(frequency)
  {
  }
};
 
// we are doing operator overloading through this
bool operator<(const SymbolInfo & s1, const SymbolInfo& s2)
{
 
  // this will return true when second symbol
  // has  frequency less than that of the first symbol given by s1. 
    
  return s1.frequency >  s2.frequency;
}
 
int main()
{
 
  priority_queue<SymbolInfo> Q;
 

  int arr[ROW][COL] = {  { 'b', 2 },
                         { 'c', 4 }, { 'd',  3 }, { 'e', 6 } , {'g',3},{ 'a',4},{'x',2},{'y',2},{'z',2} };
 
  //insert the 6 nodes above into the queue

  for (int i = 0; i < ROW; ++i) {
 
    Q.push(SymbolInfo(arr[i][0], 1, arr[i][1]));
 
    // insert an object in priority_queue by using
    // the SymbolInfo class constructor
  }
 
  //remove each node from the queue
  while (!Q.empty()) {
 
    SymbolInfo s = Q.top(); //get item at top of queue
 
    Q.pop(); //remove item at top of queue from the queue
 
    cout << s.symbol << " " << s.frequency << "\n";
  }
  return 0;
}
