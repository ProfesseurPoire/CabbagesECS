cmake_minimum_required (VERSION 3.9.0)

project(TestECS)

add_executable(${PROJECT_NAME} "") 

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

target_sources(${PROJECT_NAME}
    PUBLIC 
        src/main.cpp 
        src/UTEntity.cpp)

target_link_libraries(${PROJECT_NAME} CabbagesECS cabbages-test)

add_test( NAME ${PROJECT_NAME} COMMAND ${PROJECT_NAME} )