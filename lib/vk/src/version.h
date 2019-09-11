#pragma once
#include "util.h"
#include "fmt/core.h"

namespace vk {
	struct Version
	{
		Version(u32 vk);
		Version(u16 major, u16 minor, u16 patch);

		u16 major() const;
		u16 minor() const;
		u16 patch() const;

	private:
		u32 val;
	};
}

template <>
struct fmt::formatter<vk::Version> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const vk::Version &val, FormatContext &ctx) {
		return format_to(ctx.out(), "{}.{}.{}", val.major(), val.minor(), val.patch());
	}
};
