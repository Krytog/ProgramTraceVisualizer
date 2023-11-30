add_library(Utils
	LightTimer/LightTimer.cpp
	Logger/ConsoleLogger.cpp
		)

target_include_directories(Utils PUBLIC ..)