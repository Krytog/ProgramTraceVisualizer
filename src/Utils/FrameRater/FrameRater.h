#pragma once

#include <chrono>
#include <ratio>
#include <thread>

template <uint32_t FPS>
class FrameRater {
public:
	FrameRater() : frame_time_(1), time_point_(std::chrono::steady_clock::now()) {}

	void Sleep() {
		time_point_ += frame_time_;
		std::this_thread::sleep_until(time_point_);
	}

private:
	std::chrono::duration<double, std::ratio<1, FPS>> frame_time_;
	std::chrono::time_point<std::chrono::steady_clock, decltype(frame_time_)> time_point_;
};