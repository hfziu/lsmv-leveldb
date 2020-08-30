DB Bench records
================

Note: If note specified, all benchmarks are run on a CityU CSLab PC which is equipped with:

.. code::

  OS: Ubuntu 20.04.1 LTS x86_64
  Kernel: 5.4.0-42-generic
  CPU: Intel i7-9700K (8) @ 4.900GHz
  RAM: 4 * 16384MB DDR4 @ 2666MT/s
  Disk: Samsung SSD 860 EVO 500GB 

Original Version 1.22
---------------------

.. code::

  ➜ ./db_bench
  LevelDB:    version 1.22
  Date:       Sat Aug 29 17:10:41 2020
  CPU:        8 * Intel(R) Core(TM) i7-9700K CPU @ 3.60GHz
  CPUCache:   12288 KB
  Keys:       16 bytes each
  Values:     100 bytes each (50 bytes after compression)
  Entries:    1000000
  RawSize:    110.6 MB (estimated)
  FileSize:   62.9 MB (estimated)
  WARNING: Optimization is disabled: benchmarks unnecessarily slow
  WARNING: Assertions are enabled; benchmarks unnecessarily slow
  ------------------------------------------------
  fillseq      :       3.505 micros/op;   31.6 MB/s     
  fillsync     :    1681.854 micros/op;    0.1 MB/s (1000 ops)
  fillrandom   :       5.285 micros/op;   20.9 MB/s     
  overwrite    :       6.354 micros/op;   17.4 MB/s     
  readrandom   :       7.361 micros/op; (1000000 of 1000000 found)
  readrandom   :       6.304 micros/op; (1000000 of 1000000 found)
  readseq      :       0.470 micros/op;  235.2 MB/s    
  readreverse  :       0.934 micros/op;  118.5 MB/s    
  compact      :  759054.000 micros/op;
  readrandom   :       4.367 micros/op; (1000000 of 1000000 found)
  readseq      :       0.407 micros/op;  272.1 MB/s    
  readreverse  :       0.753 micros/op;  146.9 MB/s    
  fill100K     :     827.717 micros/op;  115.2 MB/s (1000 ops)
  crc32c       :       7.253 micros/op;  538.6 MB/s (4K per op)
  snappycomp   :       3.765 micros/op; 1037.6 MB/s (output: 55.1%)
  snappyuncomp :       0.541 micros/op; 7220.4 MB/s    


Add IntegerComparator (and set as default comparator as initializing Options object)
------------------------------------------------------------------------------------

.. code:

  ➜ ./db_bench 
  LevelDB:    version 1.22
  Date:       Sat Aug 29 18:20:56 2020
  CPU:        8 * Intel(R) Core(TM) i7-9700K CPU @ 3.60GHz
  CPUCache:   12288 KB
  Keys:       16 bytes each
  Values:     100 bytes each (50 bytes after compression)
  Entries:    1000000
  RawSize:    110.6 MB (estimated)
  FileSize:   62.9 MB (estimated)
  WARNING: Optimization is disabled: benchmarks unnecessarily slow
  WARNING: Assertions are enabled; benchmarks unnecessarily slow
  ------------------------------------------------
  fillseq      :       3.481 micros/op;   31.8 MB/s     
  fillsync     :    1665.587 micros/op;    0.1 MB/s (1000 ops)
  fillrandom   :       5.385 micros/op;   20.5 MB/s     
  overwrite    :       6.883 micros/op;   16.1 MB/s     
  readrandom   :       7.287 micros/op; (1000000 of 1000000 found)
  readrandom   :       6.425 micros/op; (1000000 of 1000000 found)
  readseq      :       0.510 micros/op;  217.1 MB/s    
  readreverse  :       0.904 micros/op;  122.4 MB/s    
  compact      :  765596.000 micros/op;
  readrandom   :       4.606 micros/op; (1000000 of 1000000 found)
  readseq      :       0.420 micros/op;  263.1 MB/s    
  readreverse  :       0.745 micros/op;  148.4 MB/s    
  fill100K     :     826.306 micros/op;  115.4 MB/s (1000 ops)
  crc32c       :       7.240 micros/op;  539.5 MB/s (4K per op)
  snappycomp   :       3.665 micros/op; 1065.7 MB/s (output: 55.1%)
  snappyuncomp :       0.489 micros/op; 7989.1 MB/s    
