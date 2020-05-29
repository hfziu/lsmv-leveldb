#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>
#include <string>

#include <cassert>
#include "leveldb/db.h"
#include "leveldb/write_batch.h"

using namespace std;

int main()
{
    leveldb::DB *db;
    leveldb::Options options;
    options.create_if_missing = true;
    // options.error_if_exists = true;
    leveldb::Status status = leveldb::DB::Open(options, "test_db", &db);
    assert(status.ok());

    if (!status.ok())
        cerr << status.ToString() << endl;

    leveldb::WriteOptions writeOptions;
    for (unsigned int i = 0; i < 100; ++i)
    {
        ostringstream keyStream;
        keyStream << setfill('0') << setw(5) << i;

        ostringstream valueStream;
        valueStream << "value: " << i;

        db->Put(writeOptions, keyStream.str(), valueStream.str());
    }

    // Iterator
    leveldb::Iterator *it = db->NewIterator(leveldb::ReadOptions());

    for (it->SeekToFirst(); it->Valid(); it->Next())
    {
        cout << it->key().ToString() << " : " << it->value().ToString() << endl;
    }

    delete it;

    delete db;
}
