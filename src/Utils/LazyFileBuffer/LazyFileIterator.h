#pragma once

#include "LazyFileBuffer.h"

#include <stdexcept>

#define ERROR_MESSAGE_NOT_ENOUGH_BYTES "LazyFileIterator: can't read T: not enough bytes left"

class LazyFileIteratorBase {
public:
    LazyFileIteratorBase(LazyFileBuffer* file_buffer);

    LazyFileIteratorBase operator+(size_t offset) const;
    LazyFileIteratorBase operator-(size_t offset) const;

    LazyFileIteratorBase& operator++();
    LazyFileIteratorBase& operator--();
    LazyFileIteratorBase& operator+=(size_t offset);
    LazyFileIteratorBase& operator-=(size_t offset);

protected:
    LazyFileBuffer* const file_buffer_;
    const size_t file_size_;
    size_t element_index_;
};

template <typename T>
class LazyFileIterator : public LazyFileIteratorBase {
public:
    T operator*() {
        if (!CanReadOneMore()) {
            throw std::runtime_error(ERROR_MESSAGE_NOT_ENOUGH_BYTES);
        }
        return file_buffer_->GetAs<T>(element_index_ * sizeof(T));
    }

    operator bool() const {
        return CanReadOneMore();
    }

private:
    bool CanReadOneMore() const {
        return (element_index_ + 1) * sizeof(T) <= file_size_;
    }
};