#include "util/util.h"
#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1
#include "vulkan/vulkan.hpp"

struct Gfx {
	static Result<Ref<Gfx>, const char*> create(const char* name, u32 version);

	vk::UniqueInstance instance;
#ifndef NDEBUG
	vk::UniqueDebugUtilsMessengerEXT debugMessenger;
#endif
};
