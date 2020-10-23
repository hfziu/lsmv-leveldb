File list
=========

Source files of LevelDB explained. Files are listed in alphabetic order (but directories are not: the major parts are listed first).

.. contents::

include/leveldb/
----------------
These are the main interfaces of LevelDB. That is, when installing LevelDB to a system, headers in this directory are typically installed to the default ``INCLUDE_PATH`` for use. For end users who use LevelDB to as the storage backend to build their own C++ application, typically they only need to include (some of) these headers in their application source files.

``c.h``
  C bindings for leveldb.

``cache.h``

``comparator.h``

``db.h``

``dumpfile.h``

``env.h``

``export.h``

``filter_policy.h``

``iterator.h``

``options.h``

``slice.h``

``status.h``

``table.h``

``table_builder.h``

``write_batch.h``


db/
---

table/
------


util/
-----

``cache.cc``
  pass

``cache_test.cc``
  pass


helpers/memenv/
---------------


issues/
-------


port/
-----


third_party/
------------


doc/
----
