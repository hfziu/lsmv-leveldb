#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <string>

#include <cstdlib>
#include <ctime>

#include <cassert>
#include "leveldb/db.h"
#include "leveldb/comparator.h"
#include "leveldb/write_batch.h"

using namespace std;

int main(int argc, char **argv) {
  auto time_ui = static_cast<unsigned int>( time(nullptr));
  srand(time_ui);

  leveldb::DB *db;
  leveldb::Options options;
  options.create_if_missing = true;
  options.error_if_exists = true;
  options.comparator = leveldb::IntegerComparator();
  leveldb::Status status = leveldb::DB::Open(options, "test_db", &db);
  assert(status.ok());

  if (!status.ok())
    cerr << status.ToString() << endl;

  // Put
  leveldb::WriteOptions writeOptions;
  for (unsigned int i = 0; i < 65535; ++i) {
    ostringstream keyStream;
    keyStream << i;

    ostringstream valueStream;
    valueStream << rand();

    db->Put(writeOptions, keyStream.str(), valueStream.str());
  }

  // Iterator
  leveldb::Iterator *it = db->NewIterator(leveldb::ReadOptions());

  for (it->SeekToFirst(); it->Valid(); it->Next()) {
    cout << it->key().ToString() << " : " << it->value().ToString() << endl;
  }

  // Get
  string value;
  string key1 = "5";

  leveldb::Status s = db->Get(leveldb::ReadOptions(), key1, &value);

  if (s.ok()) {
    cout << "Required Key: " << key1 << ", ";
    cout << "Retrieved value: " << value << endl;
  }

  delete it;

  delete db;
}
