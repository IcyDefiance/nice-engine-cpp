#include "window.h"
#include "util/util.h"

Ref<WindowCtx> WindowCtx::create() {
	auto xcb = xcb_connect(NULL, NULL);

	auto protocolsCookie = xcb_intern_atom(xcb, true, 12, "WM_PROTOCOLS");
	auto deleteCookie = xcb_intern_atom(xcb, false, 16, "WM_DELETE_WINDOW");
	auto protocolsReply = xcb_intern_atom_reply(xcb, protocolsCookie, nullptr);
	auto deleteReply = xcb_intern_atom_reply(xcb, deleteCookie, nullptr);

	auto protocolsAtom = protocolsReply->atom;
	auto deleteAtom = deleteReply->atom;

	free(protocolsReply);
	free(deleteReply);

	return Ref<WindowCtx>(new WindowCtx { xcb, protocolsAtom, deleteAtom });
}

WindowCtx::~WindowCtx() {
	xcb_disconnect(xcb);
}

Opt<Event> WindowCtx::pollEvent() {
	if (auto event = xcb_poll_for_event(xcb)) {
		switch (event->response_type) {
			case XCB_CLIENT_MESSAGE:
				auto messageEvent = reinterpret_cast<xcb_client_message_event_t*>(event);
				if(messageEvent->data.data32[0] == deleteAtom ) {
					return Some(Event(WindowEvent { messageEvent->window, WindowDestroyed {} }));
				}
				break;
		}
		return Some(Event {});
	} else {
		return None();
	}
}

Window::Window(Ref<WindowCtx> ctx) : ctx(ctx) {
	xcb_screen_t* screen = xcb_setup_roots_iterator(xcb_get_setup(ctx->xcb)).data;

	auto width = 1440;
	auto height = 810;
	auto x = screen->width_in_pixels - width / 2;
	auto y = screen->height_in_pixels - height / 2;

	xcb_drawable_t win = xcb_generate_id(ctx->xcb);
	u32 mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	u32 values[2] = {screen->white_pixel, XCB_EVENT_MASK_KEY_PRESS};
	xcb_create_window(
		ctx->xcb,
		XCB_COPY_FROM_PARENT,
		win,
		screen->root,
		x, y,
		width, height,
		10,
		XCB_WINDOW_CLASS_INPUT_OUTPUT,
		screen->root_visual,
		mask, values
	);

	xcb_change_property(ctx->xcb, XCB_PROP_MODE_REPLACE, win, ctx->protocolsAtom, 4, 32, 1, &ctx->deleteAtom);

	xcb_map_window(ctx->xcb, win);
	xcb_flush(ctx->xcb);
}
