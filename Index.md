# 1. LevelDB

## 1.1. Overview of Main Interface of LevelDB

LevelDB is a key-value storage library that provides an ordered mapping from string keys to string values.

The main interface of LevelDB is implemented in class `DB` ([source](https://github.com/google/leveldb/blob/master/db/db_impl.cc)/[header](https://github.com/google/leveldb/blob/master/include/leveldb/db.h); [doc](https://github.com/google/leveldb/blob/master/doc/index.md)) as follows:

```C++
virtual Status Put (const WriteOptions &options, const Slice &key, const Slice &value);
virtual Status Delete (const WriteOptions &options, const Slice &key);
virtual Status Write (const WriteOptions &options, WriteBatch *updates);
virtual Get (const ReadOptions &options, const Slice &key, std::string *value);
virtual Iterator * NewIterator (const ReadOptions &options);
virtual const Snapshot * GetSnapshot ();
virtual void ReleaseSnapshot (const Snapshot *snapshot);
virtual bool GetProperty (const Slice &property, std::string *value);
virtual void GetApproximateSizes (const Range *range, int n, uint64_t *sizes);
virtual void CompactRange (const Slice *begin, const Slice *end);
```

i.e., main interfaces including

1. Put,
2. Delete,
3. Write,
4. Get

and other utilities like `Iterator` and `Snapshot`, where `Put`, `Delete` and `Write` are actually implemented consistently as `Write`. `Write` serves batch writes to the database, while `Put` serves single insertion by creating a small batch that contains only one data entry (key-value) and then calls `Write` to write the small batch. `Delete` is a similar operation of `Put` but without an actual value to insert, only the key and a delete mark (tomb stone that indicates that the key is deleted) while be inserted.

## 1.2. The Concept of LSM-tree

**(Described in the LSMV-tree write up)**

LevelDB is an implementation of the idea Log-Structured Merge tree (LSM-tree). LSM-tree was proposed by [Patrick O'Neil et al.](https://dl.acm.org/doi/10.1007/s002360050048), with the purpose to reduce disk arm movements due to random write operations. The LSM-tree is a typical example of immutable file structure using append-only and merging operations to maintain the data and index. Each LSM-tree consists of two or more storage components. The first component (known as `C0`) is resided in the main memory, while the other components (`C1 ... Cn`) are maintained on disk storage with increasing sizes and ages. They are also called levels 0, 1, ..., and n, respectively. Component `C0` contains the data written in the latest time period while the other components contain data created in earlier time periods. To facilitate the search of a key-value pair, each component may have a B-tree orB+-tree of key-address pairs as an index.

## 1.3. Overview of Main Components and Utilities of LevelDB

### 1.3.1. Main Components

To implement the idea of LSM-tree, LevelDB has the following main components:

| Components | Source Files                          |
| ---------- | ------------------------------------- |
| WAL Log    | `db/log_*.{h|cc}`                     |
| MemTable   | `db/memtable.{h|cc}`, `db/skiplist.h` |
| SSTable    | `table/*.{h|cc}`                      |

where Write-Ahead Logging (WAL) is a technique to record the changes before they are actually written to the database, [providing atomicity and durability](https://en.wikipedia.org/wiki/Write-ahead_logging). When insert batches into a LevelDB database, the library first writes the change to an on-disk log, then apply the changes to the database (This is implemented in `DB:Write()`, and described in [Read-and-Write](Read-and-Write.md)). When the database host system fails unexpectedly during some operations, e.g., loses power or crashed, the database can check the write-ahead log on restart and compare it with the actual status of the database, redo the changes on the database that have not successfully applied when the system failed.

`MemTable` is the implementation of the in-memory table (i.e., $C_0$) of LSM-tree. The main class member of `MemTable` that used to store data is implemented as a [skip list](https://dl.acm.org/doi/10.1145/78973.78977). skip list is a probabilistic data structure the allows $O(\log n)$ search complexity as well as $O(\log n)$ insertion complexity.

`SSTable` is the implementation of the on-disk components (i.e., $C_1 \ldots C_n$) of LSM-tree. The on-disk tables are stored as files named `[0-9]+.ldb` (files of a LevelDB are explained below).

### 1.3.2. On-disk File Structures

A typical file directory structure of a LevelDB database is:

```
db_name
├── 000003.log
├── 000004.ldb
├── CURRENT
├── LOCK
├── LOG
├── LOG.old
└── MANIFEST-000002
```

where:

- `[0-9]+.ldb`: SSTables.
- `[0-9]+.log`: Log files. A log file stores a sequence of recent updates. A `Write` operation first write to the log file then to the `MemTable`.
- `LOG`: Info logs. Informational messages from the database engine.
- `MANIFEST`: list of the sorted tables that make up each level.
- `CURRENT`: a simple text file that contains the name of the latest MANIFEST file.
- `LOCK`: Lock that used to avoid a database to be opened multiple times.

Also the files are explained in the [official documentation](https://github.com/google/leveldb/blob/master/doc/impl.md).

### 1.3.3. Utilities

In addition to the main components that implement the concept of original LSM-tree, LevelDB implements some utilities to optimize the performance of key-value storage. Utilities including

| Utility             | Source Files                                         | Notes                                    |
| ------------------- | ---------------------------------------------------- | ---------------------------------------- |
| `Varint`            | `util/coding.{h|cc}`                                 | A variable length integer implementation |
| `Comparator`        | `leveldb/include/comparator.h`, `util/comparator.cc` | Comparator to compare key and values     |
| `BloomFilterPolicy` | `util/bloom.cc`                                      | Bloom Filter                             |
| `Cache`             | `leveldb/include/cache.h`, `util/cache.cc`           | LRU Cache system                         |

> [Varints doc on Google developer](https://developers.google.com/protocol-buffers/docs/encoding#varints)

## 1.4. Implementation of Main Components

In this section we look into the implementations of the three major components of LevelDB: WAL Log, MemTable and SSTable.

### 1.4.1. WAL Log (TODO)

### 1.4.2. MemTable

MemTable class members:

```C++
// memtable.h
public:
  MemTable (const InternalKeyComparator &comparator);
  MemTable (const MemTable &)=delete;
  MemTable & 	operator= (const MemTable &)=delete;
  void Ref ();
  void Unref ();
  size_t ApproximateMemoryUsage ();
  Iterator * NewIterator ();
  void Add (SequenceNumber seq, ValueType type, const Slice &key, const Slice &value);
  bool Get (const LookupKey &key, std::string *value, Status *s);

private:
  friend class MemTableIterator;
  friend class MemTableBackwardIterator;
  typedef SkipList<const char*, KeyComparator> Table;
  ~MemTable();  // Private since only Unref() should be used to delete it
  KeyComparator comparator_;
  int refs_;
  Arena arena_;
  Table table_;
```

where the member `table_` of type `Table` that is actually storing data is an implementation of skip list.

`MemTable::Add()` will be called when a write operation occurs:

`DBImpl::Write()` -> `WriteBatchInternal::InsertInto(write_batch, mem_)`, This function creates a `MemTableInserter inserter` -> Iterate over the batch by `WriteBatch::Iterate(&inserter)`, calling `inserter.Put()` every iteration -> `inserter.Put()` calls `MemTable::Add()`

For main operation upon MemTable, refer to [Read-and-Write](Read-and-Write.md).

### 1.4.3. SSTable

The format of SSTable files are explained in [official documentation](https://github.com/google/leveldb/blob/master/doc/table_format.md):

```
<beginning_of_file>
[data block 1]
[data block 2]
...
[data block N]
[meta block 1]
...
[meta block K]
[metaindex block]
[index block]
[Footer]        (fixed size; starts at file_size - sizeof(Footer))
<end_of_file>
```

i.e., an SSTable file contains:

- N Data blocks
- K Meta blocks (e.g. bitmap generated by bloom filter; statistics)
- MetaIndex block
- Index block
- Footer

Related source files are:

- `table/format.{h|cc}`: `BlockHandle`, `Footer`
- `table/block.{h|cc}`: `Block`

where `BlockHandle` is a internal pointer type that contains:

```
offset:   varint64
size:     varint64
```

1. Data, i.e., a sequence of key-value pairs are stored in sorted order and partitioned into data blocks.
2. After the data blocks there are meta blocks. Examples of meta blocks are bitmaps generated by bloom filter according to `FilterPolicy` options on opening the database, and statistics blocks.
3. MetaIndex block contains one entry for a meta block, where key is the name of the meta block and the value is pointer (`BlockHandle`) pointing to that meta block.
4. Index block, similar to MetaIndex block, but pointing the data blocks. The key of each entry in the Index block is a string >= last key in the data block to which the entry points, and < the first key in the successive data block.
5. Footer is a fixed length component, containing: a `BlockHandle` pointing to the MetaIndex, and a `BlockHandle` pointing to the Index, and a magic number.

These components are build by `block_builder.cc`.

The format of a (Data) block is explained as:

```C++
// BlockBuilder generates blocks where keys are prefix-compressed:
//
// When we store a key, we drop the prefix shared with the previous
// string.  This helps reduce the space requirement significantly.
// Furthermore, once every K keys, we do not apply the prefix
// compression and store the entire key.  We call this a "restart
// point".  The tail end of the block stores the offsets of all of the
// restart points, and can be used to do a binary search when looking
// for a particular key.  Values are stored as-is (without compression)
// immediately following the corresponding key.
//
// An entry for a particular key-value pair has the form:
//     shared_bytes: varint32
//     unshared_bytes: varint32
//     value_length: varint32
//     key_delta: char[unshared_bytes]
//     value: char[value_length]
// shared_bytes == 0 for restart points.
//
// The trailer of the block has the form:
//     restarts: uint32[num_restarts]
//     num_restarts: uint32
// restarts[i] contains the offset within the block of the ith restart point.
```

## 1.5. Implementation of Utils (TODO)
