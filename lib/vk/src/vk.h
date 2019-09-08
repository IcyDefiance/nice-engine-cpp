#pragma once
#include "util.h"
#include <string>

namespace vk {
	struct Vulkan
	{
		static util::Result<Vulkan, std::wstring> create();

	private:
		struct Impl;
		util::Arc<Impl> impl;

		Vulkan() = default;
	};
}
