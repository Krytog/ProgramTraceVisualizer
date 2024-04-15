#include "AddressBuffer.h"
#include "FileReader/FileReader.h"
#include "LazyFileBuffer/LazyFileBuffer.h"

#include <cstdint>
#include <stdexcept>

#define ERROR_MESSAGE_INVALID_FILE_SIZE "AddressBuffer: the file size is not divisible by sizeof(uintptr_t), as it should be"

AddressBuffer::AddressBuffer(const std::string& filename): file_(filename) {
    const size_t file_size = file_.GetFileSize();
    const size_t addresses_in_file = file_size / sizeof(ElementType);
    if (addresses_in_file * sizeof(ElementType) != file_size) {
        throw std::runtime_error(ERROR_MESSAGE_INVALID_FILE_SIZE);
    }
    elements_count_ = addresses_in_file;
}

void AddressBuffer::SetMaxMemory(size_t max_memory) {
    file_.SetMaxMemory(max_memory);
}

size_t AddressBuffer::GetMaxMemory() const {
    return file_.GetFileSize();
}

AddressBuffer::ElementType AddressBuffer::operator[](size_t index) {
    return file_.GetAs<ElementType>(index * sizeof(ElementType));
}

AddressBuffer::Iterator AddressBuffer::Begin() {
    return {&file_, 0};
}

AddressBuffer::Iterator AddressBuffer::End() {
    return {&file_, elements_count_};
}

size_t AddressBuffer::GetSize() const {
    return elements_count_;
}

AddressBuffer::Iterator AddressBuffer::begin() {
    return Begin();
}

AddressBuffer::Iterator AddressBuffer::end() {
    return End();
}

size_t AddressBuffer::size() const {
    return GetSize();
}

FileReader* AddressBuffer::GetRawFileReader() {
    return file_.GetRawFileReader();
}