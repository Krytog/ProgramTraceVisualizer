#pragma once

#include "Logger.h"

class ConsoleLogger : public Logger {
public:
	static ConsoleLogger& GetInstance();

	void AddToLog(const std::string& message) const override;

private:
	ConsoleLogger() = default;
};