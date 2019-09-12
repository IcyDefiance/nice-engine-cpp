#include "typedefs.h"
#include "fmt/core.h"

struct Str {
	friend fmt::formatter<Str>;

	Str(const char* str, usize len): str(str), len(len) {}

private:
	const char* str;
	usize len;
};

template <>
struct fmt::formatter<Str> {
	template <typename ParseContext>
	constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

	template <typename FormatContext>
	auto format(const Str &val, FormatContext &ctx) {
		return format_to(ctx.out(), "{}", std::string(val.str, val.len));
	}
};
