cmake_minimum_required(VERSION 3.0.0)
project(space-thing VERSION 0.1.0)
set(CMAKE_MODULE_PATH "${space-thing_SOURCE_DIR}/cmake")

include(CTest)
enable_testing()

option(VIDEO_VULKAN OFF)
set(VIDEO_VULKAN ON)

add_subdirectory(lib/fmt)
add_subdirectory(lib/SDL)
add_subdirectory(lib/util)

if(UNIX)
	find_package(XCB)
	if(NOT XCB_FOUND)
		message(FATAL_ERROR "Xcb library not found")
	endif()
endif()

add_executable(space-thing src/main.cpp src/gfx/gfx.cpp src/gfx/window.cpp)
set_property(TARGET space-thing PROPERTY CXX_STANDARD 17)
target_compile_definitions(space-thing PRIVATE GGD_PLUGIN_CODE)
target_include_directories(space-thing PRIVATE lib/util/include lib/Vulkan-Headers/include)
if(UNIX)
	target_include_directories(space-thing PRIVATE ${XCB_INCLUDE_DIRS})
endif()
target_link_libraries(space-thing SDL2-static fmt ${XCB_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
