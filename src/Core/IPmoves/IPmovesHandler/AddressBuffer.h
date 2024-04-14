#pragma once

#include <Utils/LazyFileBuffer/LazyFileBuffer.h>
#include <Utils/LazyFileBuffer/LazyFileIterator.h>

class AddressBuffer {
public:
    using ElementType = uintptr_t;
    using Iterator = LazyFileIterator<ElementType>;

    /* To support standard container::iterator */
    using iterator = Iterator;  // NOLINT

    explicit AddressBuffer(const std::string& filename);

    /* Sets the max amount of memory that can be used for caching data from the file */
    void SetMaxMemory(size_t max_memory);

    /* Returns the max amount of memory that can be used for caching */
    [[nodiscard]] size_t GetMaxMemory() const;

    /* Returns an element at the given index. This method isn't const, as it can modify the underlying file
     * cache */
    ElementType operator[](size_t index);

    /* Returns iterators to begin and to end of this container */
    [[nodiscard]] Iterator Begin();
    [[nodiscard]] Iterator End();

    /* Returns the number of addresses in the file */
    [[nodiscard]] size_t GetSize() const;

    /* For STL compatibility */
    Iterator begin();     // NOLINT
    Iterator end();       // NOLINT
    size_t size() const;  // NOLINT

private:
    LazyFileBuffer file_;
    size_t elements_count_;
};