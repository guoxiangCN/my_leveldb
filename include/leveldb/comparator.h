//
// Created by kuiper on 2021/2/7.
//

#ifndef MY_LEVELDB_COMPARATOR_H
#define MY_LEVELDB_COMPARATOR_H

#include "leveldb/export.h"
#include <string>

namespace leveldb {
    class Slice;

    class LEVELDB_EXPORT Comparator {
    public:

        /**
         * @brief 
         * @return 
        */
        virtual ~Comparator() = default;

        /**
         * @brief 
         * @param a 
         * @param b 
         * @return 
        */
        virtual int Compare(const Slice &a, const Slice &b) const = 0;

        /**
         * @brief 
         * @return 
        */
        virtual const char *Name() const = 0;

        /**
         * @brief 
         * @param start 
         * @param limit 
        */
        virtual void FindShortestSeparator(std::string *start, const Slice &limit) const = 0;

        /**
         * @brief 
         * @param key 
        */
        virtual void FindShortSuccessor(std::string *key) const = 0;
    };

    /**
     * @brief 
     * @return 
    */
    LEVELDB_EXPORT const Comparator *BytewiseComparator();
}

#endif //MY_LEVELDB_COMPARATOR_H
