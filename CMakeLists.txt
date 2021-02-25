cmake_minimum_required(VERSION 3.17)
project(my_leveldb VERSION 1.23.0 LANGUAGES C CXX)

# set(CMAKE_BUILD_TYPE Debug)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

if(WIN32)
    set(LEVELDB_PLATFORM_NAME LEVELDB_PLATFORM_WINDOWS)
else (WIN32)
    set(LEVELDB_PLATFORM_NAME LEVELDB_PLATFORM_POSIX)
endif(WIN32)


INCLUDE_DIRECTORIES(./include)
INCLUDE_DIRECTORIES(./)

set(CCFILES main.cpp
        util/cache.cc
        util/coding.cc
        util/arena.cc
        util/histogram.cc
        util/options.cc
        util/comparator.cc
        util/status.cc
        util/logging.cc
        util/env.cc
        util/crc32c.cc
        table/iterator.cc
        db/filename.cc
        db/log_writer.cc
        db/log_reader.cc
        db/memtable.cc
        db/dbformat.cc
        db/write_batch.cc
        #db/dumpfile.cc
        )

        if(WIN32)
            set(CCFILES ${CCFILES} util/env_windows.cc)
        else()
            set(CCFILES ${CCFILES} util/env_posix.cc)
        endif()

message(STATUS "${CCFILES}")

add_executable(my_leveldb ${CCFILES})