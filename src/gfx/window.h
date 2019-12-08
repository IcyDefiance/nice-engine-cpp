#pragma once
#include "xcb/xcb.h"
#include "util/util.h"

struct WindowDestroyed {};

struct WindowEvent {
	xcb_window_t windowId;
	std::variant<WindowDestroyed> event;
};

using Event = std::variant<WindowEvent>;

struct WindowCtx {
	xcb_connection_t* xcb;
	xcb_atom_t protocolsAtom;
	xcb_atom_t deleteAtom;

	static Ref<WindowCtx> create();

	WindowCtx(const WindowCtx&) = delete;
	~WindowCtx();

	Opt<Event> pollEvent();
};

struct Window {
	Ref<WindowCtx> ctx;

	Window(Ref<WindowCtx> ctx);
};
