#!/bin/bash

# Build leveldb
cmake -Sleveldb -Bleveldb/build -G Ninja
cmake --build leveldb/build -- -j8

# Build demo
cmake -Sdemo -Bdemo/build -G Ninja
cmake --build demo/build -- -j8