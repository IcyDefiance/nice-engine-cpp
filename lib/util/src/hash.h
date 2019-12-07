#include "typedefs.h"

namespace util {
	struct hash_cstr
	{
		usize operator()(const char* str) const noexcept
		{
			unsigned long hash = 5381;

			while (i32 c = *str++) {
				hash = ((hash << 5) + hash) + c;
			}

			return hash;
		}
	};
}
