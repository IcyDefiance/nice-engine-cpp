#include "version.h"

#define VK_NO_PROTOTYPES
#include "vulkan/vulkan.h"
#undef VK_NO_PROTOTYPES

namespace vk {
	Version::Version(u32 vk): val(vk) {}
	Version::Version(u16 major, u16 minor, u16 patch): val(VK_MAKE_VERSION((u32)major, (u32)minor, (u32)patch)) {}

	u16 Version::major() const { return VK_VERSION_MAJOR(this->val); }
	u16 Version::minor() const { return VK_VERSION_MINOR(this->val); }
	u16 Version::patch() const { return VK_VERSION_PATCH(this->val); }
}
