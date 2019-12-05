#include "gfx/gfx.h"

int main() {
	auto gfx = Gfx::create("space-thing", VK_MAKE_VERSION(0, 0, 0)).unwrap();
}
