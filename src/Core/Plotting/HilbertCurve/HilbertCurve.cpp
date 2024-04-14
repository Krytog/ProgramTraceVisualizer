#include "HilbertCurve.h"

#include <cmath>
#include <stdexcept>

#define EPS 1e-9
#define MAX_DEGREE 31
#define HilbertCurveExceptionMessage_SeqNumTooBig "HilbertCurve2D: seq_num is too big for this curve!"
#define HilbertCurveExceptionMessage_NoSuchPoint "HilbertCurve2D: no such point contained in this curve!"
#define HilbertCurveExceptionMessage_TooBigSize "HilbertCurve2D: trying to create a curve with too big size!"

namespace {
	using Point = HilbertCurve2D::Point;

	static std::pair<size_t, size_t> Rotate(size_t n, size_t rx, size_t ry, size_t x, size_t y) {
		if (ry) {
			return { x, y };
		}
		if (rx == 1)
		{
			x = n - 1 - x;
			y = n - 1 - y;
		}
		return { y, x };
	}
}

bool HilbertCurve2D::Point::operator==(const Point& other) const{
	return std::abs(x - other.x) < EPS && std::abs(y - other.y) < EPS;
}

size_t HilbertCurve2D::Point::Hasher::operator()(const Point& point) const {
	const unsigned high_bits = *reinterpret_cast<const unsigned*>(&point.x);
	const unsigned low_bits = *reinterpret_cast<const unsigned*>(&point.y);
	size_t output = high_bits;
	output <<= 32; // shifting by number of bits in unsigned
	output += low_bits;
	return output;
}

HilbertCurve2D::Point HilbertCurve2D::Seq2XY(size_t seq_num) const {
	if (seq_num >= total_cells_) {
		throw std::runtime_error(HilbertCurveExceptionMessage_SeqNumTooBig);
	}
	return seq_2_xy_.at(seq_num);
}

size_t HilbertCurve2D::XY2Seq(const Point& point) const {
	auto iter = xy_2_seq_.find(point);
	if (iter == xy_2_seq_.end()) {
		throw std::runtime_error(HilbertCurveExceptionMessage_NoSuchPoint);
	}
	return iter->second;
}

HilbertCurve2D::Point HilbertCurve2D::Seq2XYSlow(size_t seq_num) const {
	size_t x = 0;
	size_t y = 0;
	for (size_t i = 1; i < size_; i *= 2)
	{
		size_t rx = (1 & (seq_num / 2));
		size_t ry = (1 & (seq_num ^ rx));
		auto pair = Rotate(i, rx, ry, x, y);
		x = pair.first;
		y = pair.second;
		x = x + i * rx;
		y = y + i * ry;
		seq_num /= 4;
	}
	const float x_f = static_cast<float>(x) / size_;
	const float y_f = static_cast<float>(y) / size_;
	const float offset = 0.5f / size_; // forces the point to be the center of the cell, otherwise it's the bottom left corner
	return { x_f + offset, y_f + offset};
}

HilbertCurve2D::HilbertCurve2D(unsigned degree): size_(1ull << degree), total_cells_(size_* size_) {
	if (degree > MAX_DEGREE) {
		throw std::runtime_error(HilbertCurveExceptionMessage_TooBigSize);
	}
	for (size_t i = 0; i < total_cells_; ++i) {
		auto point = Seq2XYSlow(i);
		seq_2_xy_[i] = point;
		xy_2_seq_[point] = i;
	}
}