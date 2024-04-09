add_library(Utils
	LightTimer/LightTimer.cpp
	Logger/ConsoleLogger.cpp
	FileReader/FileReader.cpp
		)

target_include_directories(Utils PUBLIC ..)