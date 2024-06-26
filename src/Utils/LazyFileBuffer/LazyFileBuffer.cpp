#include "LazyFileBuffer.h"
#include "FileReader/FileReader.h"

LazyFileBuffer::LazyFileBuffer(const std::string& name) : file_(name) {
    chunk_.from = 0;
    chunk_.to = 0;
    chunk_size_ = 0;
}

void LazyFileBuffer::SetMaxMemory(size_t max_memory) {
    chunk_size_ = max_memory;
}

void LazyFileBuffer::LoadChunk(size_t position) {
    auto [bytes, size] = file_.GetChunk(position, chunk_size_);
    Chunk chunk{.from = position, .to = position + size, .bytes = std::move(bytes)};
    chunk_ = std::move(chunk);
}

size_t LazyFileBuffer::GetFileSize() const {
    return file_.GetSize();
}

FileReader* LazyFileBuffer::GetRawFileReader() {
    return &file_;
}

size_t LazyFileBuffer::GetMaxMemory() const {
    return chunk_size_;
}