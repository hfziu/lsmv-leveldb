# LevelDB code analysis

This is repository includes code analysis documents for [google/leveldb](https://github.com/google/leveldb).

## Table of Contents and Schedule

### LevelDB Code Analysis

#### Demo (~ end of May 2020)

[C++ demos](demo/) for making use of leveldb library.

#### [Classes](Classes.md)

This part includes class definitions used by the following contents.

#### [Write and Read Operations](Read-and-Write.md) (~ Week 1, June 2020)

This part of doc will include 
- API for write and read
- Overview of data structure involved, i.e. batch, log, memtable, sstable
- The process of write and read
- Snapshot mechanism

A write operation (a `Put()` or a `Delete()`) first write data to log file and then write to in-memory table. To make use of Atomical operations, user can create `WriteBatch` objects to do batch writing.

#### Implementation of in-memory table (~ Week 2, June 2020)

At present, the [in-memory table](https://github.com/google/leveldb/blob/master/db/memtable.h) of LevelDB is implemented as a [skip-list](https://github.com/google/leveldb/blob/master/db/skiplist.h). In this part I will look into the implementation of the skip-list.

#### Sorted tables and manifest (~ Week 2, June 2020)

On-disk file structures and their implementations and file operations in the source code.

- Log files (*.log)
- Sorted tables (*.ldb)
- Manifest (like index of the on-disk tables)
- CURRENT: a simple text file that contains the name of the latest MANIFEST file.
- Info logs (LOG and LOG.old)
- Others (LOCK, *.dbtmp)

#### Compactions (~ Week 3, June 2020)

#### Bloom Filters and Other Utilities (~ Week 4, June 2020)

#### Recovery (~ Week 4, June 2020)

### Multi-Version Algorithm Design