#include <iostream>
#include "fmt/core.h"
#include "vk.h"
#include <array>

using namespace std;
using namespace vk;

void say_hello() {
	auto vulkan = Vulkan::create().unwrap();
	fmt::print("Vulkan {}\n", vulkan->enumerateInstanceVersion());

	fmt::print("extensions:\n");
	auto exts = vulkan->instanceExtProps().unwrap();
	for (auto&& ext : exts) {
		fmt::print("    {} (rev {})\n", ext.name(), ext.specVer());
	}

	fmt::print("layers:\n");
	auto layers = vulkan->instanceLayerProps().unwrap();
	for (auto&& layer : layers) {
		fmt::print("    {} ({} rev {})\n", layer.name(), layer.specVer(), layer.implVer());
	}

	auto instance = Instance::create(vulkan).unwrap();
	auto pdevs = PhysicalDevice::enumerate(instance).unwrap();
	fmt::print("{} device(s) found\n", pdevs.size());

	struct Test {
		int val;

		Test(int val) : val(val) {}
		Test(const Test&) = delete;
		Test(Test&& other) : val(other.val) {}
	};

	auto&& lazy = ([]() {
		array<Test, 10> test{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		return iter(move(test))
			| Transform([](auto&& x) { return x.val * 2; });
	})();

	while (auto&& item = lazy.next()) {
		static_assert(std::is_same<decltype(item), Opt<int>&&>::value);

		fmt::print("{}\n", *item);
	}

	array<Test, 10> test2{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	auto&& lazy2 =  iter(test2);

	while (auto&& item = lazy2.next()) {
		static_assert(std::is_same<decltype(item), Opt<reference_wrapper<Test>>&&>::value);

		fmt::print("{}\n", (*item).get().val);
	}
}
