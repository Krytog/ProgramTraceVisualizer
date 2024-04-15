#pragma once

#include <FileReader/FileReader.h>

#include <memory>
#include <cassert>

class LazyFileBuffer {
public:
    LazyFileBuffer(const std::string& filename);
    LazyFileBuffer(const LazyFileBuffer& other) = delete;
    LazyFileBuffer& operator=(const LazyFileBuffer& other) = delete;

    /* Returns an object at the given position as T type. May modify the underlying caching buffer */
    template <typename T>
    [[nodiscard]] T GetAs(size_t position) {
        assert(sizeof(T) <= chunk_size_);
        char* object_bytes = GetAtPos<T>(position);
        return *reinterpret_cast<T*>(object_bytes);
    }

    /* Sets the max amount of memory that the underlaying cache can use */
    void SetMaxMemory(size_t max_memory);

    /* Returns tha max amount of memory that the underlaying cache can use */
    [[nodiscard]] size_t GetMaxMemory() const;

    /* Returns the size of the underlying file */
    [[nodiscard]] size_t GetFileSize() const;

    /* Returns a pointer to the underlying FileReader. Can be used to read without affecting the caching system */
    [[nodiscard]] FileReader* GetRawFileReader();

private:
    /* Returns a pointer to location of at least sizeof(T) bytes, starting from the byte at the given position */
    template <typename T>
    [[nodiscard]] char* GetAtPos(size_t position) {
        const size_t end_position = position + sizeof(T);
        if (position < chunk_.from || end_position > chunk_.to) {
            LoadChunk(position);
        }
        return chunk_.bytes.get() + position - chunk_.from;
    }

    /* Loads a chunk from file, starting from the byte at position "from" */
    void LoadChunk(size_t from);

    struct Chunk {
        size_t from;
        size_t to;
        std::unique_ptr<char[]> bytes;
    };

    FileReader file_;
    size_t chunk_size_;
    Chunk chunk_;
};