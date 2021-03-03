//
// Created by kuiper on 2021/2/17.
//

#ifndef MY_LEVELDB_LOG_FORMAT_H
#define MY_LEVELDB_LOG_FORMAT_H

namespace leveldb {

    namespace log {

        enum RecordType {

            /**
             * @brief 
            */
            kZeroType = 0,

            /**
             * @brief 
            */
            kFullType = 1,

            /**
             * @brief 
            */
            kFirstType = 2,
            kMiddleType = 3,
            kLastType = 4
        };

        /**
         * @brief 
        */
        static const int kMaxRecordType = kLastType;
        
        /**
         * @brief 每个WAL-LOG-BLOCK的字节大小 aka 32KB.
         */
        static const int kBlockSize = 32768;

        /**
         * @brief 每条WAL-LOG-RECORD的头大小
         * 
         * 4字节的checksum + 2字节的长度信息 + 1字节的type
        */
        static const int kHeaderSize = 4 + 2 + 1;

    }
}

#endif //MY_LEVELDB_LOG_FORMAT_H
