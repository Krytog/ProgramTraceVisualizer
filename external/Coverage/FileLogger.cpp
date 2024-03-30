#include "FileLogger.h"
#include <cstring>

FileLogger::FileLogger(const std::string& output_file, size_t buffer_size, bool* is_alive)
    : pos_to_write_(0), file_(output_file), is_alive_(is_alive) {
    buffer_.resize(buffer_size);
}

void FileLogger::Write(const std::string& data) {
    Write(data.c_str(), data.size());
}

void FileLogger::Write(const char* data, size_t size) {
    std::unique_lock lock(mutex_);
    if (pos_to_write_ + size >= buffer_.size()) {
        DumpBuffer();
    }
    memcpy(static_cast<void*>(buffer_.data() + pos_to_write_), static_cast<const void*>(data), size);
    pos_to_write_ += size;
}

void FileLogger::DumpBuffer() {
    file_.write(buffer_.c_str(), pos_to_write_);
    pos_to_write_ = 0;
}

FileLogger::~FileLogger() {
    *is_alive_ = false;
    DumpBuffer();
    file_.flush();
}