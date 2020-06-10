cmake_minimum_required(VERSION 3.10)
project(leveldb_demo VERSION 0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

include(CheckLibraryExists)
check_library_exists(crc32c crc32c_value "" HAVE_CRC32C)
check_library_exists(snappy snappy_compress "" HAVE_SNAPPY)
check_library_exists(tcmalloc malloc "" HAVE_TCMALLOC)

set(LEVELDB_PATH "${PROJECT_SOURCE_DIR}/../leveldb")
set(LIBLEVELDB "${LEVELDB_PATH}/build/libleveldb.a")

# Find libleveldb
# If ../leveldb/build/libleveldb.a exists, link it
# Else search for leveldb library in system path
if (EXISTS ${LIBLEVELDB})
  set(HEADER_LEVELDB ${LEVELDB_PATH}/include)
else (EXISTS ${LIBLEVELDB})
  find_path(HEADER_LEVELDB
    leveldb/db.h
    )
  unset(LIBLEVELDB)
  find_library(LIBLEVELDB
    leveldb
    )
endif ()

include_directories(${HEADER_LEVELDB})

add_executable(demo)
target_sources(demo PRIVATE
  demo.cpp)
target_link_libraries(demo ${LIBLEVELDB})
if(HAVE_CRC32C)
  target_link_libraries(demo crc32c)
endif(HAVE_CRC32C)
if(HAVE_SNAPPY)
  target_link_libraries(demo snappy)
endif(HAVE_SNAPPY)
if(HAVE_TCMALLOC)
  target_link_libraries(demo tcmalloc)
endif(HAVE_TCMALLOC)

# Needed by port_stdcxx.h
find_package(Threads REQUIRED)
target_link_libraries(demo Threads::Threads)

# Echo variables
message(STATUS "Leveldb path - ${LEVELDB_PATH}")
message(STATUS "Included leveldb headers directory - ${HEADER_LEVELDB}/leveldb")
message(STATUS "Linked leveldb library - ${LIBLEVELDB}")