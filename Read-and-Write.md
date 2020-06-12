# Read and Write operations

This part of doc will include 
- API for write and read
- Overview of data structure involved, i.e. batch, log, memtable, sstable
- The process of write and read
- Snapshot mechanism

Most of the functions return a `Status` object defined in `include/status.h`.

A single key or data value, is of datatype `Slice` declared in `include/leveldb/slice.h`.

## Write

### Create batch

A write operation (a `Put()` or a `Delete()`) first write data to log file and then write to in-memory table. To make use of Atomical operations, user can create `WriteBatch` objects to do batch writing.

Even we insert single key-values by calling `DB::Put()` or `DB::Delete()`, each `Put/Delete` operation from `DB()` is creating a batch and then perform batch writing by `Writebatch::Put()` as follows:

```C++
// db_impl.cc
Status DB::Put(const WriteOptions& opt, const Slice& key, const Slice& value) {
  WriteBatch batch;
  batch.Put(key, value);
  return Write(opt, &batch);
}
```

where `batch.Put(key, value)` is implemented in [`write_batch.cc`](https://github.com/google/leveldb/blob/master/db/write_batch.cc#L98) as:
```C++
// write_batch.cc
void WriteBatch::Put(const Slice& key, const Slice& value) {
  WriteBatchInternal::SetCount(this, WriteBatchInternal::Count(this) + 1);
  rep_.push_back(static_cast<char>(kTypeValue));
  PutLengthPrefixedSlice(&rep_, key);
  PutLengthPrefixedSlice(&rep_, value);
}
```

Where the main data structure of a `Writebatch` object, which contains **a list a data entries**, is a C++ string named `rep_`, with predefined format. The format of `rep_` string is defined in the header of `write_batch.cc`:

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

where `kTypeValue` and `kTypeDeletion` are defined as 
```
enum ValueType { kTypeDeletion = 0x0, kTypeValue = 0x1 };
```

i.e., a write batch contains a header and a list of records, where each record contains the following fields: 


- Type (`kTypeValue` for add/update and `kTypeDeletion` for delete) 
- Key length
- Key
- Value length (when `kTypeValue`)
- Value (when `kTypeValue`)

As the implementation of `batch.Put(key, value)` shown above, the method uses 3 statement to add a key-value to a batch:

```C++
//write_batch.cc
rep_.push_back(static_cast<char>(kTypeValue));
PutLengthPrefixedSlice(&rep_, key);
PutLengthPrefixedSlice(&rep_, value);
```

Firstly using a `std::string::push_back` function to append one character `static_cast<char>(kTypeValue)` to the original `rep_` string, indicating the type of this operation, i.e. `kTypeValue` for adding key-value and `kTypeDeletion` for deletion.

Next call `PutLengthPrefixedSlice(&rep_, key)` to append two fields `key size | key` to the `rep_` string. Then call this function again and append `value size | value` to `rep_` string, completing a data insertion to a batch.

The function `PutLengthPrefixedSlice(&rep_, key)` is implemented in [`coding.cc`](https://github.com/google/leveldb/blob/master/util/coding.cc#L72)

```C++
// coding.cc
// ...
void PutVarint32(std::string* dst, uint32_t v) {
  char buf[5];
  char* ptr = EncodeVarint32(buf, v);
  dst->append(buf, ptr - buf);
}
// ...
void PutLengthPrefixedSlice(std::string* dst, const Slice& value) {
  PutVarint32(dst, value.size());
  dst->append(value.data(), value.size());
}
// ...
```

A deletion in a batch procedure (`Deletion()`) is very similar to insertion (`Put()`), except that we do not append a value to `sep_` but only the key after a `kTypeDeletion` indicator. The calling stack / chain is like:

```
DB::Delete(options, key) -> WriteBatch::Delete(const Slice& key)
```

### Write to MemTable

After creating the batch object, data will be write to the MemTable by calling `DBImpl::Write()`.

The keys are converted into `InternelKey` objects (from `Slice` objects in `rep_`) before they are inserted into the MemTable. An `InternalKey` contains a `Sequence number` (of the operation) and a `ValueType` (that indicate the type of the operation, i.e. put or delete).

While writing (`DBImpl::Write()`): 
1. Get MutexLock
2. If there are other write operations, may wait for merge
3. Add to log and apply to memtable
4. Release the lock

### Call chain

As inserting a data entry into the database (in [demo.cpp](demo/demo.cpp#L39)), we step in the `Put()` function and trace the calling:

Fig. [calling chain.](figs/put_entry.pdf)


## Read

Read data using the `DBImpl::Get(const ReadOptions& options, const Slice& key, std::string* value)` interface.

A snapshot need to be determined when calling the Get() method. If `snapshot` is not set in `options` (`options.snapshot == nullptr`), the last sequence number will be used as snapshot.

Firstly the required key and snapshot (sequence number) are combined (i.e. sequence number and kValueType are packed, see `dbformat.cc` ) to form a `LookupKey` object `lkey`. 

the searches are according to `lkey` and data entries that with a sequence number newer than required sequence number will be skipped.


```C++
// ...
  MemTable* mem = mem_;
  MemTable* imm = imm_;
  Version* current = versions_->current();
  mem->Ref();
  if (imm != nullptr) imm->Ref();
  current->Ref();
// ...
  {
    mutex_.Unlock();
    // First look in the memtable, then in the immutable memtable (if any).
    LookupKey lkey(key, snapshot);
    if (mem->Get(lkey, value, &s)) {
      // Done
    } else if (imm != nullptr && imm->Get(lkey, value, &s)) {
      // Done
    } else {
      s = current->Get(options, lkey, value, &stats);
      have_stat_update = true;
    }
    mutex_.Lock();
  }
// ...
```

1. Search in MemTable,
2. Search in Immutable MemTable,
3. Search in SSTables from Level 0 to Level n by `Version::Get(const ReadOptions& options, const LookupKey& k, std::string* value, GetStats* stats)`.
4. Save the value into `value`. If not found, return `NotFound.