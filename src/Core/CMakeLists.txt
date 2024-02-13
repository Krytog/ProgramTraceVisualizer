add_library(Core
	null.cpp
	)

add_subdirectory(Plotting)

target_link_libraries(Core PRIVATE Plotting)