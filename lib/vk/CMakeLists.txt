add_library(vk src/vk.cpp)
set_property(TARGET vk PROPERTY CXX_STANDARD 17)
target_include_directories(vk PRIVATE ../../include ../util/include)
target_include_directories(vk PUBLIC ../util/include ../Vulkan-Headers/include)
