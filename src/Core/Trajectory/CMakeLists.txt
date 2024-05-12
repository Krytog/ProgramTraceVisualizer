add_library(Trajectory
	TrajectoryHandler.cpp
		)

target_link_libraries(Trajectory PRIVATE Plotting)
target_link_libraries(Trajectory PRIVATE Utils)
target_link_libraries(Trajectory PRIVATE GUI)


target_include_directories(Trajectory PUBLIC ${BASEPATH}/src)
target_include_directories(Trajectory PUBLIC ${BASEPATH}/3rdparty)