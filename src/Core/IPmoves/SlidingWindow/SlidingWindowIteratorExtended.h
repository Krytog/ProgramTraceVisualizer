#pragma once

#include <utility>
#include <cstddef>

template <typename Container>
class SlidingWindowIteratorExtended {
    using Iterator = typename Container::iterator;

public:
    SlidingWindowIteratorExtended(Container& data)
        : container_begin_(data.begin()), container_size_(data.size()), pos_(0), window_size_(0) {
    }

    [[nodiscard]] std::pair<Iterator, Iterator> operator*() const {
        const Iterator begin = container_begin_ + GetBeginPos();
        const Iterator end = container_begin_ + GetEndPos();
        return {begin, end};
    }

    [[nodiscard]] operator bool() const {
        return pos_ < container_size_ + window_size_ - 1;
    }

    void Advance(size_t to_advance) {
        pos_ += to_advance;
    }

    void Restart() {
        pos_ = 0;
    }

    void SetSize(size_t size) {
        window_size_ = size;
    }

    [[nodiscard]] size_t GetWindowSize() const {
        return window_size_;
    }

    void SetPosition(size_t position) {
        pos_ = position;
    }

    void SetProgress(float alpha) {
        const size_t position = alpha * (container_size_ + window_size_ - 1);
        SetPosition(position);
    }

    [[nodiscard]] float GetProgress() const {
        return static_cast<float>(pos_) / (container_size_ + window_size_ - 1);
    }

    /* Returns an iterator to begin, an iterator to end, and an offset. A positive offset equals to the number
    of element that should go before the window, a negative offset equals to the number of elements that
    should go after the window */
    [[nodiscard]] std::tuple<Iterator, Iterator, ptrdiff_t> GetWindowWithOffset() const {
        ptrdiff_t offset = 0;
        if (pos_ < window_size_) {
            offset = window_size_ - 1 - pos_;
        } else if (pos_ >= container_size_) {
            offset = container_size_ - 1 - pos_;
        }
        auto [begin, end] = SlidingWindowIteratorExtended::operator*();
        return {begin, end, offset};
    }

private:
    size_t GetBeginPos() const {
        if (pos_ < window_size_) {
            return 0;
        }
        return pos_ + 1 - window_size_;
    }

    size_t GetEndPos() const {
        const size_t advanced_pos = pos_ + 1;
        return std::min(container_size_, advanced_pos);
    }

    const Iterator container_begin_;
    const size_t container_size_;
    size_t pos_;
    size_t window_size_;
};