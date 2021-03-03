//
// Created by kuiper on 2021/2/18.
//

#ifndef MY_LEVELDB_LOG_WRITER_H
#define MY_LEVELDB_LOG_WRITER_H

#include <cstdint>

#include "db/log_format.h"
#include "leveldb/slice.h"
#include "leveldb/status.h"

namespace leveldb {

    class WritableFile;

    namespace log {

        /**
         * @brief leveldb的WAL-LOG写入器.
        */
        class Writer {
        public:

            /**
             * @brief 创建一个WAL-LOG写入器.
             * @param dest 目标文件，必须是初始化空的文件.
             * @return 
            */
            explicit Writer(WritableFile *dest);

            /**
             * @brief 创建一个WAL-LOG写入器.
             * @param dest 目标文件.
             * @param dest_length 目标长度.dest的初始化长度必须为dest_length.
             * @return 
            */
            Writer(WritableFile *dest, uint64_t dest_length);

            /**
             * @brief 不可以拷贝构造.
             * @param  
             * @return 
            */
            Writer(const Writer&) = delete;

            /**
             * @brief 不可以被赋值.
             * @param  
             * @return 
            */
            Writer& operator=(const Writer&) = delete;

            /**/
            ~Writer() = default;

            /**
             * @brief 追加一条WAL-LOG.
             * @param slice WAL-LOG.
             * @return 追加的结果.
            */
            Status AddRecord(const Slice& slice);

        private:

            /**
             * @brief 实际写入物理块的函数实现.
             * @param type 
             * @param ptr 
             * @param length 
             * @return 
            */
            Status EmitPhysicalRecord(RecordType type, const char* ptr, size_t length);

            WritableFile* dest_;                        // 当前使用的WAL-LOG文件.
            int block_offset_;                          // 在当前块内的偏移.
            uint32_t type_crc_[kMaxRecordType + 1]{};   // CRC类型校验码.每个RECORD_TYPE4个字节.
        };


    }

}


#endif //MY_LEVELDB_LOG_WRITER_H
