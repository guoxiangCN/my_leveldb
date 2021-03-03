//
// Created by kuiper on 2021/2/19.
//

#ifndef MY_LEVELDB_WRITE_BATCH_INTERNAL_H
#define MY_LEVELDB_WRITE_BATCH_INTERNAL_H

#include "db/dbformat.h"
#include "leveldb/write_batch.h"

namespace leveldb {

    class MemTable;

    /*
     * @brief WriteBatchInternal提供了一些辅助函数给到WriteBatch使用
     * 这些辅助函数我们不希望通过WriteBatch提供给到用户.
     */
    class WriteBatchInternal {
    public:

        /**
         * @brief 返回batch中的条目数量.
         * @param batch 
         * @return 
        */
        static int Count(const WriteBatch *batch);

        /**
         * @brief 将batch中的条目数量设置为n.
         * @param batch 
         * @param n 
        */
        static void SetCount(WriteBatch *batch, int n);

        /**
         * @brief 获取当前WriteBatch的起始条目的序列号.
         * @param batch 
         * @return 
        */
        static SequenceNumber Sequence(const WriteBatch *batch);

        /**
         * @brief 设置当前WriteBatch的起始条目的序列号.
         * @param batch 
         * @param seq 
        */
        static void SetSequence(WriteBatch *batch, SequenceNumber seq);

        /**
         * @brief 
         * @param batch 
         * @return 
        */
        static Slice Contents(const WriteBatch *batch) { return Slice(batch->rep_); }

        /**
         * @brief 
         * @param batch 
         * @param contents 
        */
        static void SetContents(WriteBatch *batch, const Slice &contents);

        /**
         * @brief 
         * @param batch 
         * @return 
        */
        static size_t ByteSize(const WriteBatch *batch) { return batch->rep_.size(); }

        /**
         * @brief 
         * @param batch 
         * @param memTable 
         * @return 
        */
        static Status InsertInto(const WriteBatch *batch, MemTable *memTable);

        /**
         * @brief 
         * @param dst 
         * @param src 
        */
        static void Append(WriteBatch *dst, const WriteBatch *src);

    };

}

#endif //MY_LEVELDB_WRITE_BATCH_INTERNAL_H
