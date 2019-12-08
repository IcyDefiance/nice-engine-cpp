#include "gfx/gfx.h"
#include "gfx/window.h"

using namespace std;

int main() {
	auto gfx = Gfx::create("space-thing", VK_MAKE_VERSION(0, 0, 0)).unwrap();
	auto windowCtx = WindowCtx::create().unwrap();
	auto window = Window::create(windowCtx).unwrap();

	while (true) {
		while (auto event = windowCtx->pollEvent()) {
			printf("event");

			if (auto winEvent = get_if<WindowEvent>(&*event)) {
				if (holds_alternative<WindowDestroyed>(winEvent->event)) {
					return 0;
				}
			}
		}

		// draw things
	}
}
