add_library(IPmoves
	Clamping/DataClamper.cpp
	IPmovesHandler/IPmovesHandler.cpp
		)

target_link_libraries(IPmoves PRIVATE Plotting)
target_link_libraries(IPmoves PRIVATE Utils)

target_include_directories(IPmoves PUBLIC ${BASEPATH}/src)