cmake_minimum_required(VERSION 3.0.0)
project(space-thing VERSION 0.1.0)

include(CTest)
enable_testing()

option(VIDEO_VULKAN ON)

add_subdirectory(lib/fmt)
add_subdirectory(lib/SDL)
add_subdirectory(lib/util)

add_executable(space-thing src/main.cpp src/gfx/gfx.cpp)
set_property(TARGET space-thing PROPERTY CXX_STANDARD 17)
target_compile_definitions(space-thing PRIVATE GGD_PLUGIN_CODE)
target_include_directories(space-thing PRIVATE lib/util/include lib/Vulkan-Headers/include)
target_link_libraries(space-thing SDL2-static fmt)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
