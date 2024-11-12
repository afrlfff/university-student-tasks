#ifndef STRINGL_H
#define STRINGL_H

#include "Array.h"

/**
 * StringL (String relying on Length).
 * 
 * Brief:
 * - A string class that stores characters using a size variable, rather than relying on a null-terminator (\0).
 * - This allows for more efficient storage and manipulation of strings, especially for strings that contain null characters.
 * - This class completely ignores '\0' characters within the string unlike the std::string class.
 * 
 * Parameters:
 * - charType The type of the characters in the string (char, char16_t/wchar_t , char32_t).
 * 
 * Details:
 * - this class completely copies Array class so to get more inforamtion check Array.h
 * 
 */
template <typename charType>
class StringL
{
private:
    Array<charType> data_;
public:
    StringL(): data_(Array<charType>()) {}
    StringL(const size_t size): data_(Array<charType>(size)) {}
    StringL(const StringL<charType>& other): data_(Array<charType>(other.data_)) {}
    StringL(const size_t size, const charType c): data_(Array<charType>(size, c)) {}
    StringL(const charType* data, const size_t size): data_(Array<charType>(data, size)) {}
    StringL(std::initializer_list<charType> values) : data_(Array(values)) {}

    void push_back(const charType c) {
        data_.push_back(c);
    }

    void push_back(const StringL<charType>& other) {
        data_.push_back(other.data_);
    }

    void pop_back() {
        data_.pop_back();
    }

    template <typename valueType>
    void assign(const valueType& index, const charType& value) {
        data_.assign(index, value);
    }

    StringL<charType> copy() const {
        return StringL<charType>(data_.c_arr(), data_.size());
    }

    void resize(const size_t& size) {
        data_.resize(size);
    }

    inline const size_t size() const {
        return data_.size();
    }

    inline const size_t capacity() const {
        return data_.capacity();
    }

    inline const charType* c_str() const {
        return data_.c_arr();
    }

    void fit_to_size() {
        data_.fit_to_size();
    }

    StringL<charType> substr(size_t start, size_t count) const {
        if (start >= data_.size()) return StringL<charType>();
        if (start + count > data_.size()) count = data_.size() - start;

        StringL<charType> newString(count);
        for (size_t i = start; i < start + count; ++i) {
            newString.push_back(data_[i]);
        }
        return newString;
    }

    inline charType* begin() const {
        return data_.begin();
    }

    inline charType* end() const {
        return data_.end();
    }

    inline void clear() {
        data_.clear();
    }

    void free_memory() {
        data_.free_memory();
    }

    StringL<charType>& operator=(const StringL<charType>& other) {
        if (this != &other) {
            this->free_memory();
            data_ = other.data_;
        }
        return *this;
    }

    const StringL<charType> operator+(const StringL<charType>& other) const {
        StringL<charType> newString(data_.size() + other.size());
        for (size_t i = 0; i < data_.size(); ++i) {
            newString.push_back(data_[i]);
        }
        for (size_t i = 0; i < other.size(); ++i) {
            newString.push_back(other[i]);
        }
        return newString;
    }

    const StringL<charType> operator+(const charType other) const {
        StringL<charType> newString(data_.size() + 1);
        for (size_t i = 0; i < data_.size(); ++i) {
            newString.push_back(data_[i]);
        }
        newString.push_back(other);
        return newString;
    }

    const bool operator<(const StringL<charType>& other) const
    {
        size_t minSize = std::min(data_.size(), other.size());
        for (size_t i = 0; i < minSize; ++i) {
            if (data_[i] < other[i]) {
                return true;
            } else if (data_[i] > other[i]) {
                return false;
            }
        }
        return data_.size() < other.size();
    }

    const bool operator>(const StringL<charType>& other) const
    {
        size_t minSize = std::min(data_.size(), other.size());
        for (size_t i = 0; i < minSize; ++i) {
            if (data_[i] > other[i]) {
                return true;
            } else if (data_[i] < other[i]) {
                return false;
            }
        }
        return data_.size() > other.size();
    }

    bool operator==(StringL<charType>& other) const {
        return data_ == other.data_;
    }

    bool operator!=(const StringL<charType>& other) const {
        return data_ != other.data_;
    }

    inline const charType& operator[](const size_t index) const {
        return data_[index];
    }

    inline charType& operator[](const size_t index) {
        return data_[index];
    }

    ~StringL() {
        data_.free_memory();
    }

};

#endif