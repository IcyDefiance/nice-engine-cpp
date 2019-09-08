add_executable(sandbox main.cpp)
set_property(TARGET sandbox PROPERTY CXX_STANDARD 17)
target_include_directories(sandbox PRIVATE ../../include)
target_link_libraries(sandbox nice-engine)
