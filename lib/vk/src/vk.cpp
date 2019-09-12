#include "vk.h"
#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#undef WIN32_LEAN_AND_MEAN
#undef UNICODE

#include <iostream>

using namespace std;

#define LOAD_VK_INSTANCE_PFN(instance, name) \
	ret.##name = reinterpret_cast<PFN_##name>(vkGetInstanceProcAddr(instance, #name));

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

	Str ExtProps::name() const {
		return Str(this->extName, strlen(this->extName));
	}

	u32 ExtProps::specVer() const {
		return this->specVersion;
	}

	Str LayerProps::name() const {
		return Str(this->layerName, strlen(this->layerName));
	}

	Version LayerProps::specVer() const {
		return Version(this->specVersion);
	}

	u32 LayerProps::implVer() const {
		return this->implVersion;
	}

	Str LayerProps::desc() const {
		return Str(this->description, strlen(this->description));
	}

	Result<Vulkan, wstring> Vulkan::create() {
#ifdef _WIN32
		auto module = LoadLibrary(L"vulkan-1.dll");
		if (!module) { return Err(getLastErrorMessage()); }

		auto vkGetInstanceProcAddr =
			reinterpret_cast<PFN_vkGetInstanceProcAddr>(GetProcAddress(module, "vkGetInstanceProcAddr"));
		if (!vkGetInstanceProcAddr) { return Err(getLastErrorMessage()); }
#endif

		Vulkan ret;
		ret.module = module;
		LOAD_VK_INSTANCE_PFN(nullptr, vkEnumerateInstanceVersion);
		LOAD_VK_INSTANCE_PFN(nullptr, vkEnumerateInstanceExtensionProperties);
		LOAD_VK_INSTANCE_PFN(nullptr, vkEnumerateInstanceLayerProperties);
		LOAD_VK_INSTANCE_PFN(nullptr, vkCreateInstance);
		return Ok(move(ret));
	}

	Vulkan::Vulkan(Vulkan&& other) :
		module(other.module),
		vkGetInstanceProcAddr(other.vkGetInstanceProcAddr),
		vkEnumerateInstanceVersion(other.vkEnumerateInstanceVersion),
		vkEnumerateInstanceExtensionProperties(other.vkEnumerateInstanceExtensionProperties),
		vkEnumerateInstanceLayerProperties(other.vkEnumerateInstanceLayerProperties),
		vkCreateInstance(other.vkCreateInstance)
	{
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
		if (!this->vkEnumerateInstanceVersion) {
			return Version(1, 0, 0);
		}

		u32 vk;
		if (this->vkEnumerateInstanceVersion(&vk)) {
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
	Result<Vec<ExtProps>, InstanceExtPropsError> Vulkan::instanceExtProps() const {
		// Extensions can change at any time. If an extension is added between function calls, the second call may
		// return VK_INCOMPLETE, so we try again until we get VK_SUCCESS.
		while (true) {
			u32 size;
			auto res = this->vkEnumerateInstanceExtensionProperties(nullptr, &size, nullptr);
			if (res != VK_SUCCESS) {
				return Err(static_cast<InstanceExtPropsError>(res));
			}

			Vec<ExtProps> ret(size);
			auto data = reinterpret_cast<VkExtensionProperties*>(ret.data());
			res = this->vkEnumerateInstanceExtensionProperties(nullptr, &size, data);
			if (res == VK_SUCCESS) {
				return Ok(ret);
			} else if (res != VK_INCOMPLETE) {
				return Err(static_cast<InstanceExtPropsError>(res));
			}
		}
	}

	// Success
	// * VK_SUCCESS
	// * VK_INCOMPLETE
	// Failure
	// * VK_ERROR_OUT_OF_HOST_MEMORY
	// * VK_ERROR_OUT_OF_DEVICE_MEMORY
	Result<Vec<LayerProps>, InstanceLayerPropsError> Vulkan::instanceLayerProps() const {
		// Layers can change at any time. If an layer is added between function calls, the second call may
		// return VK_INCOMPLETE, so we try again until we get VK_SUCCESS.
		while (true) {
			u32 size;
			auto res = this->vkEnumerateInstanceLayerProperties(&size, nullptr);
			if (res != VK_SUCCESS) {
				return Err(static_cast<InstanceLayerPropsError>(res));
			}

			Vec<LayerProps> ret(size);
			auto data = reinterpret_cast<VkLayerProperties*>(ret.data());
			res = this->vkEnumerateInstanceLayerProperties(&size, data);
			if (res == VK_SUCCESS) {
				return Ok(ret);
			} else if (res != VK_INCOMPLETE) {
				return Err(static_cast<InstanceLayerPropsError>(res));
			}
		}
	}
}
