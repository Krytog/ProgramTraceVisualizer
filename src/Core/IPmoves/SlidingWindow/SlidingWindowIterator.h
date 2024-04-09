#pragma once

#include <utility>
#include <iterator>

template <typename Container>
concept HasRandomAccessIterator = std::random_access_iterator<typename Container::iterator>;

template <HasRandomAccessIterator Container>
class SlidingWindowIterator {
	using Iterator = typename Container::iterator;

public:
	SlidingWindowIterator(Container& data, size_t window_size): 
		container_begin_(data.begin()),
		container_end_(data.end()),
		begin_(data.begin()),
		window_size_(window_size)
	{}

	[[nodiscard]] std::pair<Iterator, Iterator> operator*() const {
		return { begin_, begin_ + window_size_ };
	}

	[[nodiscard]] operator bool() const {
		return window_size_ <= container_end_ - begin_;
	}

	SlidingWindowIterator& operator+=(size_t offset) {
		begin_ += offset;
		return *this;
	}

	SlidingWindowIterator& operator-=(size_t offset) {
		begin_ -= offset;
		if (begin_ < container_begin_) {
			begin_ = container_begin_;
		}
		return *this;
	}

	SlidingWindowIterator& operator++() {
		++begin_;
		return* this;
	}

	void Restart() {
		begin_ = container_begin_;
	}

private:
	const Iterator container_begin_;
	const Iterator container_end_;
	Iterator begin_;
	size_t window_size_;
};