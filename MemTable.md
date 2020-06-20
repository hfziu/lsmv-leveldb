# MemTable

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

where `Table` is an implementation of Skiplist. `MemTable::Add()` will be called when a write operation occurs:

`DBImpl::Write()` -> `WriteBatchInternal::InsertInto(write_batch, mem_)`, This function creates a `MemTableInserter inserter` -> Iterate over the batch by `WriteBatch::Iterate(&inserter)`, calling `inserter.Put()` every iteration -> `inserter.Put()` calls `MemTable::Add()`

For main operations on MemTable, refer to [Read-and-Write](Read-and-Write.md).

## Skip-List
