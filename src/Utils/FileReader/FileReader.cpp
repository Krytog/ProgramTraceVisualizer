#include "FileReader.h"

#include <memory>
#include <stdexcept>

#define ERROR_MESSAGE_FAILED_TO_OPEN "FileHandler: failed to open file with name: "
#define ERROR_MESSAGE_SEEK_FAILED "FileHandler: seek failed"

FileReader::FileReader(const std::string& filename): file_(std::ifstream(filename, std::ios::binary)) {
    if (!file_.is_open()) {
        std::string error_message = ERROR_MESSAGE_FAILED_TO_OPEN;
        error_message += filename; 
        throw std::runtime_error(error_message);
    }
    const auto begin = file_.tellg();
    file_.seekg(0, std::ios::end);
    const auto end = file_.tellg();
    size_ = end - begin;
}


size_t FileReader::GetSize() const {
    return size_;
}

std::pair<std::unique_ptr<char[]>, size_t> FileReader::GetChunk(size_t offset, size_t size) {
    file_.seekg(offset);
    if (!file_.good()) {
        throw std::runtime_error(ERROR_MESSAGE_SEEK_FAILED);
    }
    std::unique_ptr<char[]> array = std::make_unique<char[]>(size);
    file_.read(array.get(), size);
    const auto bytes_read = file_.gcount();
    return {std::move(array), bytes_read};
}