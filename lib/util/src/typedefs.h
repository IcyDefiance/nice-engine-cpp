#pragma once
#include <string>

namespace util {
	template <typename T>
	using Arc = std::shared_ptr<T>;

	#ifdef _WIN32
	using OsString = std::wstring;
	#else
	using OsString = std::string;
	#endif
}
