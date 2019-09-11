#include <iostream>
#include "fmt/core.h"
#include "vk.h"

using namespace std;
using namespace vk;

void say_hello() {
	auto vulkan = Vulkan::create().unwrap();
	fmt::print("Vulkan {}", vulkan.enumerateInstanceVersion());
}
