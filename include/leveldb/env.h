//
// Created by kuiper on 2021/2/8.
//

#ifndef MY_LEVELDB_ENV_H
#define MY_LEVELDB_ENV_H

#include <cstdarg>
#include <cstdint>
#include <string>
#include <vector>

#include "leveldb/export.h"
#include "leveldb/status.h"

#if defined(_WIN32)
#if defined(DeleteFile)

#undef DeleteFile
#define LEVELDB_DELETEFILE_UNDEFINED

#endif  // DeleteFile
#endif  // _WIN32

namespace leveldb {

    class FileLock;
    class Logger;
    class RandomAccessFile;
    class SequentialFile;
    class Slice;
    class WritableFile;

    /**
     * @brief 
    */
    class LEVELDB_EXPORT Env {
    public:
        Env();

        Env(const Env &) = delete;
        Env &operator=(const Env &) = delete;

        virtual ~Env();

        /**
         * @brief ��ȡĬ�ϵ�Envʵ������
         * @return ���ص�ʵ��ȫ�ֵ���Ψһ
        */
        static Env *Default();

        /**
         * @brief ��һ��֧��˳������Ѿ����ڵ��ļ�.
         * @param fname �ļ���
         * @param result �������
         * @return ����򿪳ɹ�������Status::OK()����*result������Ϊ��ȷ��ֵ.
        */
        virtual Status NewSequentialFile(const std::string &fname, SequentialFile **result) = 0;

        /**
         * @brief 
         * @param fname 
         * @param result 
         * @return 
        */
        virtual Status NewRandomAccessFile(const std::string &fname, RandomAccessFile **result) = 0;

        /**
         * @brief 
         * O_TRUNC
         * @param fname 
         * @param result 
         * @return 
        */
        virtual Status NewWritableFile(const std::string &fname, WritableFile **result) = 0;

        /**
         * @brief 
        * O_APPEND
         * @param fname 
         * @param result 
         * @return 
        */
        virtual Status NewAppendableFile(const std::string &fname, WritableFile **result) = 0;

        /**
         * @brief 
         * @param fname 
         * @return 
        */
        virtual bool FileExists(const std::string &fname) = 0;

        /**
         * @brief 
         * @param dir 
         * @param result 
         * @return 
        */
        virtual Status GetChildren(const std::string &dir, std::vector<std::string> *result) = 0;

        /**
         * @brief 
         * @param fname 
         * @return 
        */
        virtual Status RemoveFile(const std::string &fname);

        /**
         * @brief 
         * @param fname 
         * @return 
        */
        virtual Status DeleteFile(const std::string &fname);

        /**
         * @brief 
         * @param dirname 
         * @return 
        */
        virtual Status CreateDir(const std::string &dirname) = 0;

        /**
         * @brief 
         * @param dirname 
         * @return 
        */
        virtual Status RemoveDir(const std::string &dirname);

        /**
         * @brief 
         * @param dirname 
         * @return 
        */
        virtual Status DeleteDir(const std::string &dirname);

        /**
         * @brief 
         * @param fname 
         * @param file_size 
         * @return 
        */
        virtual Status GetFileSize(const std::string &fname, uint64_t *file_size) = 0;

        /**
         * @brief 
         * @param src 
         * @param target 
         * @return 
        */
        virtual Status RenameFile(const std::string &src, const std::string &target) = 0;

        /**
         * @brief 
         * @param fname 
         * @param lock 
         * @return 
        */
        virtual Status LockFile(const std::string &fname, FileLock **lock) = 0;

        /**
         * @brief 
         * @param lock 
         * @return 
        */
        virtual Status UnlockFile(FileLock *lock) = 0;

        /**
         * @brief 
         * @param func 
         * @param arg 
        */
        virtual void Schedule(void (*func)(void *arg), void *arg) = 0;

        /**
         * @brief 
         * @param func 
         * @param arg 
        */
        virtual void StartThread(void (*func)(void *arg), void *arg) = 0;

        /**
         * @brief 
         * @param path 
         * @return 
        */
        virtual Status GetTestDirectory(std::string *path) = 0;

        /**
         * @brief 
         * @param fname 
         * @param result 
         * @return 
        */
        virtual Status NewLogger(const std::string &fname, Logger **result) = 0;

        /**
         * @brief ��ȡ��ǰ��΢����.
         * @return 
        */
        virtual uint64_t NowMicros() = 0;

        /**
         * @brief 
         * @param micros 
        */
        virtual void SleepForMicroseconds(int micros) = 0;
    };

    /**
     * @brief 
    */
    class LEVELDB_EXPORT SequentialFile {
    public:
        SequentialFile() = default;

        virtual ~SequentialFile() = default;

        SequentialFile(const SequentialFile &) = delete;
        SequentialFile &operator=(const SequentialFile &) = delete;

        virtual Status Read(size_t n, Slice *result, char *scratch) = 0;

        virtual Status Skip(uint64_t n) = 0;
    };

    /**
     * @brief 
    */
    class LEVELDB_EXPORT RandomAccessFile {
    public:
        virtual ~RandomAccessFile() = default;

        virtual Status Read(uint64_t offset, size_t n, Slice *result, char *scratch) const = 0;
    };

    /**
     * @brief 
    */
    class LEVELDB_EXPORT WritableFile {
    public:
        WritableFile() = default;

        WritableFile(const WritableFile &) = delete;

        WritableFile &operator=(const WritableFile &) = delete;

        virtual ~WritableFile() = default;

        virtual Status Append(const Slice &data) = 0;

        virtual Status Close() = 0;

        virtual Status Flush() = 0;

        virtual Status Sync() = 0;
    };

    /**
     * @brief 
    */
    class LEVELDB_EXPORT Logger {
    public:
        Logger() = default;

        Logger(const Logger &) = delete;

        Logger &operator=(const Logger &) = delete;

        virtual ~Logger() = default;

        virtual void Logv(const char *fmt, std::va_list ap) = 0;
    };

    /**
     * @brief 
    */
    class LEVELDB_EXPORT FileLock {
    public:
        FileLock() = default;

        FileLock(const FileLock &) = delete;

        FileLock &operator=(const FileLock &) = delete;

        virtual ~FileLock() = default;
    };

}


#if defined(_WIN32) && defined(LEVELDB_DELETEFILE_UNDEFINED)
#if defined(UNICODE)
#define DeleteFile DeleteFileW
#else
#define DeleteFile DeleteFileA
#endif  // defined(UNICODE)
#endif  // defined(_WIN32) && defined(LEVELDB_DELETEFILE_UNDEFINED)

#endif //MY_LEVELDB_ENV_H
