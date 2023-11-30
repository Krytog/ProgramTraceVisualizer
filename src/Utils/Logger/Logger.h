#pragma once

#include <string>

class Logger {
public:
	virtual void AddToLog(const std::string& message) const = 0;
protected:
	Logger() = default;
};