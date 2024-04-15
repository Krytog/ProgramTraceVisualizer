add_library(IPmoves
	IPmovesHandler/IPmovesHandler.cpp
	IPmovesHandler/AddressBuffer.cpp
		)

target_link_libraries(IPmoves PRIVATE Plotting)
target_link_libraries(IPmoves PRIVATE Utils)

target_include_directories(IPmoves PUBLIC ${BASEPATH}/src)