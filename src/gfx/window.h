#pragma once
#ifdef _WIN32
#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#else
#include "xcb/xcb.h"
#endif
#include "util/util.h"

struct WindowDestroyed {};

struct WindowEvent {
#ifdef _WIN32
	HWND windowId;
#else
	xcb_window_t windowId;
#endif
	std::variant<WindowDestroyed> event;
};

using Event = std::variant<WindowEvent>;

struct WindowCtx {
#ifdef _WIN32
	HINSTANCE hInstance;
	ATOM classAtom;
#else
	xcb_connection_t* xcb;
	xcb_atom_t protocolsAtom;
	xcb_atom_t deleteAtom;
#endif

	static Result<Ref<WindowCtx>, OsString> create();

	WindowCtx(const WindowCtx&) = delete;
	~WindowCtx();

	/// Windows has a separate event queue for each thread that a window was created
	/// on, and this function can only return events from the current thread.
	Opt<Event> pollEvent() const;
};

struct Window {
	Ref<WindowCtx> ctx;
#ifdef _WIN32
	HWND hwnd;
#else
	// TODO: store window handle and destroy in destructor
#endif

	static Result<Window, OsString> create(Ref<WindowCtx> ctx);
};
