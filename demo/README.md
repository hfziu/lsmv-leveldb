# Demo: start from here

## Build LevelDB Source Code

### Getting the Source

```
git clone --recurse-submodules https://github.com/google/leveldb.git
```

### Building

```
mkdir -p build && cd build
cmake -DCMAKE_BUILD_TYPE=Release .. && cmake --build .
```

## Create a Demo

Assuming we have built LevelDB in `${LEVELDB}/build`. 

Create a demo [`demo.cpp`](demo.cpp) as follows:

```C++
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

    // ... do something with the database
    // e.g. Put some key-values
    // Iterate over the data and print the data entries

    delete db; // delete database
    return 0;
}
```

We compile with the following commands (e.g. using `clang++`):

```bash
# export LEVELDB=<path to leveldb>
$ clang++ -std=c++11 -stdlib=libc++ -I${LEVELDB}/include -g demo.cpp ${LEVELDB}/build/libleveldb.a -lsnappy -lpthread -o demo.o
```

By running `demo.o` we created a test database (named `test_db` in the example above) lies in a directory named `test_db`.

A typical file structure of the database looks like this:

```
$ tree test_db
test_db
├── 000015.log
├── 000016.ldb
├── CURRENT
├── LOCK
├── LOG
├── LOG.old
└── MANIFEST-000013
```

where the contents of these files are explained [here](https://github.com/google/leveldb/blob/master/doc/impl.md).