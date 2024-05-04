add_library(Core
	null.cpp
	)

add_subdirectory(Plotting)
add_subdirectory(IPmoves)
add_subdirectory(w2v)

target_link_libraries(Core PRIVATE Plotting)
target_link_libraries(Core PRIVATE IPmoves)
target_link_libraries(Core PRIVATE w2v)
