#include <iostream>
#include "vk.h"

using namespace std;
using namespace vk;

void say_hello() {
	auto vulkan = Vulkan::create().unwrap();
	cout << "end" << endl;
}
