#ifndef ARRAY_H
#define ARRAY_H

/**
 * Array.
 * 
 * Brief:
 * - Class defines dynamic array of objects of type T
 * 
 * Parameters:
 * - T - The type of the characters in the string (char, char16_t/wchar_t , char32_t).
 * 
 * Memory usage:
 * sizeof(T) * size
 * 
 * Details:
 * - clear() just sets size to 0 (ensures correct work like there is an empty array but the memory isn't freed)
 * - freeMemory() is the only function which deletes the array (except of destructor)
 * - to add a new element use only push_back() method.
 * - you can use operator[] only for reading elements.
 * - ! For optimization purposes, this class does not perform bounds checking on the operator[] and assign() functions. Accessing an index outside the array's bounds or attempting to access an empty element may result in undefined behavior. Use with caution !
 * - To preallocate memory, use constructor or resize() method.
 * - If you worry about optimization so always use resize() or corresponding contructor to preallocate memory for the entire array
 * 
 */
template <typename T>
class Array
{
private:
    T* data_;
    size_t size_;
    size_t capacity_;
public:
    Array(): data_(nullptr), size_(0), capacity_(0) {}
    Array(const size_t size): data_(new T[size]), size_(0), capacity_(size) {
        if (size == 0) data_ = nullptr;
    }
    Array(const Array<T>& other): data_(new T[other.size()]), size_(other.size()), capacity_(other.size()) {
        if (other.c_arr() == nullptr || other.size() == 0) {
            if (data_ != nullptr) delete[] data_;
            data_ = nullptr;
            size_ = 0; capacity_ = 0;
        } else {
            for (size_t i = 0; i < other.size(); ++i) {
                data_[i] = other[i];
            }
        }
    }
    Array(const T* data, const size_t size): data_(new T[size]), size_(size), capacity_(size) {
        for (size_t i = 0; i < size; ++i) {
            data_[i] = data[i];
        }
    }
    Array(std::initializer_list<T> values) : data_(new T[values.size()]), size_(values.size()), capacity_(values.size()) {
        std::copy(values.begin(), values.end(), data_);
    }

    Array(const size_t size, const T value): data_(new T[size]), size_(size), capacity_(size) {
        for (size_t i = 0; i < size; ++i) {
            data_[i] = value;
        }
        if (size == 0) data_ = nullptr;
    }

    void resize(const size_t size) {
        if (data_ != nullptr) {
            if (size == 0) {
                delete[] data_;
                data_ = nullptr;
            } else {
                T* newdata_ = new T[size];
                for (size_t i = 0; i < std::min(size, size_); ++i)
                    newdata_[i] = data_[i];
                
                delete[] data_;
                data_ = newdata_;
            }
        } else {
            data_ = new T[size];
        }
        size_ = std::min(size_, size);
        capacity_ = size;
    }

    void push_back(T value) {
        if (size_ < capacity_) {
            data_[size_++] = value;
        } else {
            // if size_ == capacity_

            // calculate next degree of 2 which is greater than capacity_
            int degree = 0;
            while (capacity_ > 1) {
                capacity_ /= 2;
                ++degree;
            }
            ++degree;

            // get new capacity
            capacity_ = 1;
            while (degree-- > 0) capacity_ *= 2;

            // reallocate memory
            T* newdata_ = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                newdata_[i] = data_[i];
            }
            newdata_[size_++] = value;
            delete[] data_;
            data_ = newdata_;
        }
    }

    void push_back(const Array<T>& other) {
        if (size_ + other.size() <= capacity_) {
            for (const T& value : other) {
                push_back(value);
            }
        } else {
            resize(size_ + other.size());
            for (const T& value : other) {
                push_back(value);
            }
        }
    }

    void pop_back() {
        if (size_ > 0) --size_;
    }

    template <typename valueType>
    void assign(const valueType index, const T value) {
        data_[index] = value;
    }

    Array<T> copy() const {
        return Array<T>(data_, size_);
    }

    void fit_to_size() {
        if (size_ < capacity_) {
            T* newdata_ = new T[size_];
            for (size_t i = 0; i < size_; ++i) {
                newdata_[i] = data_[i];
            }
            delete[] data_;
            data_ = newdata_;
            capacity_ = size_;
        }
    }

    Array<T> subarr(size_t start, size_t count) const {
        if (start >= size_) return Array<T>();
        if (start + count > size_) count = size_ - start;

        Array<T> newArray(count);
        for (size_t i = start; i < start + count; ++i) {
            newArray.push_back(data_[i]);
        }
        return newArray;
    }

    inline const size_t size() const {
        return size_;
    }

    inline const size_t capacity() const {
        return capacity_;
    }

    inline const T* c_arr() const {
        return data_;
    }

    const Array<T> operator+(const Array<T>& other) const {
        Array<T> newArray(size_ + other.size());
        for (size_t i = 0; i < size_; ++i) {
            newArray.push_back(data_[i]);
        }
        for (size_t i = 0; i < other.size(); ++i) {
            newArray.push_back(other[i]);
        }
        return newArray;
    }

    Array<T>& operator=(const Array<T>& other) {
        if (this != &other) {
            if (data_ != nullptr) delete[] data_;
            data_ = new T[other.size()];
            size_ = other.size();
            capacity_ = other.size();
            for (size_t i = 0; i < other.size(); ++i) {
                data_[i] = other[i];
            }
        }
        return *this;
    }

    bool operator==(const Array<T>& other) const {
        if (size_ != other.size()) return false;
        for (size_t i = 0; i < size_; ++i) {
            if (data_[i] != other[i]) return false;
        }
        return true;
    }

    bool operator!=(const Array<T>& other) const {
        return !(*this == other);
    }

    inline const T& operator[](const size_t index) const {
        return data_[index];
    }

    inline T& operator[](const size_t index) {
        return data_[index];
    }

    inline T* begin() const {
        return data_;
    }

    inline T* end() const {
        return data_ + size_;
    }

    inline void clear() {
        size_ = 0;
    }

    inline void free_memory() {
        if (data_ != nullptr) {
            delete[] data_;
            data_ = nullptr;
        }
        size_ = 0;
        capacity_ = 0;
    }

    ~Array() {
        if (data_ != nullptr) {
            delete[] data_;
        }    
    }
};

#endif