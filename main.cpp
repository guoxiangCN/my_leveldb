#include <iostream>
#include "leveldb/export.h"
#include "leveldb/slice.h"
#include "leveldb/cxx.h"
#include "leveldb/cache.h"
#include "leveldb/db.h"
#include "port/port_stdcxx.h"
#include "util/arena.h"
#include "util/histogram.h"
#include "leveldb/options.h"
#include "util/coding.h"
#include "leveldb/status.h"
#include "util/mutexlock.h"
#include "util/logging.h"

extern void testArena();
extern void testHistogram();
extern void testState();
extern void testAppendEscapedStringTo();

int main() {
    std::cout << "Hello, World!" << std::endl;

    leveldb::Slice s = "aaaaaa";
    std::cout << s.size() << std::endl;

    leveldb::Slice sb = "aaaaaaaaaa";

    std::cout << std::boolalpha << s.starts_with(sb) << std::endl;

    //leveldb::port::Mutex mutex;
    // mutex.Lock();
    // leveldb::port::CondVar condVar(&mutex);
    // condVar.Wait();

    leveldb::Options options;

    leveldb::ReadOptions readOptions;
    readOptions.verify_checksums = true;

    leveldb::WriteOptions writeOptions;
    writeOptions.sync = true;

    testArena();
    testHistogram();
    testState();
    testAppendEscapedStringTo();


    return 0;
}

 void testArena()
{
    leveldb::Arena a;
    for(int i = 0; i <100;i++) {
        a.Allocate(1);
    }
    std::cout << "total: " << a.MemoryUsage() << std::endl;
}

void testHistogram(){
    leveldb::Histogram histogram;

}

void testState()
{
    auto s = leveldb::Status::OK();
    std::cout << s.ToString() << std::endl;
    std::cout << std::boolalpha;
    std::cout << s.IsOK() << std::endl;
    std::cout << s.IsCorruption() << std::endl;
    std::cout << s.IsInvalidArgument() << std::endl;
    std::cout << s.IsNotFound() << std::endl;
    std::cout << s.IsIOError() << std::endl;
}

void testAppendEscapedStringTo()
{
    std::string res;
    leveldb::AppendEscapedStringTo(&res, "aa\x6zzzz");
    std::cout << res << std::endl;
    std::cout << "aa\x6zzzz" << std::endl;

    std::string numstr;
    leveldb::AppendNumberTo(&numstr, 12233333);
    std::cout << numstr << std::endl;

    auto x = leveldb::NumberToString(18888888L);
    std::cout << x << std::endl;

    std::string a = "\x8";
    auto a1 = leveldb::EscapeString(a);
    std::cout << a1 << std::endl;
}
