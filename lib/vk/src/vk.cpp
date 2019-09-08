#include "vk.h"
#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#undef WIN32_LEAN_AND_MEAN
#undef UNICODE

#define VK_NO_PROTOTYPES
#include "vulkan/vulkan.h"
#undef VK_NO_PROTOTYPES

#include <iostream>

using namespace std;
using namespace util;

#define LOAD_VK_INSTANCE_PFN(instance, name) this->##name = (PFN_##name)vkGetInstanceProcAddr(instance, #name);

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

	struct Vulkan::Impl {
		HMODULE module;
		PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr;
		PFN_vkEnumerateInstanceVersion vkEnumerateInstanceVersion;
		PFN_vkEnumerateInstanceExtensionProperties vkEnumerateInstanceExtensionProperties;
		PFN_vkEnumerateInstanceLayerProperties vkEnumerateInstanceLayerProperties;
		PFN_vkCreateInstance vkCreateInstance;

		Impl(HMODULE module, PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr):
			module(module),
			vkGetInstanceProcAddr(vkGetInstanceProcAddr)
		{
			LOAD_VK_INSTANCE_PFN(nullptr, vkEnumerateInstanceVersion);
			LOAD_VK_INSTANCE_PFN(nullptr, vkEnumerateInstanceExtensionProperties);
			LOAD_VK_INSTANCE_PFN(nullptr, vkEnumerateInstanceLayerProperties);
			LOAD_VK_INSTANCE_PFN(nullptr, vkCreateInstance);
		}

		Impl(const Impl&) = delete;

		Impl(Impl&& other) :
			module(other.module),
			vkGetInstanceProcAddr(other.vkGetInstanceProcAddr),
			vkEnumerateInstanceVersion(other.vkEnumerateInstanceVersion),
			vkEnumerateInstanceExtensionProperties(other.vkEnumerateInstanceExtensionProperties),
			vkEnumerateInstanceLayerProperties(other.vkEnumerateInstanceLayerProperties),
			vkCreateInstance(other.vkCreateInstance)
		{}

		~Impl() noexcept {
			if (this->module && !FreeLibrary(this->module)) {
				wcerr << getLastErrorMessage() << endl;
				terminate();
			}
		}
	};

	Result<Vulkan, wstring> Vulkan::create() {
#ifdef _WIN32
		auto module = LoadLibrary(L"vulkan-1.dll");
		if (!module) { return Err(getLastErrorMessage()); }

		auto vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)(GetProcAddress(module, "vkGetInstanceProcAddr"));
		if (!vkGetInstanceProcAddr) { return Err(getLastErrorMessage()); }
#endif

		Vulkan ret;
		ret.impl = make_shared<Impl>(module, vkGetInstanceProcAddr);
		return Ok(ret);
	}
}
