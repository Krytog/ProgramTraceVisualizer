add_library(Utils
	LightTimer/LightTimer.cpp
	Logger/ConsoleLogger.cpp
	FileReader/FileReader.cpp
	LazyFileBuffer/LazyFileBuffer.cpp
	LazyFileBuffer/LazyFileIterator.cpp
		)

target_include_directories(Utils PUBLIC ${BASEPATH}/src/Utils)