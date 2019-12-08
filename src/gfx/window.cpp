#include "window.h"
#include "util/util.h"

using namespace std;

#ifdef _WIN32
const wchar_t* WNDCLASS_NAME = L"Window";
thread_local Opt<Event> EVENT = None();

wstring getLastErrorMessage() {
	DWORD err = GetLastError();

	LPWSTR buf = NULL;
	size_t size = FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&buf, 0, NULL
	);

	wstring message(buf, size);

	LocalFree(buf);

	return message;
}

LRESULT CALLBACK windowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	DefWindowProc(hwnd, uMsg, wParam, lParam);

	switch (uMsg)
	{
	case WM_CLOSE:
		EVENT = Some(Event(WindowEvent { hwnd, WindowDestroyed {} }));
	}
}
#endif

Result<Ref<WindowCtx>, OsString> WindowCtx::create() {
#ifdef _WIN32
	auto hInstance = GetModuleHandle(NULL);

	WNDCLASS wc = {};
	wc.lpfnWndProc = windowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = WNDCLASS_NAME;
	auto classAtom = RegisterClass(&wc);
	if (!classAtom) {
		return Err(getLastErrorMessage());
	}

	return Ok(Ref<WindowCtx>(new WindowCtx { hInstance, classAtom }));
#else
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
#endif
}

WindowCtx::~WindowCtx() {
#ifdef _WIN32
	UnregisterClass(WNDCLASS_NAME, hInstance);
#else
	xcb_disconnect(xcb);
#endif
}

Opt<Event> WindowCtx::pollEvent() const {
#ifdef _WIN32
	MSG msg;
	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (EVENT) {
			return EVENT.take();
		}
	}
#else
	while (auto event = xcb_poll_for_event(xcb)) {
		switch (event->response_type) {
			case XCB_CLIENT_MESSAGE:
				auto messageEvent = reinterpret_cast<xcb_client_message_event_t*>(event);
				if(messageEvent->data.data32[0] == deleteAtom ) {
					return Some(Event(WindowEvent { messageEvent->window, WindowDestroyed {} }));
				}
				break;
		}
	}
#endif

	return None();
}

Result<Window, OsString> Window::create(Ref<WindowCtx> ctx) {
#ifdef _WIN32
	auto hwnd = CreateWindowEx(
		0,
		reinterpret_cast<LPCWSTR>(ctx->classAtom),
		L"Space Thing",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		ctx->hInstance,
		NULL
	);
	if (!hwnd) {
		return Err(getLastErrorMessage());
	}

	ShowWindow(hwnd, SW_SHOW);

	return Ok(Window { ctx, hwnd });
#else
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
#endif
}
