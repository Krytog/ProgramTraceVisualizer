#include "ConsoleLogger.h"

#include <iostream>

ConsoleLogger& ConsoleLogger::GetInstance() {
	static ConsoleLogger logger;
	return logger;
}

void ConsoleLogger::AddToLog(const std::string& message) const {
	std::cout << message << std::endl;
}