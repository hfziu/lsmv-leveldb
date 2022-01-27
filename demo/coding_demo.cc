#include <iostream>
#include <string>
#include "util/coding.h"

using namespace leveldb;
using namespace std;

int main () {
  Slice a;
  string rep_;
  uint64_t num_a = 1609653794653;
//  PutFixed64(&rep_, num_a);
  cout << rep_ << endl;
  return 0;
}