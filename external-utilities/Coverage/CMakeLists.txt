add_library(PC_Coverage FileLogger.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE PC_Coverage)