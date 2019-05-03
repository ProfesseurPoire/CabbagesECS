cmake_minimum_required (VERSION 3.9.0)

project(CabbagesECS VERSION 1.0.0)

set_property(GLOBAL PROPERTY FIND_LIBRARY_USE_LIB64_PATHS TRUE)

add_library(${PROJECT_NAME} STATIC "") 

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    find_package(SimpleListd 1.0.0 NAMES CONFIG REQUIRED)
else()
    find_package(SimpleList 1.0.0 NAMES CONFIG REQUIRED)
endif()

target_link_libraries(${PROJECT_NAME}  debug		SimpleListd)
target_link_libraries(${PROJECT_NAME}  optimized	SimpleList)


target_sources(${PROJECT_NAME} PRIVATE
    src/entity.cpp
    src/EntityManager.cpp
    src/SystemManager.cpp
    include/cabba/ecs/ComponentPool.h
    include/cabba/ecs/Constant.h
    include/cabba/ecs/Entity.h
    include/cabba/ecs/EntityManager.h
    include/cabba/ecs/System.h
    include/cabba/ecs/SystemManager.h
    include/cabba/ecs/World.h
)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)
# Public    : Used by the current target and the ones consuming the target
# Private   : Only used by the current target
# Interface : Only used by the targets consuming the current target
# BUILD_INTERFACE and INSTALL_INTERFACE are here to specify which path to
# use depending on if the library is installed or not
target_include_directories( ${PROJECT_NAME} PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

add_subdirectory(tests)
