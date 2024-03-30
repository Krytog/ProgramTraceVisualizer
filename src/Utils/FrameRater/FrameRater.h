#pragma once

#include <chrono>
#include <ratio>
#include <thread>

template <uint32_t FPS>
class FrameRater {
public:
    FrameRater() : frame_time_(1), time_point_(std::chrono::steady_clock::now()) {
    }

    /* Forces the current thread to sleep until the beginning of the next frame */
    void Sleep() {
        time_point_ += frame_time_;
		if (false && time_point_ < std::chrono::steady_clock::now()) {
			time_point_ = std::chrono::steady_clock::now();
			return;
		} 
        std::this_thread::sleep_until(time_point_);
    }

    /* Returns the timepoint of the beginning of the current frame, it can be used to measure performance */
    double GetElapsedFrametime() const {
		std::chrono::duration<double> interval = std::chrono::steady_clock::now() - time_point_;
        return interval.count();
    }

private:
    std::chrono::duration<double, std::ratio<1, FPS>> frame_time_;
    std::chrono::time_point<std::chrono::steady_clock, decltype(frame_time_)> time_point_;
};