cmake_minimum_required (VERSION 3.9.0)

project(BenchmarkECS)

add_executable(${PROJECT_NAME} "") 

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

target_sources(${PROJECT_NAME}
    PUBLIC 
        src/benchmark.cpp 
        src/LoopTesting.cpp)

target_link_libraries(${PROJECT_NAME} CabbagesECS debug		cabbages-testd)
target_link_libraries(${PROJECT_NAME} CabbagesECS optimized cabbages-test)

