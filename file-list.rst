File list
=========

Source files of LevelDB explained. Files are listed in alphabetic order (but directories are not: the major parts are listed first).

**Current status:** This file created.

**Notes:** all ``*_test.cc`` files are unit tests written with Google test framework.

.. contents::

include/leveldb/
----------------
These are the main interfaces of LevelDB. That is, when installing LevelDB to a system, headers in this directory are typically installed to the default ``INCLUDE_PATH`` for use. For end users who use LevelDB to as the storage backend to build their own C++ application, typically they only need to include (some of) these headers in their application source files.

``c.h``
  C bindings for leveldb.

``cache.h``

``comparator.h``
  Comparator object that provides a total order across slices that are used as keys in an sstable or a database.
  
  ``class Comparator``
    
    - ``Compare(const Slice& a, const Slice& b)``
    - ``Name()`` - Name of the comparator
    - ``FindShortestSeparator(string* start, const Slice& limit)`` - if ``*start < limit``, changes ``*start`` to a short string in ``[start, limit)``. An implementation of this method that does nothing is correct.
    - ``FindShortSuccessor()`` changes ``*key`` to a short string. An implementation of this method that does nothing is correct.
    - ``LEVELDB_EXPORT const Comparator* BytewiseComparator``

``db.h``

  ``class Snapshot``

  ``struct Range``
    A range of keys

    - ``Slice start``
    - ``Slice limit``

  ``class DB``
    main interface of LevelDB.

    - ``Status Open(Options& options, string& name, DB** dbptr)``
    - ``DB()`` - default constructor
    - ``~DB()`` - destructor
    - ``virtual Status Put(WriteOptions& options, Slice& key, Slice& value)``
    - ``virtual Status Delete(WriteOptions& options, Slice& key)``
    - ``virtual Status Write(WriteOptions& options, WriteBatch* updates)`` - Apply the specified updates to the database.
    - ``virtual Status Get(ReadOptions& options, Slice& key, string* value)`` - If the database contains an entry for "key" store the corresponding value in ``*value`` and return OK. If there is no entry for "key" leave ``*value`` unchanged and return a status for which Status::IsNotFound() returns true.
    - ``virtual Iterator* NewIterator(ReadOptions& options)``
    - ``virtual Snapshot* GetSnapshot()``
    - ``virtual void ReleaseSnapshot(Snapshot* snapshot)``
    - ``virtual bool GetProperty(Slice& property, string* value)``
    - ``virtual GetApproximateSizes(Range* range, int n, unit64_t* sizes)``
    - ``virtual void CompactRange(const Slice* begin, const Slice* end)``
    - ``Status DestroyDB(const std::string& name, const Options& options)``
    - ``Status RepairDB(const std::string& dbname, const Options& options)``

``dumpfile.h``
  ``Status DumpFile(Env* env, const std::string& fname, WritableFile* dst)``

``env.h``
  .. code-block:: cpp

    virtual Status  NewSequentialFile (const std::string &fname, SequentialFile **result)=0
    virtual Status  NewRandomAccessFile (const std::string &fname, RandomAccessFile **result)=0
    virtual Status  NewWritableFile (const std::string &fname, WritableFile **result)=0
    virtual Status  NewAppendableFile (const std::string &fname, WritableFile **result)
    virtual bool  FileExists (const std::string &fname)=0
    virtual Status  GetChildren (const std::string &dir, std::vector< std::string > *result)=0
    virtual Status  RemoveFile (const std::string &fname)
    virtual Status  DeleteFile (const std::string &fname)
    virtual Status  CreateDir (const std::string &dirname)=0
    virtual Status  RemoveDir (const std::string &dirname)
    virtual Status  DeleteDir (const std::string &dirname)
    virtual Status  GetFileSize (const std::string &fname, uint64_t *file_size)=0
    virtual Status  RenameFile (const std::string &src, const std::string &target)=0
    virtual Status  LockFile (const std::string &fname, FileLock **lock)=0
    virtual Status  UnlockFile (FileLock *lock)=0
    virtual void  Schedule (void(*function)(void *arg), void *arg)=0
    virtual void  StartThread (void(*function)(void *arg), void *arg)=0
    virtual Status  GetTestDirectory (std::string *path)=0
    virtual Status  NewLogger (const std::string &fname, Logger **result)=0
    virtual uint64_t  NowMicros ()=0
    virtual void  SleepForMicroseconds (int micros)=0

``export.h``

``filter_policy.h``
  ``class FilterPolicy``

  .. code-block:: cpp

    virtual void CreateFilter (const Slice *keys, int n, std::string *dst) const =0
    virtual bool KeyMayMatch (const Slice &key, const Slice &filter) const =0

``iterator.h``
  ``class Iterator``

  .. code-block:: cpp

    virtual bool Valid () const =0
    virtual void SeekToFirst ()=0
    virtual void SeekToLast ()=0
    virtual void Seek (const Slice &target)=0
    virtual void Next ()=0
    virtual void Prev ()=0
    virtual Slice key () const =0
    virtual Slice value () const =0
    virtual Status status () const =0
    void RegisterCleanup (CleanupFunction function, void *arg1, void *arg2)

``options.h``
  ``struct Options``

  .. code-block:: cpp

    const Comparator *  comparator
    bool create_if_missing = false
    bool error_if_exists = false
    bool paranoid_checks = false
    Env * env
    Logger *  info_log = nullptr
    size_t  write_buffer_size = 4 * 1024 * 1024
    int max_open_files = 1000
    Cache * block_cache = nullptr
    size_t  block_size = 4 * 1024
    int block_restart_interval = 16
    size_t  max_file_size = 2 * 1024 * 1024
    CompressionType compression = kSnappyCompression 
    bool  reuse_logs = false
    const FilterPolicy *  filter_policy = nullptr

``slice.h``
  ``class Slice``

  .. code-block:: cpp

    Slice (const char *d, size_t n)
    Slice (const std::string &s)
    Slice (const char *s)
    Slice (const Slice &)=default
    Slice & operator= (const Slice &)=default
    const char* data () const
    size_t size () const
    bool empty () const
    char operator[] (size_t n) const
    void clear ()
    void remove_prefix (size_t n)
    std::string ToString () const
    int compare (const Slice &b) const
    bool starts_with (const Slice &x) const

``status.h``
  ``class Status``

  .. code-block:: cpp

    Status (const Status &rhs)
    Status &  operator= (const Status &rhs)
    Status (Status &&rhs) noexcept
    Status &  operator= (Status &&rhs) noexcept
    bool ok () const
    bool IsNotFound () const
    bool IsCorruption () const
    bool IsIOError () const
    bool IsNotSupportedError () const
    bool IsInvalidArgument () const
    std::string ToString () const
    static Status OK ()
    static Status NotFound (const Slice &msg, const Slice &msg2=Slice())
    static Status Corruption (const Slice &msg, const Slice &msg2=Slice())
    static Status NotSupported (const Slice &msg, const Slice &msg2=Slice())
    static Status InvalidArgument (const Slice &msg, const Slice &msg2=Slice())
    static Status IOError (const Slice &msg, const Slice &msg2=Slice())

``table.h``
  ``class Table``

  .. code-block:: cpp

    Table (const Table &)=delete
    Table & operator= (const Table &)=delete
    Iterator * NewIterator (const ReadOptions &) const
    uint64_t ApproximateOffsetOf (const Slice &key) const
    static Status Open (const Options &options, RandomAccessFile *file, uint64_t file_size, Table **table)  

``table_builder.h``
  ``class TableBuilder``

  .. code-block:: cpp

    TableBuilder (const Options &options, WritableFile *file)
    TableBuilder (const TableBuilder &)=delete
    TableBuilder &  operator= (const TableBuilder &)=delete
    Status ChangeOptions (const Options &options)
    void Add (const Slice &key, const Slice &value)
    void Flush ()
    Status status () const
    Status Finish ()
    void Abandon ()
    uint64_t NumEntries () const
    uint64_t FileSize () const

``write_batch.h``
  ``class WriteBatch``

  .. code-block:: cpp

    WriteBatch (const WriteBatch &)=default
    WriteBatch & operator= (const WriteBatch &)=default
    void Put (const Slice &key, const Slice &value)
    void Delete (const Slice &key)
    void Clear ()
    size_t ApproximateSize () const
    void Append (const WriteBatch &source)
    Status Iterate (Handler *handler) const

  member ``class Handler``

  .. code-block:: cpp

    virtual void Put (const Slice &key, const Slice &value)=0
    virtual void Delete (const Slice &key)=0 


db/
---

``autocompact_test.cc``
  .. code-block:: cpp

``builder.cc``
  See ``builder.h``

``builder.h``
  .. code-block:: cpp

      Status BuildTable(const std::string& dbname, Env* env, const Options& options,
                  TableCache* table_cache, Iterator* iter, FileMetaData* meta);

``c.cc``
  .. code-block:: cpp

``c_test.c``
  .. code-block:: cpp

``corruption_test.cc``
  .. code-block:: cpp

``db_impl.cc``
  See ``db_impl.h``

``db_impl.h``
  ``class DBImpl``

  .. code-block:: cpp

    // public members
    DBImpl (const Options &options, const std::string &dbname)DBImpl (const 
    DBImpl &)=delete
    DBImpl & operator= (const DBImpl &)=delete
    ~DBImpl () override
    Status Put (const WriteOptions &, const Slice &key, const Slice &value) override
    Status Delete (const WriteOptions &, const Slice &key) override
    Status Write (const WriteOptions &options, WriteBatch *updates) override
    Status Get (const ReadOptions &options, const Slice &key, std::string *value) override
    Iterator * NewIterator (const ReadOptions &) override
    const Snapshot * GetSnapshot () override
    void ReleaseSnapshot (const Snapshot *snapshot) override
    bool GetProperty (const Slice &property, std::string *value) override
    void GetApproximateSizes (const Range *range, int n, uint64_t *sizes) override
    void CompactRange (const Slice *begin, const Slice *end) override
    void TEST_CompactRange (int level, const Slice *begin, const Slice *end)
    Status TEST_CompactMemTable ()
    Iterator * TEST_NewInternalIterator ()
    int64_t TEST_MaxNextLevelOverlappingBytes ()
    void RecordReadSample (Slice key)
    // private members
    Iterator *  NewInternalIterator (const ReadOptions &, SequenceNumber *latest_snapshot, uint32_t *seed)
    Status NewDB ()
    Status Recover (VersionEdit *edit, bool *save_manifest) EXCLUSIVE_LOCKS_REQUIRED(mutex_)
    void MaybeIgnoreError (Status *s) const
    void RemoveObsoleteFiles () EXCLUSIVE_LOCKS_REQUIRED(mutex_)
    void CompactMemTable () EXCLUSIVE_LOCKS_REQUIRED(mutex_)
    Status RecoverLogFile (uint64_t log_number, bool last_log, bool *save_manifest, VersionEdit *edit, SequenceNumber *max_sequence) EXCLUSIVE_LOCKS_REQUIRED(mutex_)
    Status WriteLevel0Table (MemTable *mem, VersionEdit *edit, Version *base) EXCLUSIVE_LOCKS_REQUIRED(mutex_)
    Status MakeRoomForWrite (bool force) EXCLUSIVE_LOCKS_REQUIRED(mutex_)
    WriteBatch * BuildBatchGroup (Writer **last_writer) EXCLUSIVE_LOCKS_REQUIRED(mutex_)
    void RecordBackgroundError (const Status &s)
    void MaybeScheduleCompaction () EXCLUSIVE_LOCKS_REQUIRED(mutex_)
    void BackgroundCall ()
    void BackgroundCompaction () EXCLUSIVE_LOCKS_REQUIRED(mutex_)
    void CleanupCompaction (CompactionState *compact) EXCLUSIVE_LOCKS_REQUIRED(mutex_)
    Status DoCompactionWork (CompactionState *compact) EXCLUSIVE_LOCKS_REQUIRED(mutex_)
    Status OpenCompactionOutputFile (CompactionState *compact)
    Status FinishCompactionOutputFile (CompactionState *compact, Iterator *input)
    Status InstallCompactionResults (CompactionState *compact) EXCLUSIVE_LOCKS_REQUIRED(mutex_)
    const Comparator * user_comparator () const
    port::CondVar background_work_finished_signal_ GUARDED_BY (mutex_)
    MemTable *imm_ GUARDED_BY (mutex_)
    uint64_t logfile_number_ GUARDED_BY (mutex_)
    uint32_t seed_ GUARDED_BY (mutex_)
    std::deque< Writer * > writers_ GUARDED_BY (mutex_)
    WriteBatch *tmp_batch_  GUARDED_BY (mutex_)
    SnapshotList snapshots_ GUARDED_BY (mutex_)
    std::set< uint64_t > pending_outputs_ GUARDED_BY (mutex_)
    bool background_compaction_scheduled_ GUARDED_BY (mutex_)
    ManualCompaction *manual_compaction_  GUARDED_BY (mutex_)
    VersionSet *const versions_ GUARDED_BY (mutex_)
    Status bg_error_  GUARDED_BY (mutex_)
    CompactionStats stats_[config::kNumLevels]  GUARDED_BY (mutex_)


``db_iter.cc``
  .. code-block:: cpp

``db_iter.h``
  See ``db_iter.cc``.

``db_test.cc``
  .. code-block:: cpp

``dbformat.cc``
  See `dbformat.h`

``dbformat.h``
  
  - ``namespace config``
  - ``class InternalKey``
  - ``struct PArsedInternalKey``
  - ``void AppendInternalKey(std::string* result, const ParsedInternalKey& key)``
  - ``bool ParseInternalKey(const Slice& internal_key, ParsedInternalKey* result)``
  - ``inline Slice ExtractUserKey(const Slice& internal_key)``
  - ``class InternalKeyComparator : public Comparator``
  - ``class InternalFilterPolicy : public FilterPolicy``
  - ``class LookupKey``

``dbformat_test.cc``
  .. code-block:: cpp

``dumpfile.cc``
  See ``include/dumpfile.h``, and
  
  .. code-block:: cpp

    bool GuessType(const std::string& fname, FileType* type)
    class CorruptionReporter : public log::Reader::Reporter
    Status PrintLogContents(Env* env, const std::string& fname,
                        void (*func)(uint64_t, Slice, WritableFile*),
                        WritableFile* dst)
    class WriteBatchItemPrinter : public WriteBatch::Handler
    static void WriteBatchPrinter(uint64_t pos, Slice record, WritableFile* dst)
    Status DumpLog(Env* env, const std::string& fname, WritableFile* dst)
    static void VersionEditPrinter(uint64_t pos, Slice record, WritableFile* dst)
    Status DumpDescriptor(Env* env, const std::string& fname, WritableFile* dst)
    Status DumpTable(Env* env, const std::string& fname, WritableFile* dst)

``fault_injection_test.cc``
  .. code-block:: cpp

``filename.cc``
  See ``filename.h``

``filename.h``
  .. code-block:: cpp

    std::string LogFileName(const std::string& dbname, uint64_t number)
    std::string TableFileName(const std::string& dbname, uint64_t number)
    std::string SSTTableFileName(const std::string& dbname, uint64_t number)
    std::string DescriptorFileName(const std::string& dbname, uint64_t number)
    std::string CurrentFileName(const std::string& dbname)
    std::string LockFileName(const std::string& dbname)
    std::string TempFileName(const std::string& dbname, uint64_t number)
    std::string InfoLogFileName(const std::string& dbname)
    std::string OldInfoLogFileName(const std::string& dbname)
    bool ParseFileName(const std::string& filename, uint64_t* number, FileType* type)
    Status SetCurrentFile(Env* env, const std::string& dbname, uint64_t descriptor_number)

``filename_test.cc``
  .. code-block:: cpp

``leveldbutil.cc``
  This is an executable util.

  .. code-block:: cpp

    class StdoutPrinter : public WritableFile
    bool HandleDumpCommand(Env* env, char** files, int num)
    static void Usage()
    int main(int argc, char** argv)

``log_format.h``
  .. code-block:: cpp

    enum RecordType
    static const int kMaxRecordType = kLastType
    static const int kBlockSize = 32768
    static const int kHeaderSize = 4 + 2 + 1

``log_reader.cc``
  See ``log_reader.h``.

``log_reader.h``
  ``class Reader``

  .. code-block:: cpp
    
    // public
    Reader (SequentialFile *file, Reporter *reporter, bool checksum, uint64_t initial_offset)
    Reader (const Reader &)=delete
    Reader & operator= (const Reader &)=delete
    ~Reader ()
    bool ReadRecord (Slice *record, std::string *scratch)
    uint64_t LastRecordOffset ()
    // private
    bool SkipToInitialBlock ()
    unsigned int ReadPhysicalRecord (Slice *result)
    void ReportCorruption (uint64_t bytes, const char *reason)
    void ReportDrop (uint64_t bytes, const Status &reason)

``log_test.cc``
  Unit test.

``log_writer.cc``
  See ``log_writer.h``

``log_writer.h``
  ``class Wrtier``

  .. code-block:: cpp

    Writer (WritableFile *dest)
    Writer (WritableFile *dest, uint64_t dest_length)
    Writer (const Writer &)=delete
    Writer & operator= (const Writer &)=delete
    ~Writer ()
    Status AddRecord (const Slice &slice)

``memtable.cc``
  See ``memtable.h``

``memtable.h``
  ``class MemTable``

  .. code-block:: cpp
    
    MemTable (const InternalKeyComparator &comparator)
    MemTable (const MemTable &)=delete
    MemTable & operator= (const MemTable &)=delete
    void Ref ()
    void Unref ()
    size_t ApproximateMemoryUsage ()
    Iterator * NewIterator ()
    void Add (SequenceNumber seq, ValueType type, const Slice &key, const Slice &value)
    bool Get (const LookupKey &key, std::string *value, Status *s)
        

``recovery_test.cc``
  Unit test for recovery.

``repair.cc``
  ``class Repairer``

  .. code-block:: cpp

    // public
    Repairer (const std::string &dbname, const Options &options)
    ~Repairer ()
    Status Run ()
    // private
    Status FindFiles ()
    void ConvertLogFilesToTables ()
    Status ConvertLogToTable (uint64_t log)
    void ExtractMetaData ()
    Iterator * NewTableIterator (const FileMetaData &meta)
    void ScanTable (uint64_t number)
    void RepairTable (const std::string &src, TableInfo t)
    Status WriteDescriptor ()
    void ArchiveFile (const std::string &fname)


``skiplist.h``
  ``class Skiplist``

  .. code-block:: cpp

    // public
    SkipList (Comparator cmp, Arena *arena)
    SkipList (const SkipList &)=delete
    SkipList & operator= (const SkipList &)=delete
    void Insert (const Key &key)  
    bool Contains (const Key &key) const
    // private
    int GetMaxHeight () const
    Node * NewNode (const Key &key, int height)
    int RandomHeight ()
    bool Equal (const Key &a, const Key &b) const
    bool KeyIsAfterNode (const Key &key, Node *n) const
    Node * FindGreaterOrEqual (const Key &key, Node **prev) const
    Node * FindLessThan (const Key &key) const
    Node * FindLast () const


``skiplist_test.cc``
  Unit test for skip-list (integer as key). 

``snapshot.h``
  ``class SnapshotList``

  .. code-block:: cpp
    SnapshotList ()
    bool empty () const
    SnapshotImpl * oldest () const
    SnapshotImpl * newest () const
    SnapshotImpl * New (SequenceNumber sequence_number)
    void Delete (const SnapshotImpl *snapshot)


``table_cache.cc``
  See ``table_cache.h``

``table_cache.h``
  ``class TableCache``

  .. code-block:: cpp

    TableCache (const std::string &dbname, const Options &options, int entries)
    ~TableCache ()
    Iterator * NewIterator (const ReadOptions &options, uint64_t file_number, uint64_t file_size, Table **tableptr=nullptr)
    Status Get (const ReadOptions &options, uint64_t file_number, uint64_t file_size, const Slice &k, void *arg, void(*handle_result)(void *, const Slice &, const Slice &))
    void Evict (uint64_t file_number)

``version_edit.cc``
  See ``version_edit.h``.

``version_edit.h``
  ``class VersionEdit``

  .. code-block:: cpp

    VersionEdit ()
    ~VersionEdit ()=default
    void Clear ()
    void SetComparatorName (const Slice &name)
    void SetLogNumber (uint64_t num)
    void SetPrevLogNumber (uint64_t num)
    void SetNextFile (uint64_t num)
    void SetLastSequence (SequenceNumber seq)
    void SetCompactPointer (int level, const InternalKey &key)
    void AddFile (int level, uint64_t file, uint64_t file_size, const InternalKey &smallest, const InternalKey &largest)
    void RemoveFile (int level, uint64_t file)
    void EncodeTo (std::string *dst) const
    Status DecodeFrom (const Slice &src)
    std::string DebugString () const
    

``version_set.cc``
  See ``version_set.h``

``version_set.h``
  ``class VersionSet``

  .. code-block:: cpp

    // public
    VersionSet (const std::string &dbname, const Options *options, TableCache *table_cache, const InternalKeyComparator *)
    VersionSet (const VersionSet &)=delete
    VersionSet & operator= (const VersionSet &)=delete
    ~VersionSet ()
    Status LogAndApply (VersionEdit *edit, port::Mutex *mu) EXCLUSIVE_LOCKS_REQUIRED(mu)
    Status Recover (bool *save_manifest)
    Version * current () const
    uint64_t ManifestFileNumber () const
    uint64_t NewFileNumber ()
    void ReuseFileNumber (uint64_t file_number)
    int NumLevelFiles (int level) const
    int64_t NumLevelBytes (int level) const
    uint64_t LastSequence () const
    void SetLastSequence (uint64_t s)
    void MarkFileNumberUsed (uint64_t number)
    uint64_t LogNumber () const
    uint64_t PrevLogNumber () const
    Compaction * PickCompaction ()
    Compaction * CompactRange (int level, const InternalKey *begin, const InternalKey *end)
    int64_t MaxNextLevelOverlappingBytes ()
    Iterator * MakeInputIterator (Compaction *c)
    bool NeedsCompaction () const
    void AddLiveFiles (std::set< uint64_t > *live)
    uint64_t ApproximateOffsetOf (Version *v, const InternalKey &key)
    const char * LevelSummary (LevelSummaryStorage *scratch) const
    // private
    bool ReuseManifest (const std::string &dscname, const std::string &dscbase)
    void Finalize (Version *v)
    void GetRange (const std::vector< FileMetaData * > &inputs, InternalKey *smallest, InternalKey *largest)
    void GetRange2 (const std::vector< FileMetaData * > &inputs1, const std::vector< FileMetaData * > &inputs2, InternalKey *smallest, InternalKey *largest)
    void SetupOtherInputs (Compaction *c)
    Status WriteSnapshot (log::Writer *log)
    void AppendVersion (Version *v)


``version_set_test.cc``
  Unit test for version_set.

``write_batch.cc``
  See ``include/write_batch.h`` and ``write_batch_internal.h``.

``write_batch_internal.h``
  ``class WriteBatchInternal``

  .. code-block:: cpp

    static int Count (const WriteBatch *batch)
    static void SetCount (WriteBatch *batch, int n)
    static SequenceNumber Sequence (const WriteBatch *batch)
    static void SetSequence (WriteBatch *batch, SequenceNumber seq)
    static Slice  Contents (const WriteBatch *batch)
    static size_t ByteSize (const WriteBatch *batch)
    static void SetContents (WriteBatch *batch, const Slice &contents)
    static Status InsertInto (const WriteBatch *batch, MemTable *memtable)
    static void Append (WriteBatch *dst, const WriteBatch *src)

``write_batch_test.cc``
  Unit test for write_batch.

table/
------

``block.cc``
  See ``block.h``.

``block.h``
  ``class Block``

  .. code-block:: cpp

    // public functions
    Block (const BlockContents &contents)
    Block (const Block &)=delete
    Block & operator= (const Block &)=delete
    ~Block ()
    size_t size () const
    Iterator * NewIterator (const Comparator *comparator)
    // private functions
    uint32_t NumRestarts () const
    // private attributes
    const char * data_
    size_t size_
    uint32_t restart_offset_
    bool owned_
    

``block_build.cc``
  See ``block_builder.h``.

``block_builder.h``
  ``class BlockBuilder``

  .. code-block:: cpp

    BlockBuilder (const Options *options)
    BlockBuilder (const BlockBuilder &)=delete
    BlockBuilder & operator= (const BlockBuilder &)=delete
    void Reset ()
    void Add (const Slice &key, const Slice &value)
    Slice Finish ()
    size_t CurrentSizeEstimate () const
    bool empty () const

``filter_block.cc``
  See ``filter_block.h``.

``filter_block.h``
  ``class FilterBlockBuilder``

  .. code-block:: cpp

    FilterBlockBuilder (const FilterPolicy *)
    FilterBlockBuilder (const FilterBlockBuilder &)=delete
    FilterBlockBuilder & operator= (const FilterBlockBuilder &)=delete
    void StartBlock (uint64_t block_offset)
    void AddKey (const Slice &key)
    Slice Finish ()

  ``class FilterBlockReader``

  .. code-block:: cpp

    FilterBlockReader (const FilterPolicy *policy, const Slice &contents)
    bool KeyMayMatch (uint64_t block_offset, const Slice &key)

``filter_block_test.cc``
  Unit test for filter_block.

``format.cc``
  See ``format.h``.

``format.h``
  ``class BlockHandle``

  .. code-block:: cpp

    BlockHandle ()
    uint64_t offset () const
    void set_offset (uint64_t offset)
    uint64_t size () const
    void set_size (uint64_t size)
    void EncodeTo (std::string *dst) const
    Status DecodeFrom (Slice *input)

``iterator.cc``
  An implementation of ``include/iterator.h``.

``iterator_wrapper.h``
  ``class IteratorWrapper``

  .. code-block:: cpp

    IteratorWrapper ()
    IteratorWrapper (Iterator *iter)
    ~IteratorWrapper ()
    Iterator * iter () const
    void Set (Iterator *iter)
    bool Valid () const
    Slice key () const
    Slice value () const
    Status status () const
    void Next ()
    void Prev ()
    void Seek (const Slice &k)
    void SeekToFirst ()
    void SeekToLast ()


``merger.cc``
  See ``merger.h``.

``merger.h``
  ``class MergingIterator : Iterator``

  .. code-block:: cpp

     MergingIterator (const Comparator *comparator, Iterator **children, int n)
    ~MergingIterator () override
    bool Valid () const override
    void SeekToFirst () override
    void SeekToLast () override
    void Seek (const Slice &target) override
    void Next () override
    void Prev () override
    Slice key () const override
    Slice value () const override
    Status status () const override

``table.cc``
  An implementation of ``include/table.h``.

``table_builder.cc``
  An implementation of ``include/table_builder.h``.

``table_test.cc``
  Unit test.

``two_level_iterator.cc``
  See ``two_level_iterator.h``.

``two_level_iterator.h``
  ``class TwoLevelIterator : public Iterator``

  .. code-block:: cpp

    TwoLevelIterator (Iterator *index_iter, BlockFunction block_function, void *arg, const ReadOptions &options)
    ~TwoLevelIterator () override
    void Seek (const Slice &target) override
    void SeekToFirst () override
    void SeekToLast () override
    void Next () override
    void Prev () override
    bool Valid () const override
    Slice key () const override
    Slice value () const override
    Status status () const override


util/
-----

``arena.cc``
  See ``arena.h``.

``arena.h``
  Used to allocate memory.
  ``class Arena``.

  .. code-block:: cpp

    Arena ()
    Arena (const Arena &)=delete
    Arena & operator= (const Arena &)=delete
    ~Arena ()
    char * Allocate (size_t bytes)
    char * AllocateAligned (size_t bytes)
    size_t MemoryUsage () const

``arena_test.cc``
  Unit test.

``bloom.cc``
  ``class BloomFilterPolicy : public FilterPolicy``

  .. code-block:: cpp

    BloomFilterPolicy (int bits_per_key)
    const char *  Name () const override
    void CreateFilter (const Slice *keys, int n, std::string *dst) const override
    bool KeyMayMatch (const Slice &key, const Slice &bloom_filter) const override


``bloom_test.cc``
  Unit test.

``cache.cc``
  - ``class LRUCache``
  - ``class HandleTable``
  - ``struct LRUHandle``

``cache_test.cc``
  Unit test.

``coding.cc``
  See ``coding.h``.

``coding.h``
  Encoding functions for Varint encoding, decoding, etc.

  .. code-block:: cpp

    void PutFixed32(std::string* dst, uint32_t value);
    void PutFixed64(std::string* dst, uint64_t value);
    void PutVarint32(std::string* dst, uint32_t value);
    void PutVarint64(std::string* dst, uint64_t value);
    void PutLengthPrefixedSlice(std::string* dst, const Slice& value);

    bool GetVarint32(Slice* input, uint32_t* value);
    bool GetVarint64(Slice* input, uint64_t* value);
    bool GetLengthPrefixedSlice(Slice* input, Slice* result);

    const char* GetVarint32Ptr(const char* p, const char* limit, uint32_t* v);
    const char* GetVarint64Ptr(const char* p, const char* limit, uint64_t* v);

    int VarintLength(uint64_t v);

    char* EncodeVarint32(char* dst, uint32_t value);
    char* EncodeVarint64(char* dst, uint64_t value);

``coding_test.cc``
  Unit test.

``comparator.cc``
  ``class BytewiseComparatorImpl : public Comparator``

  .. code-block:: cpp

    BytewiseComparatorImpl ()=default
    const char * Name () const override
    int Compare (const Slice &a, const Slice &b) const override
    void FindShortestSeparator (std::string *start, const Slice &limit) const override
    void FindShortSuccessor (std::string *key) const override


``crc32c.cc``
  See ``crc32.h``.

``crc32c.h``
  crc32 checksum util.

``crc32c_test.cc``
  Unit test.

``env.cc``
  See ``include/env.h``

``env_posix.cc``

``env_posix_test.cc``

``env_posix_test_helper.cc``

``env_test.cc``

``env_windows.cc``

``env_windows_test.cc``

``env_windows_test_helper.h``

``filter_policy.cc``
  Empty Destructor of class ``FilterPolicy``.

``hash.cc``
  See ``hash.h``.

``hash.h``

  .. code-block:: cpp

    uint32_t Hash(const char* data, size_t n, uint32_t seed)

``hash_test.cc``
  Unit test for hash.

``histogram.cc``
  See ``histogram.h``.

``histogram.h``

  .. code-block:: cpp
    Histogram ()
    ~Histogram ()
    void Clear ()
    void Add (double value)
    void Merge (const Histogram &other)
    std::string ToString () const

``logging.cc``
  See ``logging.h``

``logging.h``

  .. code-block:: cpp

    void AppendNumberTo(std::string* str, uint64_t num)
    void AppendEscapedStringTo(std::string* str, const Slice& value)
    std::string NumberToString(uint64_t num)
    std::string EscapeString(const Slice& value)

``logging_test.cc``
  Unit test.

``mutexlock.h``
  ``class MutexLock``

  .. code-block:: cpp

    MutexLock (port::Mutex *mu) EXCLUSIVE_LOCK_FUNCTION(mu)
    ~MutexLock () UNLOCK_FUNCTION()
    MutexLock (const MutexLock &)=delete
    MutexLock & operator= (const MutexLock &)=delete


``no_destrutor.h``
  ``class NoDestructor``

  .. code-block:: cpp

    NoDestructor (ConstructorArgTypes &&... constructor_args)
    ~NoDestructor ()=default
    NoDestructor (const NoDestructor &)=delete
    NoDestructor & operator= (const NoDestructor &)=delete
    InstanceType * get ()

``no_destrutor_test.cc``
  Unit test.

``options.cc``
  An implementation of ``include/options.h``.

``posix_logger.h``
  ``class PosixLogger final : public Logger``

  .. code-block:: cpp

    PosixLogger (std::FILE *fp)
    ~PosixLogger () override
    void Logv (const char *format, std::va_list arguments) override


``random.h``
  A simple random number generator.

``status.cc``
  An implementation of ``include/status.h``.

``status_test.cc``
  Unit test for status.

``testutil.cc``
  Test random or compressible strings.

``testutil.h``
  See ``testutil.cc``.

``windows_logger.h``
  Logger implementation for the Windows platform.


issues/
-------

``issue178_test.cc``
  Test for `issue 178 <https://github.com/google/leveldb/issues/184>`_: a manual compaction causes deleted data to reappear.

``issue200_test.cc``

``issue320_test.cc``


port/
-----

Port/ directory contains interfaces and implementations that isolate the
rest of the package from platform details.

``port.h``

``port_config.h.in``

``port_example.h``

``port_stdcxx.h``

``thread_annotations.h``


third_party/googletest
----------------------

The GoogleTest module. This directory serves as a git sub-module here and technically it's not part of LevelDB. It's a framework used to write unit tests. Project `homepage <https://github.com/google/googletest>`_.




doc/
----
