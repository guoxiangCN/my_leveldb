//
// Created by kuiper on 2021/2/7.
//

#ifndef MY_LEVELDB_ARENA_H
#define MY_LEVELDB_ARENA_H

#include <atomic>
#include <cassert>
#include <cstdint>
#include <vector>

namespace leveldb {

    /**
     * @brief  简单的内存分配器实现.
    */
    class Arena {
    public:
        Arena();

        Arena(const Arena &) = delete;
        Arena &operator=(const Arena &) = delete;

        ~Arena();

        /**
         * @brief 
         * @param bytes 
         * @return 
        */
        char *Allocate(size_t bytes);

        /**
         * @brief 
         * @param bytes 
         * @return 
        */
        char *AllocateAligned(size_t bytes);

        /**
         * @brief 
         * @return 
        */
        std::size_t MemoryUsage() const {
            return memory_usage_.load(std::memory_order_relaxed);
        }

    private:

        /**
         * @brief 
         * @param bytes 
         * @return 
        */
        char *AllocateFallback(size_t bytes);

        /**
         * @brief 
         * @param block_bytes 
         * @return 
        */
        char *AllocateNewBlock(size_t block_bytes);

        char *alloc_ptr_;
        size_t alloc_bytes_remaining_;
        std::vector<char *> blocks_;
        std::atomic<size_t> memory_usage_;
    };

    inline char *Arena::Allocate(size_t bytes) {
        assert(bytes > 0);
        if (bytes <= alloc_bytes_remaining_) {
            char *result = alloc_ptr_;
            alloc_ptr_ += bytes;
            alloc_bytes_remaining_ -= bytes;
            return result;
        }
        return AllocateFallback(bytes);
    }
}


#endif //MY_LEVELDB_ARENA_H
