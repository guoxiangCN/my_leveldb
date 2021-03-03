//
// Created by kuiper on 2021/2/12.
//

#ifndef MY_LEVELDB_WRITE_BATCH_H
#define MY_LEVELDB_WRITE_BATCH_H

#include "leveldb/export.h"
#include "leveldb/status.h"

namespace leveldb {

    class Slice;

    /**
     * @brief 代表一次批量写。
    */
    class LEVELDB_EXPORT WriteBatch {
    public:

        /**
         * @brief 
        */
        class LEVELDB_EXPORT Handler {
        public:
            virtual ~Handler();
            virtual void Put(const Slice &key, const Slice &val) = 0;
            virtual void Delete(const Slice &key) = 0;
        };

        WriteBatch();

        WriteBatch(const WriteBatch &) = default;
        WriteBatch &operator=(const WriteBatch &) = default;

        ~WriteBatch();

        /**
         * @brief 
         * @param key 
         * @param value 
        */
        void Put(const Slice &key, const Slice &value);

        /**
         * @brief 
         * @param key 
        */
        void Delete(const Slice &key);

        /**
         * @brief 
        */
        void Clear();

        /**
         * @brief 
         * @return 
        */
        size_t ApproximateSize() const;

        /**
         * @brief 
         * @param source 
        */
        void Append(const WriteBatch &source);

        /**
         * @brief 
         * @param handler 
         * @return 
        */
        Status Iterate(Handler *handler) const;

    private:
        friend class WriteBatchInternal;
        std::string rep_;
    };

}

#endif //MY_LEVELDB_WRITE_BATCH_H
