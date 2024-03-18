#pragma once

#include <vector>
#include <limits>

template <typename T>
class DataClamper {
public:
	/* Returns the clamped version (within interval [0, 1]) of the given value */
	[[nodiscard]] float GetClamped(T value) const {
		if (value < min_) {
			return min_;
		}
		if (value > max_) {
			return max_;
		}
		const T numerator = value - min_;
		const T denominator = max_ - min_;
		return static_cast<float>(numerator) / denominator;
	}
protected:
	T min_;
	T max_;
};

template <typename T>
class MinMaxDataClamper : public DataClamper<T> {
public:
	using DataClamper<T>::min_;
	using DataClamper<T>::max_;

	MinMaxDataClamper(const std::vector<T>& data) {
		min_ = std::numeric_limits<T>::max();
		max_ = std::numeric_limits<T>::min();
		for (const auto elem : data) {
			if (elem > max_) {
				max_ = elem;
			}
			if (elem < min_) {
				min_ = elem;
			}
		}
	}
};

template <typename T>
class AbsoluteDataClamper : public DataClamper<T> {
	using DataClamper<T>::min_;
	using DataClamper<T>::max_;

	AbsoluteDataClamper(T min, T max) {
		min_ = min;
		max_ = max;
	}
};