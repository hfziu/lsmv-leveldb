# Read and Write operations

This part of doc will include 
- API for write and read
- Overview of data structure involved, i.e. batch, log, memtable, sstable
- The process of write and read
- Snapshot mechanism

Most of the functions return a `Status` object defined in `include/status.h`.

A single key or data value, is of datatype `Slice` declared in `include/leveldb/slice.h`.

## Write

A write operation (a `Put()` or a `Delete()`) first write data to log file and then write to in-memory table. To make use of Atomical operations, user can create `WriteBatch` objects to do batch writing.

Even we insert single key-values by calling `DB::Put()` or `DB::Delete()`, each `Put/Delete` operation from `DB()` is creating a batch and then perform batch writing by `Writebatch::Put()`.

A `Writebatch` object is a list of data entries, it contains a member named `rep_` which is defined as follows:

```C++
// write_batch.cc

// WriteBatch::rep_ :=
//    sequence: fixed64
//    count: fixed32
//    data: record[count]
// record :=
//    kTypeValue varstring varstring         |
//    kTypeDeletion varstring
// varstring :=
//    len: varint32
//    data: uint8[len]
```

where `rep_` is a C++ string with fixed format as described above, and `kTypeValue` and `kTypeDeletion` are defined as 
```
enum ValueType { kTypeDeletion = 0x0, kTypeValue = 0x1 };
```

i.e., a write batch contains a header and a list of records, where each record contains the following fields: 


- Type (`kTypeValue` for add/update and `kTypeDeletion` for delete) 
- Key length
- Key
- Value length (when `kTypeValue`)
- Value (when `kTypeValue`)

After creating the batch object, data will be write to the MemTable by calling `DBImpl::Write()`.

The keys are converted into `InternelKey` objects (from `Slice` objects in `rep_`) before they are inserted into the MemTable. An `InternalKey` contains a `Sequence number` (of the operation) and a `ValueType` (that indicate the type of the operation, i.e. put or delete).

While writing (`DBImpl::Write()`): 
1. Get MutexLock
2. If there are other write operations, may wait for merge
3. Add to log and apply to memtable
4. Release the lock


## Read