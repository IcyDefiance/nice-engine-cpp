#ifdef _WIN32
#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#undef WIN32_LEAN_AND_MEAN
#undef UNICODE
#endif

#include <iostream>
#include "vk.h"

using namespace std;

#define LOAD_VK_PFN(name) \
	fns.##name = reinterpret_cast<PFN_##name>(vkGetInstanceProcAddr(nullptr, #name));

namespace vk {
	#ifdef _WIN32
	wstring getLastErrorMessage() {
		DWORD err = GetLastError();

		LPWSTR buf = NULL;
		size_t size = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&buf, 0, NULL
		);

		wstring message(buf, size);

		LocalFree(buf);

		return message;
	}
	#endif

	const char* ExtProps::name() const {
		return this->data.extensionName;
	}

	u32 ExtProps::specVer() const {
		return this->data.specVersion;
	}

	const char* LayerProps::name() const {
		return this->data.layerName;
	}

	Version LayerProps::specVer() const {
		return Version(this->data.specVersion);
	}

	u32 LayerProps::implVer() const {
		return this->data.implementationVersion;
	}

	const char* LayerProps::desc() const {
		return this->data.description;
	}

	Result<Ref<Vulkan>, OsString> Vulkan::create() {
#ifdef _WIN32
		auto module = LoadLibrary(L"vulkan-1.dll");
		if (!module) { return Err(getLastErrorMessage()); }

		auto vkGetInstanceProcAddr =
			reinterpret_cast<PFN_vkGetInstanceProcAddr>(GetProcAddress(module, "vkGetInstanceProcAddr"));
		if (!vkGetInstanceProcAddr) { return Err(getLastErrorMessage()); }
#endif

		VulkanFns fns;
		fns.vkGetInstanceProcAddr = vkGetInstanceProcAddr;
		LOAD_VK_PFN(vkEnumerateInstanceVersion);
		LOAD_VK_PFN(vkEnumerateInstanceExtensionProperties);
		LOAD_VK_PFN(vkEnumerateInstanceLayerProperties);
		LOAD_VK_PFN(vkCreateInstance);

		return Ok(make_shared<Vulkan>(module, fns));
	}

	Vulkan::Vulkan(const void* module, VulkanFns fns) : module(module), fns(fns) {}

	Vulkan::Vulkan(Vulkan&& other) : module(other.module), fns(other.fns) {
		other.module = nullptr;
	}

	Vulkan::~Vulkan() noexcept {
		if (this->module && !FreeLibrary((HMODULE)this->module)) {
			wcerr << getLastErrorMessage() << endl;
			terminate();
		}
	}

	// Success
	// * VK_SUCCESS
	Version Vulkan::enumerateInstanceVersion() const {
		if (!this->fns.vkEnumerateInstanceVersion) {
			return Version(1, 0, 0);
		}

		u32 vk;
		if (this->fns.vkEnumerateInstanceVersion(&vk)) {
			terminate();
		}
		return Version(vk);
	}

	// Success
	// * VK_SUCCESS
	// * VK_INCOMPLETE
	// Failure
	// * VK_ERROR_OUT_OF_HOST_MEMORY
	// * VK_ERROR_OUT_OF_DEVICE_MEMORY
	// * VK_ERROR_LAYER_NOT_PRESENT
	Result<Vec<ExtProps>, VkResult> Vulkan::instanceExtProps() const {
		// Extensions can change at any time. If an extension is added between function calls, the second call may
		// return VK_INCOMPLETE, so we try again until we get VK_SUCCESS.
		while (true) {
			u32 size;
			auto res = this->fns.vkEnumerateInstanceExtensionProperties(nullptr, &size, nullptr);
			if (res != VK_SUCCESS) {
				return Err(res);
			}

			Vec<ExtProps> ret(size);
			auto data = reinterpret_cast<VkExtensionProperties*>(ret.data());
			res = this->fns.vkEnumerateInstanceExtensionProperties(nullptr, &size, data);
			if (res == VK_SUCCESS) {
				return Ok(ret);
			} else if (res != VK_INCOMPLETE) {
				return Err(res);
			}
		}
	}

	// Success
	// * VK_SUCCESS
	// * VK_INCOMPLETE
	// Failure
	// * VK_ERROR_OUT_OF_HOST_MEMORY
	// * VK_ERROR_OUT_OF_DEVICE_MEMORY
	Result<Vec<LayerProps>, VkResult> Vulkan::instanceLayerProps() const {
		// Layers can change at any time. If an layer is added between function calls, the second call may
		// return VK_INCOMPLETE, so we try again until we get VK_SUCCESS.
		while (true) {
			u32 size;
			auto res = this->fns.vkEnumerateInstanceLayerProperties(&size, nullptr);
			if (res != VK_SUCCESS) {
				return Err(res);
			}

			Vec<LayerProps> ret(size);
			auto data = reinterpret_cast<VkLayerProperties*>(ret.data());
			res = this->fns.vkEnumerateInstanceLayerProperties(&size, data);
			if (res == VK_SUCCESS) {
				return Ok(ret);
			} else if (res != VK_INCOMPLETE) {
				return Err(res);
			}
		}
	}
}
