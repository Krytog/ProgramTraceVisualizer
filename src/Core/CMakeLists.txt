add_library(Core
	null.cpp
	)

add_subdirectory(Plotting)
add_subdirectory(IPmoves)

target_link_libraries(Core PRIVATE Plotting)
target_link_libraries(Core PRIVATE IPmoves)