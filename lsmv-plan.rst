Plan for modifying LevelDB
==========================

.. contents::

Plan for modifying LevelDB into an LSMV-tree.

Changes will be maintained in a newly created git branch in forked levelDB repository.


Plan
----

MemTable
^^^^^^^^

From bottom to top:

- Skip list: no need to modify. Encoded InternalKey data is inserted to Skip list 
- MemTable: Add 



Data Validity Model and Data Entry Format
-----------------------------------------

We are going to modify LevelDB to support data validity model, i.e., data entry with lower valid time and upper valid time. The original LevelDB uses an InternalKey structure: :code:`< user_key | sequence_no | kType >`.

To support data version and validity model, we need to add lower valid time, upper valid time and pointer to previous version to InternalKey. 

Suppose our ParsedInternalKey is 

.. code:: cpp

  {
    Slice user_key;
    SequenceNumber sequence;
    ValueType type;
    TimeInMillis lower_valid_time;
    TimeInMillis upper_valid_time;
    ParsedInternalKey* previous_version;
  };

or append the previous version pointer to the data entry. (?)

Once the InternalKey and LookupKey structure has been changed, corresponding procedures need to be modified. Based on LevelDB interfaces, we consider 

- L0 Write
- Log Write
- Flush to Disk
- SSTable file format and block format
- Compaction
- Recovery
- Read (Query)


MemTable
^^^^^^^^

- Arena (class used to allocate memory)
- Skip-list: internal data structure in MemTable to store key/values.


Functions we plan to support
^^^^^^^^^^^^^^^^^^^^^^^^^^^^

(by priority)

#. Inserting record with current timestamp as lower valid time to MemTable
#. Set upper valid time for previous version on inserting
#. Versioned data support in LogWriter
#. Flushing versioned records to SSTable
#. Adapt Lookup procedure to multi-versioned data (exact-match)


L0 Write
^^^^^^^^

The critical path [#critial_path]_ for L0 Write is 

.. code::

  DBImpl::Write()
  -> LogWriter::AddRecord()
  -> WriteBatchInternal::InsertInto()
     -> MemTable::Add()
        -> Skiplist::Insert()
  -> VersionSet::SetLastSequence()


Affected source files when InternalKey structure changed:

- :code:`dbformat.h[.cc]` - ParsedInternalKey, AppendInternalKey, Comparator
- :code:`memtable.cc` - Change the behavior how :code:`MemTable::Add()` handle :code:`internal_key_size`

Log Write
^^^^^^^^^

Affected source files:

- ``log_writer.h[.cc]`` - Adapt the way LogWriter handle versioned data record

Flush to Disk and SSTable Format
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Critical path for Flush:

.. code::

  DBImpl::WriteLevel0Table()
  -> MemTable::NewIterator()
  -> BuildTable()
     -> Env::NewWritableFile()
     -> TableBuilder::Add()
        -> BlockBuilder::Add()
     -> TableBuilder::Finish()
        -> WriteBlock()
           -> WritableFile::Append()
     -> TableCache::NewIterator()
   -> Version::PickLevelForMemTableOutput()
   -> VersionEdit::AddFile()

Affected source files:

- ``db/db_impl.h[.cc]`` - ``WriteLevel0Table``
- ``include/builder.h,table/builder.cc`` - The main entrance for building a disk table
- ``table/block_builder.h[.cc]`` - Class ``BlockBuilder``
- ``db/version_edit.h[.cc]`` - Class ``VersionEdit``


.. Compaction
.. ^^^^^^^^^^

.. Recovery
.. ^^^^^^^^



Membership Procedure
--------------------

Two possible options:

#. Keep the functions of multi-version database engine unchanged, let application clients perform the membership procedure.
#. Add this function as a new feature to the db engine.


Component Time Interval
-----------------------

Add lower and upper valid time to block file.

Affected source files:

- ``db/builder.cc``
- ``table/table_builder.cc``


Handling Multi-Component Versions
---------------------------------

#. Point to previous version: Set the previous version pointer pointing to the address 
#. Handling valid time of multi-component data

Affected procedures:

- Flush (Set pointer when a new component generated)
- Read (Recognize multi-component data version when perform query operations)


Query Algorithms and Interfaces
-------------------------------

Four Query Algorithms: 

- Exact-Match Query
- Key-Range Query
- Time-Range Query
- Key-Range-and-Time-Range Query

Critical path for Read:

::

  DBImpl::Get() 
  -> MemTable::Get() 
  -> Version::Get()
     -> Version::FindFile()
     -> TableCache::Get()
        -> TableCache::FindTable()
           -> Cache::Lookup()
           -> Table::Open()
              -> RandomAccessFile::Read()
                -> ReadBlock()
                   -> RandomAccessFile::Read()
            -> Cache::Insert()
        -> Table::InternalGet() 
           -> Block::NewIterator()
           -> Iterator::Seek()
           -> Filter:KeyMayMatch()
           -> BlockReader()
              -> Cache::Lookup()
              -> ReadBlock()
                 -> RandomAccessFile::Read()
              -> Cache::Insert() 
           	-> Iterator::Seek()

Adapt the functions above to server multi-version query based on algorithm design.


References
----------

.. [#critial_path] https://chenju2k6.github.io/blog/2018/11/leveldb