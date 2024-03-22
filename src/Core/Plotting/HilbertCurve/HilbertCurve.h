#pragma once

#include <unordered_map>
#include <cstddef>

class HilbertCurve2D {
public:
	struct Point {
		float x;
		float y;

		bool operator==(const Point& other) const;

		struct Hasher {
			size_t operator()(const Point& point) const;
		};
	};

	HilbertCurve2D(unsigned degree);

	/* Returns the coordinates of the corresponding cell. This method finds the answer in precalculated map, so it's fast */
	[[nodiscard]] Point Seq2XY(size_t seq_num) const;

	/* Returns the sequence number of the corresponding cell. This method finds the answer in precalculated map, so it's fast */
	[[nodiscard]] size_t XY2Seq(const Point& point) const;

private:
	/* This method computes the coordinates, it's slow and should be used only for precalculations */
	[[nodiscard]] Point Seq2XYSlow(size_t seq_num) const;

	std::unordered_map<size_t, Point> seq_2_xy_;
	std::unordered_map<Point, size_t, Point::Hasher> xy_2_seq_;
	const size_t size_;
	const size_t total_cells_;
};