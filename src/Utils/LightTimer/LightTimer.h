#pragma once

#include <chrono>

class LightTimer {
public:
	LightTimer();
	~LightTimer();

	LightTimer& operator=(const LightTimer& other);

	void ResetTime();

	double EvaluateTime() const;

private:
	std::chrono::time_point<std::chrono::steady_clock> start_clock_;
};
