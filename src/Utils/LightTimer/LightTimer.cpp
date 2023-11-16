#include "LightTimer.h"

LightTimer::LightTimer() {
	start_clock_ = std::chrono::steady_clock::now();
}

LightTimer::~LightTimer() = default;

double LightTimer::EvaluateTime() const {
	std::chrono::duration<double> time_interval = std::chrono::steady_clock::now() - start_clock_;
	return time_interval.count();
}

LightTimer& LightTimer::operator=(const LightTimer& other) {
	start_clock_ = other.start_clock_;
	return *this;
}

void LightTimer::ResetTime() {
	start_clock_ = std::chrono::steady_clock::now();
}
