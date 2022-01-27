cmake_minimum_required(VERSION 3.10)
project(leveldb_demo VERSION 0.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS True)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

include(CheckLibraryExists)
check_library_exists(crc32c crc32c_value "" HAVE_CRC32C)
check_library_exists(snappy snappy_compress "" HAVE_SNAPPY)
check_library_exists(tcmalloc malloc "" HAVE_TCMALLOC)

set(LEVELDB_PATH "${PROJECT_SOURCE_DIR}/../leveldb")
message(STATUS "LevelDB Path: ${LEVELDB_PATH}")
#find_library(LIBLEVELDB NAMES libleveldb.a
#             PATHS ${LEVELDB_PATH}/build NO_DEFAULT_PATH)
set(LIBLEVELDB "${LEVELDB_PATH}/build/libleveldb.a")

#message(STATUS "LevelDB library path after first try: ${LIBLEVELDB}")

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

include_directories(
  ${HEADER_LEVELDB}
  "."
)

function(leveldb_demo demo_file)
  get_filename_component(demo_file_name "${demo_file}" NAME_WE)

  add_executable("${demo_file_name}")
  target_sources("${demo_file_name}"
    PRIVATE
    "${demo_file}"
  )

  target_link_libraries("${demo_file_name}" ${LIBLEVELDB})
  if(HAVE_CRC32C)
    target_link_libraries("${demo_file_name}" crc32c)
  endif(HAVE_CRC32C)
  if(HAVE_SNAPPY)
    target_link_libraries("${demo_file_name}" snappy)
  endif(HAVE_SNAPPY)
  if(HAVE_TCMALLOC)
    target_link_libraries("${demo_file_name}" tcmalloc)
  endif(HAVE_TCMALLOC)

  # Needed by port_stdcxx.h
  find_package(Threads REQUIRED)
  target_link_libraries("${demo_file_name}" Threads::Threads)
  
endfunction(leveldb_demo)

leveldb_demo("demo.cc")
leveldb_demo("mv_demo.cc")

# add_executable(demo)
# target_sources(demo PRIVATE
#   demo.cc)
# target_link_libraries(demo ${LIBLEVELDB})
# if(HAVE_CRC32C)
#   target_link_libraries(demo crc32c)
# endif(HAVE_CRC32C)
# if(HAVE_SNAPPY)
#   target_link_libraries(demo snappy)
# endif(HAVE_SNAPPY)
# if(HAVE_TCMALLOC)
#   target_link_libraries(demo tcmalloc)
# endif(HAVE_TCMALLOC)

# # Needed by port_stdcxx.h
# find_package(Threads REQUIRED)
# target_link_libraries(demo Threads::Threads)

# Coding
# add_executable(coding_demo)
# target_sources(coding_demo PRIVATE
#   coding_demo.cc)

# Echo variables
message(STATUS "Leveldb path - ${LEVELDB_PATH}")
message(STATUS "Included leveldb headers directory - ${HEADER_LEVELDB}/leveldb")
message(STATUS "Linked leveldb library - ${LIBLEVELDB}")

add_executable(time_t time_t.cc)
add_executable(coding_demo
               coding_demo.cc
               util/coding.h)