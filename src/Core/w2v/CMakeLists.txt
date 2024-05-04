add_library(w2v
	w2v.cpp
		)

target_link_libraries(w2v PRIVATE Plotting)
target_link_libraries(w2v PRIVATE Utils)

target_include_directories(w2v PUBLIC ${BASEPATH}/src)
target_include_directories(w2v PUBLIC ${BASEPATH}/3rdparty)