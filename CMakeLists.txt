cmake_minimum_required(VERSION 3.0.0)
project(nice-engine VERSION 0.1.0)

include(CTest)
enable_testing()

add_subdirectory(lib/fmt)
add_subdirectory(lib/util)
add_subdirectory(lib/vk)

add_library(nice-engine src/nice-engine.cpp)
set_property(TARGET nice-engine PROPERTY CXX_STANDARD 17)
target_include_directories(nice-engine PRIVATE lib/vk/include)
target_link_libraries(nice-engine fmt vk)

add_subdirectory(examples/sandbox)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
