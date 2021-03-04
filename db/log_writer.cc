//
// Created by kuiper on 2021/2/18.
//

#include "leveldb/env.h"
#include "db/log_writer.h"
#include "port/port.h"
#include "util/coding.h"
#include "util/crc32c.h"

namespace leveldb {
namespace log {

        /**
         * @brief
         * @param type_crc
        */
        static void InitTypeCrc(uint32_t* type_crc) 
        {
            for (int i = 0; i <= kMaxRecordType; i++) {
                char t = static_cast<char>(i);
                type_crc[i] = crc32c::Value(&t, 1);
            }
        }

        Writer::Writer(WritableFile* dest) : dest_(dest), block_offset_(0)
        {
            InitTypeCrc(type_crc_);
        }  

        Writer::Writer(WritableFile* dest, uint64_t dest_length)
            : dest_(dest), block_offset_(dest_length% kBlockSize)
        {
            InitTypeCrc(type_crc_);
        }

        Status Writer::AddRecord(const Slice& slice)
        {
           size_t size = slice.size();

        }

        /**
         * ||                        header                              ||payload||
         * ||crc32_1||crc32_2||crc32_3||crc32_4||length_1||length_2||type||payload||
         * @brief 
         * @param type 
         * @param ptr 
         * @param length 
         * @return 
        */
        Status Writer::EmitPhysicalRecord(RecordType type, const char* ptr, size_t length) {

            // 每个物理块的长度用2字节存储，所以只能最大0xffff
            assert(length <= 0xffff);
            
            // 确保当前block可以写下当前物理块
            assert(static_cast<size_t>(block_offset_) + static_cast<size_t>(kHeaderSize) + length < kBlockSize);

            char buf[kHeaderSize];
            buf[4] = static_cast<char>(length & 0xff);
            buf[5] = static_cast<char>(length >> 8ull);
            buf[6] = static_cast<char>(type);

            // 计算并写入CRC32C
            uint32_t crc = crc32c::Extend(type_crc_[type], ptr, length);
            crc = crc32c::Mask(crc);
            EncodeFixed32(buf, crc);

            // 写入物理块
            Status s = dest_->Append(Slice(buf, kHeaderSize));
            if (LIKELY(s.IsOK())) {
                s = dest_->Append(Slice(ptr, length));
                if (LIKELY(s.IsOK())) {
                    s = dest_->Flush();
                }
            }

            // 无论写入是否成功，offset都跳过这个区域.
            block_offset_ += static_cast<int>((kHeaderSize + length));
            return s;
        }


} // end of namespace log  
} // end of namespace leveldb