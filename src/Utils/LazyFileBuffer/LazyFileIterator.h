#pragma once

#include "LazyFileBuffer.h"

#include <stdexcept>
#include <cstddef>

#define ERROR_MESSAGE_NOT_ENOUGH_BYTES "LazyFileIterator: can't read T: not enough bytes left"

class LazyFileIteratorBase {
public:
    template <typename T>
    friend class LazyFileIterator;

    using difference_type = ptrdiff_t;  // NOLINT

    LazyFileIteratorBase(LazyFileBuffer* file_buffer, size_t pos);

    LazyFileIteratorBase operator+(size_t offset) const;
    LazyFileIteratorBase operator-(size_t offset) const;

    LazyFileIteratorBase& operator++();
    LazyFileIteratorBase& operator--();
    LazyFileIteratorBase& operator+=(size_t offset);
    LazyFileIteratorBase& operator-=(size_t offset);

    bool operator==(const LazyFileIteratorBase& other) const;
    bool operator!=(const LazyFileIteratorBase& other) const;

    difference_type operator-(const LazyFileIteratorBase& other) const;

protected:
    LazyFileBuffer* const file_buffer_;
    const size_t file_size_;
    size_t element_index_;
};

template <typename T>
class LazyFileIterator : public LazyFileIteratorBase {
public:
    LazyFileIterator(LazyFileBuffer* file_buffer, size_t pos) : LazyFileIteratorBase(file_buffer, pos) {
    }

    LazyFileIterator(const LazyFileIteratorBase& base): LazyFileIteratorBase(base.file_buffer_, base.element_index_) {}

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