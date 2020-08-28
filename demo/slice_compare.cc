//
// Created by xiaofzhao on 28/8/2020.
//

#include <string>
#include <sstream>
#include <iostream>

#include "leveldb/slice.h"
//#include "leveldb/db.h"
//#include "leveldb/comparator.h"

using namespace std;
using namespace leveldb;

const char* compare(Slice& key1, Slice& key2) {
  return (key1.numeric_compare(key2)>0)?" > ":" < ";
}

int main() {
  Slice key1("10");
  Slice key2("100");
  Slice key3("99");


  cout << "key1" << compare(key1, key2) << "key2" << endl;
  cout << "key3" << compare(key3, key2) << "key2" << endl;
  cout << "key3" << compare(key3, key1) << "key1" << endl;

  cout << "sizes:" << key1.size() << ", " << key2.size() << ", " << key3.size();

  return 0;
}