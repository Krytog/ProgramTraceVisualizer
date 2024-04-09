#pragma once

#include <string>
#include <memory>
#include <utility>
#include <fstream>

class FileReader {
public:
    FileReader(const std::string& filename);

    /* Returns the size in bytes of the underlying file */
    size_t GetSize() const;

    /* Reads a chunk with given parameters from the file. Returns the chunk and its real size */
    std::pair<std::unique_ptr<char[]>, size_t> GetChunk(size_t offset, size_t size);

private:
    std::ifstream file_;
    size_t size_;
};