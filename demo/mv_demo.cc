#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <string>

#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

#include <cassert>
#include "leveldb/db.h"
#include "leveldb/slice.h"
#include "leveldb/comparator.h"
#include "leveldb/write_batch.h"

#ifndef DEBUG
#define DEBUG
#endif

using namespace std;

int main(int argc, char **argv)
{
  auto time_ui = static_cast<unsigned int>(time(nullptr));
  srand(time_ui);

  leveldb::DB *db;
  leveldb::Options options;
  options.create_if_missing = true;
  options.error_if_exists = true;
  // options.comparator = leveldb::BytewiseComparator();
  leveldb::Status status = leveldb::DB::Open(options, "test_db", &db);
  assert(status.ok());

  if (!status.ok())
    cerr << status.ToString() << endl;

  // Put
  leveldb::WriteOptions writeOptions;
  for (unsigned long long i = 0; i < 10; ++i)
  {
    ostringstream keyStream;
    keyStream << i;

    ostringstream valueStream;
    valueStream << i * 11;

    ostringstream valueStreamUpdate;
    valueStreamUpdate << i * 111;

    // We are going to Put data with current timestamp.
    db->PutMV(writeOptions, keyStream.str(), valueStream.str());
    // Sleep for 1 second
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    db->PutMV(writeOptions, keyStream.str(), valueStreamUpdate.str());
  }

  // Iterator
  leveldb::Iterator *it = db->NewIterator(leveldb::ReadOptions());

  for (it->SeekToFirst(); it->Valid(); it->Next())
  {
    cout << it->key().ToString() << " : " << it->value().ToString() << endl;
  }

  // Get
  leveldb::ValidTime current_time = duration_cast<milliseconds>
       (system_clock::now().time_since_epoch()).count();
  std::cout << "Current Time: " << current_time << endl;
  string value;
  string key = "5";
  leveldb::ValidTime t = current_time;
  leveldb::ValidTime lo;
  leveldb::ValidTime hi;

  leveldb::Status s = db->GetMV(leveldb::ReadOptions(), key, t, &lo, &hi, &value);

  if (s.ok())
  {
    cout << "Required Key: " << key << ", ";
    cout << "Valid Time (From To): " << lo << ", " << hi << endl;
    cout << "Retrieved value: " << value << endl;
  }

  // Get the versions 4500 milliseconds earlier (The previous version of key)
  leveldb::Status s2 = db->GetMV(leveldb::ReadOptions(), key, t-4500, &lo, &hi, &value);

  if (s2.ok())
  {
    cout << "Required Key: " << key << ", ";
    cout << "Valid Time (From To): " << lo << ", " << hi << endl;
    cout << "Retrieved value: " << value << endl;
  }

  // Get Range
  string k1 = "5";
  string k2 = "8";
  leveldb::ValidTime t1 = 0;
  leveldb::ValidTime t2 = current_time;

  leveldb::Status s3 = db->GetMVRange(leveldb::ReadOptions(), k1, k2, t1, t2);

  delete it;

  delete db;
}
