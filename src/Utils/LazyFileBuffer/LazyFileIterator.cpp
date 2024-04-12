#include "LazyFileIterator.h"
#include "LazyFileBuffer/LazyFileBuffer.h"

LazyFileIteratorBase::LazyFileIteratorBase(LazyFileBuffer* file_buffer)
    : file_buffer_(file_buffer), file_size_(file_buffer->GetFileSize()), element_index_(0) {
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