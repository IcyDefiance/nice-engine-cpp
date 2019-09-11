#pragma once
#include "util.h"
#include "version.h"
#include <string>

namespace vk {
	struct Vulkan
	{
		static Result<Vulkan, OsString> create();

		Version enumerateInstanceVersion();

	private:
		struct Impl;
		Ref<Impl> impl;

		Vulkan() = default;
	};
}
