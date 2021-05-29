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
#include "leveldb/env.h"
#include "db/skiplist.h"
#include "db/memtable.h"

#include "db/log_reader.h"
#include "db/log_writer.h"

leveldb::SequenceNumber seqGen() {
    static std::atomic_uint64_t seq = 0;
    return seq.fetch_add(1);
}

extern void testArena();

extern void testHistogram();

extern void testState();

extern void testAppendEscapedStringTo();

extern void testEnv();

extern void testSkipList();

extern void testLogWriter();

extern void testMemTable();

void testWindowsSequenceFile() {
    auto env = leveldb::Env::Default();
    leveldb::SequentialFile* sf;
    auto status = env->NewSequentialFile("G:/test_windows_env_sequential_file.txt", &sf);
    std::cout << status.ToString() << std::endl;

    char buffer[1024] = { 0 };
    leveldb::Slice slice;
    status = sf->Read(1024, &slice, buffer);
    if (status.IsOK()) {
        std::cout << std::string(buffer) << std::endl;
    }
}

int main() {
    testWindowsSequenceFile();
    //testMemTable();
    //testArena();
    //testHistogram();
    //testState();
    //testAppendEscapedStringTo();
    //testEnv();
    //testLogWriter();
    //testSkipList();

    return 0;
}

void testArena() {
    leveldb::Arena a;
    for (int i = 0; i < 100; i++) {
        a.Allocate(1);
    }
    std::cout << "total: " << a.MemoryUsage() << std::endl;
}

void testHistogram() {
    leveldb::Histogram histogram;

}

void testState() {
    auto s = leveldb::Status::OK();
    std::cout << s.ToString() << std::endl;
    std::cout << std::boolalpha;
    std::cout << s.IsOK() << std::endl;
    std::cout << s.IsCorruption() << std::endl;
    std::cout << s.IsInvalidArgument() << std::endl;
    std::cout << s.IsNotFound() << std::endl;
    std::cout << s.IsIOError() << std::endl;
}

void testAppendEscapedStringTo() {
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

void testEnv() {
    auto pEnv = leveldb::Env::Default();
    std::cout << pEnv->NowMicros() << std::endl;

    try {
        auto sx = pEnv->CreateDir("/data/posix_test");
    }
    catch (const std::exception& ex) {
        std::cout << ex.what() << std::endl;
    }
    // std::cout << sx.ToString() << std::endl;

    std::cout << std::boolalpha << pEnv->FileExists("/data/posix_test") << std::endl;

    uint64_t filesize;
    auto get_file_size_status = pEnv->GetFileSize("/data/posix_test", &filesize);
    if (get_file_size_status.IsOK()) {
        std::cout << "fileSize:" << filesize << std::endl;
    }

    auto rfState = pEnv->RemoveDir("/data/posix_test");
    std::cout << rfState.ToString() << std::endl;

    leveldb::RandomAccessFile *raf;
    auto rafState = pEnv->NewRandomAccessFile("/data/raf_test.out", &raf);
    if (rafState.IsOK()) {
        std::cout << raf << std::endl;
        leveldb::Slice s;
        auto readState = raf->Read(0, 3, &s, nullptr);
        if (readState.IsOK()) {
            std::cout << s.ToString() << std::endl;
        }
    }

    int a = 3;
//    pEnv->Schedule([](void *a) {
//        for (int i = 0; i < 100; ++i) {
//            std::cout << i * (*(int *) a) << std::endl;
//        }
//    }, &a);

    // pEnv->SleepForMicroseconds(10000000L);
    leveldb::WritableFile *writableFile = nullptr;
    // std::unique_ptr<leveldb::WritableFile> w
    auto wfState = pEnv->NewAppendableFile("/data/buf.dat", &writableFile);
    if (wfState.IsOK()) {
        writableFile->Append("helloworld\n");
        writableFile->Append("from guoxiang on 2020/02/16 20:34\n");
        writableFile->Append("guowei is wangzherongyao\n");
        writableFile->Append("guopeng is at old house");
        // writableFile->Flush();
        //writableFile->Sync();
        delete writableFile;
    }


}

void testSkipList() {
//    leveldb::Arena arena;
//    leveldb::SkipList<int, std::less<>> list(std::less<>(), &arena);
//    for (int i = 0; i < 1000; ++i) {
//        list.Insert(i);
//    }
//    std::cout << "done" << std::endl;
}

void testLogWriter() {
    auto env = leveldb::Env::Default();
    leveldb::WritableFile *wf = nullptr;
    auto s = env->NewAppendableFile("/data/leveld_log_test", &wf);
    if (s.IsOK()) {
        leveldb::log::Writer logWriter(wf);
        logWriter.AddRecord("cccccccccccccccccccccc");
    }
}

void testMemTable() {

    leveldb::InternalKeyComparator cmp(leveldb::BytewiseComparator());
    auto memtable = new leveldb::MemTable(cmp);
    memtable->Add(seqGen(), leveldb::kTypeValue, "aaaa", "bbbbxxxxxxxxxxxxxxxguoxiang");
    memtable->Add(seqGen(), leveldb::kTypeValue, "aaaa", "value2");
    memtable->Add(seqGen(), leveldb::kTypeValue, "aaaa", "value3");
    memtable->Add(seqGen(), leveldb::kTypeValue, "aaaa", "value_final");
    memtable->Add(seqGen(), leveldb::kTypeDeletion, "aaaa", "aaaa");
    memtable->Add(seqGen(), leveldb::kTypeValue, "aaaa", "hehedadadadadadada");
    // memtable->Add(8, leveldb::kTypeValue, "aaaa", "hehedadadadadadada");

    memtable->Add(seqGen(), leveldb::kTypeValue, "cccc", "dddd");

    std::cout << "MemoryUsage:" << memtable->ApproximateMemoryUsage() << std::endl;

    std::string value;
    leveldb::Status s;
    bool bRet = memtable->Get(leveldb::LookupKey("aaaa", 9999), &value, &s);
    std::cout << "bRet:" << std::boolalpha << bRet << std::endl;

    if (bRet) {
        if (s.IsOK()) {
            std::cout << "value is: " << value << std::endl;
        } else if (s.IsNotFound()) {
            std::cout << "value found  but mark delete tag." << std::endl;
        }
    }
}