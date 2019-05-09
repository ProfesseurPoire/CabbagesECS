
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    find_package(cabbages-testd 1.0.0 NAMES CONFIG REQUIRED)
else()
    find_package(cabbages-test 1.0.0 NAMES CONFIG REQUIRED)
endif()

enable_testing()
add_subdirectory(benchmark)
add_subdirectory(unit)
add_subdirectory(integration)