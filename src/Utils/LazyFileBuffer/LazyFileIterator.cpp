#include "LazyFileIterator.h"
#include "LazyFileBuffer/LazyFileBuffer.h"

LazyFileIteratorBase::LazyFileIteratorBase(LazyFileBuffer* file_buffer, size_t pos)
    : file_buffer_(file_buffer), file_size_(file_buffer->GetFileSize()), element_index_(pos) {
}

LazyFileIteratorBase LazyFileIteratorBase::operator+(size_t offset) const {
    LazyFileIteratorBase output = *this;
    output.element_index_ += offset;
    return output;
}

LazyFileIteratorBase LazyFileIteratorBase::operator-(size_t offset) const {
    LazyFileIteratorBase output = *this;
    if (offset <= output.element_index_) {
        output.element_index_ -= offset;
    } else {
        output.element_index_ = 0;
    }
    return output;
}

LazyFileIteratorBase& LazyFileIteratorBase::operator+=(size_t offset) {
    element_index_ += offset;
    return *this;
}

LazyFileIteratorBase& LazyFileIteratorBase::operator-=(size_t offset) {
    if (offset <= element_index_) {
        element_index_ -= offset;
    } else {
        element_index_ = 0;
    }
    return *this;
}

LazyFileIteratorBase& LazyFileIteratorBase::operator++() {
    ++element_index_;
    return *this;
}

LazyFileIteratorBase& LazyFileIteratorBase::operator--() {
    if (element_index_) {
        --element_index_;
    }
    return *this;
}

bool LazyFileIteratorBase::operator==(const LazyFileIteratorBase& other) const {
    return file_buffer_ == other.file_buffer_ && element_index_ == other.element_index_;
}

bool LazyFileIteratorBase::operator!=(const LazyFileIteratorBase& other) const {
    return !(*this == other);
}

LazyFileIteratorBase::difference_type LazyFileIteratorBase::operator-(
    const LazyFileIteratorBase& other) const {
    return element_index_ - other.element_index_;
}