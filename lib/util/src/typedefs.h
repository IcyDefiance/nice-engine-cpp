#pragma once
#include <string>
#include <vector>

using u32 = uint32_t;
using u16 = uint16_t;
using usize = size_t;

template <typename T> using Ptr = std::unique_ptr<T>;
template <typename T> using Ref = std::shared_ptr<T>;
// template <typename T> using Vec = std::vector<T>;
template <typename T, typename Allocator = std::allocator<T>>
class Vec : public std::vector<T, Allocator> {
public:
	Vec() noexcept(noexcept(Allocator())) : std::vector<T, Allocator>() {}
	explicit Vec( const Allocator& alloc ) : std::vector<T, Allocator>(alloc) noexcept {}
	Vec(size_type count, const T& value, const Allocator& alloc = Allocator()) : std::vector<T, Allocator>(count, value, alloc) {}
	explicit Vec(size_type count, const Allocator& alloc = Allocator()) : std::vector<T, Allocator>(count, alloc) {}
	template<class InputIt>
	Vec(InputIt first, InputIt last, const Allocator& alloc = Allocator()) : std::vector<T, Allocator>(first, last, alloc) {}
	Vec(const Vec& other) = delete;
	Vec(const Vec& other, const Allocator& alloc) : std::vector<T, Allocator>(other, alloc) {}
	Vec(Vec&& other) noexcept : std::vector<T, Allocator>(other) {}
	Vec(Vec&& other, const Allocator& alloc) : std::vector<T, Allocator>(other, alloc) {}
	Vec(std::initializer_list<T> init, const Allocator& alloc = Allocator()) : std::vector<T, Allocator>(init, alloc) {}
};

#ifdef _WIN32
using OsString = std::wstring;
#else
using OsString = std::string;
#endif
