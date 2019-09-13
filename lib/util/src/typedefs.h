#pragma once
#include <string>
#include <vector>

using u32 = uint32_t;
using u16 = uint16_t;
using usize = size_t;

template <typename T> using Ptr = std::unique_ptr<T>;
template <typename T> using Ref = std::shared_ptr<T>;
template <typename T> using Vec = std::vector<T>;

#ifdef _WIN32
using OsString = std::wstring;
#else
using OsString = std::string;
#endif
