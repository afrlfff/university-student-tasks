#pragma once

#include <cstdint>
#include <stdexcept>

#include "Array.h"
#include "StringL.h"
#include "BinaryUtils.h"


/**
 * BitArray.
 * 
 * Brief:
 * - Class defines array of bits where 1 bit takes 1 bit of memory. It achieved by storing each 8 bits as uint8_t value
 * 
 * Memory usage:
 * O(1) + θ(N),     where N - number of bits
 * 
 * Details:
 * - use class if use care about memory usage, not for speed optimization.
 * - if use care about speed it's more effectively to store bit within a string
 */
class BitArray
{
private:
    Array<uint8_t> data_;
    size_t size_;
public:
    BitArray(): data_(Array<uint8_t>()), size_(0) {}
    BitArray(const size_t size): data_(Array<uint8_t>()), size_(0)
    {
        resize(size);
    }
    BitArray (const BitArray& other) : data_(other.array()), size_(other.size()) { }

    inline const Array<uint8_t>& array() const { return data_; }

    inline size_t size() const
    {
        return size_;
    }

    void resize(const size_t size)
    {
        if (size % 8 == 0) {
            data_.resize(size / 8);
        } else {
            data_.resize(size / 8 + 1);
        }   
    }

    BitArray copy()
    {
        return BitArray(*this);
    }

    void push_back(const char& value)
    {
        if (value != '0' && value != '1') {
            throw std::invalid_argument("Invalid value for BitArray!");
        }

        if (size_ % 8 == 0) data_.push_back(0);
        
        if (value == '0') {
            // * 2
            data_[data_.size() - 1] <<= 1;
        } else {
            // * 2 + 1
            data_[data_.size() - 1] <<= 1;
            data_[data_.size() - 1] |= 1;
        }
        ++size_;
    }

    const char get_bit(const size_t index) const
    {
        if (index > size_) {
            throw std::invalid_argument("BitArray: index is out of range!");
        }

        uint8_t num = 0;
        if (index / 8 == (data_.size() - 1) && (size_ % 8 != 0)) {
            num = ((data_[index / 8] << (8 - size_ % 8)) << index % 8) & 0b10000000;
        } else {
            num = (data_[index / 8] << index % 8) & 0b10000000;
        }

        return (num == 0) ? '0' : '1';
    }

    const StringL<char> to_string() const
    {
        StringL<char> str(size_);

        if (size_ % 8 == 0) {
            for (const uint8_t& block : data_) {
                str.push_back(BinaryUtils::GetBinaryStringFromNumber(block, 8));
            }
        } else {
            size_t i = 0;
            for (i = 0; i < data_.size() - 1; ++i) {
                str.push_back(BinaryUtils::GetBinaryStringFromNumber(data_[i], 8));
            }
            str.push_back(BinaryUtils::GetBinaryStringFromNumber(data_[i], size_ % 8));
        }
        return str;
    }

    static void to_file(std::ofstream& f, const BitArray& bitArray)
    {
        if (bitArray.size_ == 0) return;

        // write bit array to file
        if (bitArray.size_ % 8 == 0) {
            for (uint8_t& block : bitArray.data_)
                f.write(reinterpret_cast<const char*>(&block), 1);
        } else {
            size_t i = 0;
            for (i = 0; i < bitArray.data_.size() - 1; ++i)
                f.write(reinterpret_cast<const char*>(&(bitArray.data_[i])), 1);
            // write last block specifically to avoid storing its size while decoding
            uint8_t lastBlock = static_cast<uint8_t>(bitArray.data_[i] << (8 - bitArray.size_ % 8));
            f.write(reinterpret_cast<char*>(&lastBlock), 1);
        }
    }

    static const BitArray from_file(std::ifstream& f, const size_t numOfBits)
    {
        if (numOfBits % 8 != 0) {
            throw std::invalid_argument("BitArray::from_file(): Number of bits should be a multiple of 8!");
        }
            
        BitArray bitArr(numOfBits);

        uint8_t currentBlock;
        while (bitArr.size_ < numOfBits) {
            f.read(reinterpret_cast<char*>(&currentBlock), 1);
            bitArr.data_.push_back(currentBlock);
            bitArr.size_ += 8;
        }

        return bitArr;
    }

    BitArray& operator=(const BitArray& other) {
        size_ = other.size();
        data_ = Array<uint8_t>(size_);
        for (const uint8_t& block : other.array()) {
            data_.push_back(block);
        }
        return *this;
    }

    // DOESN'T WORK CORRECT
    /* const BitArray operator+(const BitArray& other) const {
        BitArray newBitArray(size_ + other.size_);
        newBitArray.size_ = size_ + other.size_;

        for (const uint8_t& block : data_) {
            newBitArray.data_.push_back(block);
        }

        if (size_ % 8 == 0) {   
            for (const uint8_t& block : other.data_) {
                newBitArray.data_.push_back(block);
            }
        } else if (other.size_ > 0) {
            int arraySize = ((size_ + other.size_) % 8 == 0) ? 
                            ((size_ + other.size_) / 8) : 
                            ((size_ + other.size_) / 8 + 1); 
            int r = size_ % 8;
            
            size_t i = 0;
            while (newBitArray.data_.size() < arraySize) {
                newBitArray.data_[data_.size() - 1ll + i] <<= (8 - r);
                newBitArray.data_[data_.size() - 1ll + i] +=
                    (other.data_[i] >> r);
                newBitArray.data_.push_back(other.data_[i++]);
            }
            if (i < other.data_.size()) {
                newBitArray.data_[newBitArray.data_.size() - 1] <<= (8 - r);
                newBitArray.data_[newBitArray.data_.size() - 1] += other.data_[other.data_.size() - 1];
            }

            //newBitArray.data_[newBitArray.data_.size() - 1] >>= (8 - r);
        }
        return newBitArray;
    } */

    const BitArray operator+(const char bit) const
    {
        BitArray newBitArray(size_ + 1);

        for (const uint8_t& block : data_) {
            newBitArray.data_.push_back(block);
        }
        newBitArray.size_ = size_;

        newBitArray.push_back(bit);
        return newBitArray;
    }

    inline void free_memory() {
        data_.free_memory();
        size_ = 0;
    }

    inline void clear() {
        size_ = 0;
        data_.clear();
    }
};