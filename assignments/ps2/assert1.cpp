#define NDEBUG

#include <iostream>
#include <cassert>
using namespace std; 

int main() {
  assert(1 == 0 && "This is never true"); // will be triggered 
  cout << "Hello World" << endl; 
  return 0; 
}
