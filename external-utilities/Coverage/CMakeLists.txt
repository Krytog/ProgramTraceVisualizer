set(CMAKE_CXX_FLAGS -fsanitize-coverage=func,trace-pc-guard)

add_library(PC_Coverage covers.cpp FileLogger.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE PC_Coverage)