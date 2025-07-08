// SPDX-FileCopyrightText: (c) 2025 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include <mathutil/color.h>
#ifdef _WIN32

#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#else
#define GLFW_EXPOSE_NATIVE_X11
#endif
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include <sharedutils/util.h>
#include <sharedutils/def_handle.h>
#include <algorithm>
#include <cassert>

#undef API
#undef None

module pragma.platform;

import :file_drop_target;

namespace pragma::platform {
	DEFINE_BASE_HANDLE(, pragma::platform::Window, Window);
};

pragma::platform::WindowCreationInfo::WindowCreationInfo()
    : resizable(true), visible(true), decorated(true), focused(true), autoIconify(true), floating(false), stereo(false), srgbCapable(false), doublebuffer(true), refreshRate(GLFW_DONT_CARE), samples(0), redBits(8), greenBits(8), blueBits(8), alphaBits(8), depthBits(24), stencilBits(8),
      width(800), height(600), monitor(nullptr)
{
}

pragma::platform::Window::Window(GLFWwindow *window) : m_handle(new PtrWindow(this)), m_window(window), m_monitor(nullptr)
{
	auto *monitor = glfwGetWindowMonitor(m_window);
	if(monitor != nullptr)
		m_monitor = std::make_unique<Monitor>(monitor);
#ifdef _WIN32
	InitFileDropHandler();
#endif
}

pragma::platform::WindowHandle pragma::platform::Window::GetHandle() { return m_handle; }

void pragma::platform::Window::Remove() { delete this; }

void pragma::platform::Window::KeyCallback(int key, int scancode, int action, int mods)
{
	if(m_callbackInterface.keyCallback != nullptr)
		m_callbackInterface.keyCallback(*this, static_cast<Key>(key), scancode, static_cast<KeyState>(action), static_cast<Modifier>(mods));
}

void pragma::platform::Window::RefreshCallback()
{
	if(m_callbackInterface.refreshCallback != nullptr)
		m_callbackInterface.refreshCallback(*this);
}

void pragma::platform::Window::ResizeCallback(int width, int height)
{
	if(m_callbackInterface.resizeCallback != nullptr)
		m_callbackInterface.resizeCallback(*this, Vector2i(width, height));
}

void pragma::platform::Window::CharCallback(unsigned int c)
{
	if(m_callbackInterface.charCallback != nullptr)
		m_callbackInterface.charCallback(*this, c);
}
void pragma::platform::Window::CharModsCallback(unsigned int c, int mods)
{
	if(m_callbackInterface.charModsCallback != nullptr)
		m_callbackInterface.charModsCallback(*this, c, static_cast<Modifier>(mods));
}
void pragma::platform::Window::CursorEnterCallback(int e)
{
	if(m_callbackInterface.cursorEnterCallback != nullptr)
		m_callbackInterface.cursorEnterCallback(*this, (e == GLFW_TRUE) ? true : false);
}
void pragma::platform::Window::CursorPosCallback(double x, double y)
{
	if(m_callbackInterface.cursorPosCallback != nullptr)
		m_callbackInterface.cursorPosCallback(*this, Vector2(x, y));
}
void pragma::platform::Window::DropCallback(int count, const char **paths)
{
	if(m_callbackInterface.dropCallback != nullptr) {
		std::vector<std::string> files;
		files.reserve(count);
		for(auto i = decltype(count) {0}; i < count; ++i)
			files.push_back(paths[i]);
		m_callbackInterface.dropCallback(*this, files);
	}
}
void pragma::platform::Window::DragEnterCallback()
{
	if(m_callbackInterface.dragEnterCallback != nullptr)
		m_callbackInterface.dragEnterCallback(*this);
}
void pragma::platform::Window::DragExitCallback()
{
	if(m_callbackInterface.dragExitCallback != nullptr)
		m_callbackInterface.dragExitCallback(*this);
}
void pragma::platform::Window::MouseButtonCallback(int button, int action, int mods)
{
	if(m_callbackInterface.mouseButtonCallback != nullptr)
		m_callbackInterface.mouseButtonCallback(*this, static_cast<MouseButton>(button), static_cast<KeyState>(action), static_cast<Modifier>(mods));
}
void pragma::platform::Window::ScrollCallback(double xoffset, double yoffset)
{
	if(m_callbackInterface.scrollCallback != nullptr)
		m_callbackInterface.scrollCallback(*this, Vector2(xoffset, yoffset));
}
void pragma::platform::Window::FocusCallback(int focused)
{
	if(m_callbackInterface.focusCallback != nullptr)
		m_callbackInterface.focusCallback(*this, (focused == GLFW_TRUE) ? true : false);
}
void pragma::platform::Window::IconifyCallback(int iconified)
{
	if(m_callbackInterface.iconifyCallback != nullptr)
		m_callbackInterface.iconifyCallback(*this, (iconified == GLFW_TRUE) ? true : false);
}
void pragma::platform::Window::WindowPosCallback(int x, int y)
{
	if(m_callbackInterface.windowPosCallback != nullptr)
		m_callbackInterface.windowPosCallback(*this, Vector2i(x, y));
}
void pragma::platform::Window::WindowSizeCallback(int w, int h)
{
	if(m_callbackInterface.windowSizeCallback != nullptr)
		m_callbackInterface.windowSizeCallback(*this, Vector2i(w, h));
}
void pragma::platform::Window::PreeditCallback(int preedit_count, unsigned int *preedit_string, int block_count, int *block_sizes, int focused_block, int caret)
{
	if(m_callbackInterface.preeditCallback != nullptr)
		m_callbackInterface.preeditCallback(*this, preedit_count, preedit_string, block_count, block_sizes, focused_block, caret);
}
void pragma::platform::Window::IMEStatusCallback()
{
	if(m_callbackInterface.imeStatusCallback != nullptr)
		m_callbackInterface.imeStatusCallback(*this);
}

const pragma::platform::Monitor *pragma::platform::Window::GetMonitor() const { return m_monitor.get(); }

const pragma::platform::WindowCreationInfo &pragma::platform::Window::GetCreationInfo() const { return m_creationInfo; }
pragma::platform::WindowCreationInfo::API pragma::platform::Window::GetAPI() const { return m_creationInfo.api; }

void pragma::platform::Window::MakeContextCurrent() const
{
	if(GetAPI() == WindowCreationInfo::API::None)
		return;
	glfwMakeContextCurrent(m_window);
	if(umath::is_flag_set(m_creationInfo.flags, WindowCreationInfo::Flags::DisableVSync))
		glfwSwapInterval(0);
	else
		glfwSwapInterval(1);
}

const GLFWwindow *pragma::platform::Window::GetGLFWWindow() const { return m_window; }

void pragma::platform::Window::SetKeyCallback(const std::function<void(Window &, Key, int, KeyState, Modifier)> &callback) { m_callbackInterface.keyCallback = callback; }
void pragma::platform::Window::SetRefreshCallback(const std::function<void(Window &)> &callback) { m_callbackInterface.refreshCallback = callback; }
void pragma::platform::Window::SetResizeCallback(const std::function<void(Window &, Vector2i)> &callback) { m_callbackInterface.resizeCallback = callback; }
void pragma::platform::Window::SetCharCallback(const std::function<void(Window &, unsigned int)> &callback) { m_callbackInterface.charCallback = callback; }
void pragma::platform::Window::SetCharModsCallback(const std::function<void(Window &, unsigned int, Modifier)> &callback) { m_callbackInterface.charModsCallback = callback; }
void pragma::platform::Window::SetCursorEnterCallback(const std::function<void(Window &, bool)> &callback) { m_callbackInterface.cursorEnterCallback = callback; }
void pragma::platform::Window::SetCursorPosCallback(const std::function<void(Window &, Vector2)> &callback) { m_callbackInterface.cursorPosCallback = callback; }
void pragma::platform::Window::SetDropCallback(const std::function<void(Window &, std::vector<std::string> &)> &callback) { m_callbackInterface.dropCallback = callback; }
void pragma::platform::Window::SetDragEnterCallback(const std::function<void(Window &)> &callback) { m_callbackInterface.dragEnterCallback = callback; }
void pragma::platform::Window::SetDragExitCallback(const std::function<void(Window &)> &callback) { m_callbackInterface.dragExitCallback = callback; }
void pragma::platform::Window::SetMouseButtonCallback(const std::function<void(Window &, MouseButton, KeyState, Modifier)> &callback) { m_callbackInterface.mouseButtonCallback = callback; }
void pragma::platform::Window::SetScrollCallback(const std::function<void(Window &, Vector2)> &callback) { m_callbackInterface.scrollCallback = callback; }
void pragma::platform::Window::SetCloseCallback(const std::function<void(Window &)> &callback) { m_callbackInterface.closeCallback = callback; }
void pragma::platform::Window::SetFocusCallback(const std::function<void(Window &, bool)> &callback) { m_callbackInterface.focusCallback = callback; }
void pragma::platform::Window::SetIconifyCallback(const std::function<void(Window &, bool)> &callback) { m_callbackInterface.iconifyCallback = callback; }
void pragma::platform::Window::SetWindowPosCallback(const std::function<void(Window &, Vector2i)> &callback) { m_callbackInterface.windowPosCallback = callback; }
void pragma::platform::Window::SetWindowSizeCallback(const std::function<void(Window &, Vector2i)> &callback) { m_callbackInterface.windowSizeCallback = callback; }
void pragma::platform::Window::SetPreeditCallback(const std::function<void(Window &, int, unsigned int *, int, int *, int, int)> &callback) { m_callbackInterface.preeditCallback = callback; }
void pragma::platform::Window::SetIMEStatusCallback(const std::function<void(Window &)> &callback) { m_callbackInterface.imeStatusCallback = callback; }
void pragma::platform::Window::SetOnShouldCloseCallback(const std::function<bool(Window &)> &callback) { m_callbackInterface.onShouldClose = callback; }
void pragma::platform::Window::SetCallbacks(const CallbackInterface &callbacks) { m_callbackInterface = callbacks; }
const pragma::platform::CallbackInterface &pragma::platform::Window::GetCallbacks() const { return m_callbackInterface; }

bool pragma::platform::Window::ShouldClose() const { return (glfwWindowShouldClose(const_cast<GLFWwindow *>(GetGLFWWindow())) == GLFW_TRUE) ? true : false; }
void pragma::platform::Window::SetShouldClose(bool b) { glfwSetWindowShouldClose(const_cast<GLFWwindow *>(GetGLFWWindow()), (b == true) ? GLFW_TRUE : GLFW_FALSE); }

pragma::platform::KeyState pragma::platform::Window::GetKeyState(Key key) { return static_cast<KeyState>(glfwGetKey(const_cast<GLFWwindow *>(GetGLFWWindow()), static_cast<uint32_t>(key))); }
pragma::platform::KeyState pragma::platform::Window::GetMouseButtonState(MouseButton button) { return static_cast<KeyState>(glfwGetMouseButton(const_cast<GLFWwindow *>(GetGLFWWindow()), static_cast<uint32_t>(button))); }

std::string pragma::platform::Window::GetClipboardString() const
{
	auto *str = glfwGetClipboardString(const_cast<GLFWwindow *>(GetGLFWWindow()));
	if(!str)
		return {};
	return str;
}
void pragma::platform::Window::SetClipboardString(const std::string &str) { glfwSetClipboardString(const_cast<GLFWwindow *>(GetGLFWWindow()), str.c_str()); }

Vector2 pragma::platform::Window::GetCursorPos() const
{
	if(m_cursorPosOverride.has_value())
		return *m_cursorPosOverride;
	double x = 0.0;
	double y = 0.0;
	glfwGetCursorPos(const_cast<GLFWwindow *>(GetGLFWWindow()), &x, &y);
	return Vector2(x, y);
}
void pragma::platform::Window::SetCursorPosOverride(const Vector2 &pos) { m_cursorPosOverride = pos; }
void pragma::platform::Window::ClearCursorPosOverride() { m_cursorPosOverride = {}; }
void pragma::platform::Window::SetCursorPos(const Vector2 &pos) { glfwSetCursorPos(const_cast<GLFWwindow *>(GetGLFWWindow()), pos.x, pos.y); }
void pragma::platform::Window::SetCursorInputMode(CursorMode mode) { return glfwSetInputMode(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_CURSOR, static_cast<int>(mode)); }
pragma::platform::CursorMode pragma::platform::Window::GetCursorInputMode() const { return static_cast<CursorMode>(glfwGetInputMode(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_CURSOR)); }
void pragma::platform::Window::SetStickyKeysEnabled(bool b) { return glfwSetInputMode(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_STICKY_KEYS, (b == true) ? GLFW_TRUE : GLFW_FALSE); }
bool pragma::platform::Window::GetStickyKeysEnabled() const { return (glfwGetInputMode(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_STICKY_KEYS) == GLFW_TRUE) ? true : false; }
void pragma::platform::Window::SetStickyMouseButtonsEnabled(bool b) { return glfwSetInputMode(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_STICKY_MOUSE_BUTTONS, (b == true) ? GLFW_TRUE : GLFW_FALSE); }
bool pragma::platform::Window::GetStickyMouseButtonsEnabled() const { return (glfwGetInputMode(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_STICKY_MOUSE_BUTTONS) == GLFW_TRUE) ? true : false; }
void pragma::platform::Window::SetPreeditCursorRectangle(int32_t x, int32_t y, int32_t w, int32_t h) { glfwSetPreeditCursorRectangle(const_cast<GLFWwindow *>(GetGLFWWindow()), x, y, w, h); }
void pragma::platform::Window::GetPreeditCursorRectangle(int32_t &outX, int32_t &outY, int32_t &outW, int32_t &outH) const { glfwGetPreeditCursorRectangle(const_cast<GLFWwindow *>(GetGLFWWindow()), &outX, &outY, &outW, &outH); }
void pragma::platform::Window::ResetPreeditText() { glfwResetPreeditText(const_cast<GLFWwindow *>(GetGLFWWindow())); }
void pragma::platform::Window::SetIMEEnabled(bool enabled) { return glfwSetInputMode(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_IME, enabled ? GLFW_TRUE : GLFW_FALSE); }
bool pragma::platform::Window::IsIMEEnabled() const { return (glfwGetInputMode(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_IME) == GLFW_TRUE) ? true : false; }

void pragma::platform::Window::SetVSyncEnabled(bool enabled)
{
	umath::set_flag(m_creationInfo.flags, WindowCreationInfo::Flags::DisableVSync, !enabled);
	glfwSwapInterval(enabled ? 1 : 0);
}
bool pragma::platform::Window::IsVSyncEnabled() const { return !umath::is_flag_set(m_creationInfo.flags, WindowCreationInfo::Flags::DisableVSync); }

void pragma::platform::Window::SwapBuffers() const { glfwSwapBuffers(const_cast<GLFWwindow *>(GetGLFWWindow())); }
void pragma::platform::Window::SetWindowTitle(const std::string &title)
{
	glfwSetWindowTitle(const_cast<GLFWwindow *>(GetGLFWWindow()), title.c_str());
	m_windowTitle = title;
}
const std::string &pragma::platform::Window::GetWindowTitle() const { return m_windowTitle; }
void pragma::platform::Window::SetWindowIcon(uint32_t width, uint32_t height, const uint8_t *data)
{
	GLFWimage iconData {};
	iconData.width = width;
	iconData.height = height;
	iconData.pixels = const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>(data));
	// Note: This won't work on Wayland, as Wayland does not support changing the window icon
	glfwSetWindowIcon(const_cast<GLFWwindow *>(GetGLFWWindow()), 1, &iconData);
	glfwGetError(nullptr); // Clear any errors that may have occurred
}

Vector2i pragma::platform::Window::GetPos() const
{
	int x = 0;
	int y = 0;
	glfwGetWindowPos(const_cast<GLFWwindow *>(GetGLFWWindow()), &x, &y);
	return Vector2i(x, y);
}

void pragma::platform::Window::SetBorderColor(const Color &color)
{
	m_borderColor = color;
#ifdef _WIN32

#if defined(WINVER) && (WINVER >= 0x0501)
	auto tmp = color;
	umath::swap(tmp.r, tmp.b);
	auto hex = tmp.ToHexColorRGB();
	COLORREF hexCol = ::util::to_hex_number("0x" + hex);
	const DWORD ATTR_BORDER_COLOR = 34; // See DWMWINDOWATTRIBUTE::DWMWA_BORDER_COLOR, can't use the enum because it may not be available and there's no way to check for it
	DwmSetWindowAttribute(GetWin32Handle(), ATTR_BORDER_COLOR, &hexCol, sizeof(hexCol));
#endif

#else
	// Not yet implemented
#endif
}
std::optional<Color> pragma::platform::Window::GetBorderColor() const { return m_borderColor; }
void pragma::platform::Window::SetTitleBarColor(const Color &color)
{
	m_titleBarColor = color;
#ifdef _WIN32

#if defined(WINVER) && (WINVER >= 0x0A00)
	auto tmp = color;
	umath::swap(tmp.r, tmp.b);
	auto hex = tmp.ToHexColorRGB();
	COLORREF hexCol = ::util::to_hex_number("0x" + hex);
	const DWORD ATTR_CAPTION_COLOR = 35; // See DWMWINDOWATTRIBUTE::DWMWA_CAPTION_COLOR, can't use the enum because it may not be available and there's no way to check for it
	DwmSetWindowAttribute(GetWin32Handle(), ATTR_CAPTION_COLOR, &hexCol, sizeof(hexCol));
#endif

#else
	// Not yet implemented
#endif
}
std::optional<Color> pragma::platform::Window::GetTitleBarColor() const { return m_titleBarColor; }

void pragma::platform::Window::SetPos(const Vector2i &pos) { glfwSetWindowPos(const_cast<GLFWwindow *>(GetGLFWWindow()), pos.x, pos.y); }
Vector2i pragma::platform::Window::GetSize() const
{
	int w = 0;
	int h = 0;
	glfwGetWindowSize(const_cast<GLFWwindow *>(GetGLFWWindow()), &w, &h);
	return Vector2i(w, h);
}
void pragma::platform::Window::SetSize(const Vector2i &size) { glfwSetWindowSize(const_cast<GLFWwindow *>(GetGLFWWindow()), size.x, size.y); }

void pragma::platform::Window::Poll()
{
#ifdef __linux__
	if(m_pendingWaylandDragAndDrop) {
		auto t = m_pendingWaylandDragAndDrop->t;
		auto t2 = std::chrono::steady_clock::now();
		if(t2 - t > std::chrono::milliseconds(100)) {
			std::vector<const char *> cpaths;
			cpaths.reserve(m_pendingWaylandDragAndDrop->files.size());
			for(auto &path : m_pendingWaylandDragAndDrop->files)
				cpaths.push_back(path.c_str());
			DropCallback(cpaths.size(), cpaths.data());
			DragExitCallback();
			m_pendingWaylandDragAndDrop = {};
		}
	}
#endif

	if(!m_shouldCloseInvoked) {
		if(ShouldClose()) {
			m_shouldCloseInvoked = true;
			if(m_callbackInterface.onShouldClose) {
				auto res = m_callbackInterface.onShouldClose(*this);
				if(res == false) {
					m_shouldCloseInvoked = false;
					SetShouldClose(false);
				}
			}
		}
	}
}
void pragma::platform::Window::UpdateWindow(const WindowCreationInfo &info)
{
	GLFWmonitor *monitor = nullptr;
	if(info.monitor)
		monitor = const_cast<GLFWmonitor *>(info.monitor->GetGLFWMonitor());
	// If the title bar is visible, we'll move the window down slightly
	auto yOffset = info.decorated ? 30 : 0;
	glfwSetWindowMonitor(m_window, monitor, 0, yOffset, info.width, info.height, GLFW_DONT_CARE);
	glfwSetWindowAttrib(m_window, GLFW_DECORATED, info.decorated ? GLFW_TRUE : GLFW_FALSE);
}

Vector2i pragma::platform::Window::GetFramebufferSize() const
{
	int w = 0;
	int h = 0;
	glfwGetFramebufferSize(const_cast<GLFWwindow *>(GetGLFWWindow()), &w, &h);
	return Vector2i(w, h);
}

Vector4i pragma::platform::Window::GetFrameSize() const
{
	int left = 0;
	int top = 0;
	int right = 0;
	int bottom = 0;
	glfwGetWindowFrameSize(const_cast<GLFWwindow *>(GetGLFWWindow()), &left, &top, &right, &bottom);
	return Vector4i(left, top, right, bottom);
}

void pragma::platform::Window::Iconify() const { glfwIconifyWindow(const_cast<GLFWwindow *>(GetGLFWWindow())); }
void pragma::platform::Window::Restore() const { glfwRestoreWindow(const_cast<GLFWwindow *>(GetGLFWWindow())); }
void pragma::platform::Window::Show() const { glfwShowWindow(const_cast<GLFWwindow *>(GetGLFWWindow())); }
void pragma::platform::Window::Hide() const { glfwHideWindow(const_cast<GLFWwindow *>(GetGLFWWindow())); }

void pragma::platform::Window::Maximize() { glfwMaximizeWindow(const_cast<GLFWwindow *>(GetGLFWWindow())); }
bool pragma::platform::Window::IsMaximized() const { return (glfwGetWindowAttrib(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_MAXIMIZED) != GLFW_FALSE) ? true : false; }
std::optional<pragma::platform::MonitorBounds> pragma::platform::Window::GetMonitorBounds() const
{
#ifdef _WIN32
	auto hWnd = GetWin32Handle();
	if(!hWnd)
		return {};
	auto monitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONEAREST);
	if(!monitor)
		return {};
	MONITORINFO info;
	info.cbSize = sizeof(MONITORINFO);
	if(!GetMonitorInfo(monitor, &info))
		return {};
	return MonitorBounds {Vector2 {info.rcMonitor.left, info.rcMonitor.top}, Vector2 {info.rcMonitor.right - info.rcMonitor.left, info.rcMonitor.bottom - info.rcMonitor.top},

	  Vector2 {info.rcWork.left, info.rcWork.top}, Vector2 {info.rcWork.right - info.rcWork.left, info.rcWork.bottom - info.rcWork.top}};
#else
	throw std::runtime_error {"Not yet implemented!"};
	return {};
#endif
}

bool pragma::platform::Window::IsFocused() const { return (glfwGetWindowAttrib(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_FOCUSED) != GLFW_FALSE) ? true : false; }
bool pragma::platform::Window::IsIconified() const { return (glfwGetWindowAttrib(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_ICONIFIED) != GLFW_FALSE) ? true : false; }
bool pragma::platform::Window::IsVisible() const { return (glfwGetWindowAttrib(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_VISIBLE) != GLFW_FALSE) ? true : false; }
bool pragma::platform::Window::IsResizable() const { return (glfwGetWindowAttrib(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_RESIZABLE) != GLFW_FALSE) ? true : false; }
bool pragma::platform::Window::IsDecorated() const { return (glfwGetWindowAttrib(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_DECORATED) != GLFW_FALSE) ? true : false; }
bool pragma::platform::Window::IsFloating() const { return (glfwGetWindowAttrib(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_FLOATING) != GLFW_FALSE) ? true : false; }
void pragma::platform::Window::SetResizable(bool resizable) { glfwSetWindowAttrib(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE); }

void pragma::platform::Window::SetCursor(const Cursor &cursor)
{
	auto *c = cursor.GetGLFWCursor();
	glfwSetCursor(const_cast<GLFWwindow *>(GetGLFWWindow()), const_cast<GLFWcursor *>(c));
}
void pragma::platform::Window::SetCursor(Cursor::Shape shape)
{
	SetCursor(Cursor::GetStandardCursor(shape));
}
void pragma::platform::Window::ClearCursor() { glfwSetCursor(const_cast<GLFWwindow *>(GetGLFWWindow()), nullptr); }

#ifdef _WIN32
HWND pragma::platform::Window::GetWin32Handle() const { return glfwGetWin32Window(const_cast<GLFWwindow *>(GetGLFWWindow())); }
HGLRC pragma::platform::Window::GetOpenGLContextHandle() const { return glfwGetWGLContext(const_cast<GLFWwindow *>(GetGLFWWindow())); }
#endif

static std::vector<pragma::platform::Window *> g_windows;
std::vector<pragma::platform::Window *> &pragma::platform::Window::GetWindows() { return g_windows; }

pragma::platform::Window::~Window()
{
#ifdef _WIN32
	ReleaseFileDropHandler();
#endif
	m_handle.Invalidate();
	glfwDestroyWindow(m_window);

	auto it = std::find(g_windows.begin(), g_windows.end(), this);
	assert(it != g_windows.end());
	if(it != g_windows.end())
		g_windows.erase(it);
}

void pragma::platform::Window::Reinitialize(const WindowCreationInfo &info)
{
	auto pos = GetPos();
	auto *w = const_cast<GLFWwindow *>(GetGLFWWindow());
	auto &ci = info;
	glfwSetWindowAttrib(w, GLFW_RESIZABLE, ci.resizable ? GLFW_TRUE : GLFW_FALSE);
	glfwSetWindowAttrib(w, GLFW_DECORATED, ci.decorated ? GLFW_TRUE : GLFW_FALSE);
	glfwSetWindowAttrib(w, GLFW_AUTO_ICONIFY, ci.autoIconify ? GLFW_TRUE : GLFW_FALSE);
	glfwSetWindowAttrib(w, GLFW_FLOATING, ci.floating ? GLFW_TRUE : GLFW_FALSE);

	SetWindowTitle(info.title);
	SetSize(Vector2i {info.width, info.height});
	GLFWmonitor *monitor = nullptr;
	if(info.monitor.has_value())
		monitor = const_cast<GLFWmonitor *>(info.monitor->GetGLFWMonitor());
	glfwSetWindowMonitor(w, monitor, 0, 0, ci.width, ci.height, ci.refreshRate);
	SetPos(pos);

	// Only update the properties that we could actually change
	m_creationInfo.resizable = info.resizable;
	m_creationInfo.decorated = info.decorated;
	m_creationInfo.autoIconify = info.autoIconify;
	m_creationInfo.floating = info.floating;
	m_creationInfo.width = info.width;
	m_creationInfo.height = info.height;
	m_creationInfo.title = info.title;
	m_creationInfo.monitor = info.monitor;
	m_creationInfo.refreshRate = info.refreshRate;
}

std::unique_ptr<pragma::platform::Window> pragma::platform::Window::Create(const WindowCreationInfo &info)
{
	if(pragma::platform::initialize() == false)
		throw std::runtime_error("Unable to create GLFW Window: GLFW hasn't been initialized!");
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_RESIZABLE, info.resizable);
	glfwWindowHint(GLFW_VISIBLE, info.visible);
	glfwWindowHint(GLFW_DECORATED, info.decorated);
	glfwWindowHint(GLFW_FOCUSED, info.focused);
	glfwWindowHint(GLFW_AUTO_ICONIFY, info.autoIconify);
	glfwWindowHint(GLFW_FLOATING, info.floating);
	glfwWindowHint(GLFW_STEREO, info.stereo);
	glfwWindowHint(GLFW_SRGB_CAPABLE, info.srgbCapable);
	glfwWindowHint(GLFW_DOUBLEBUFFER, info.doublebuffer);
	glfwWindowHint(GLFW_REFRESH_RATE, info.refreshRate);
	glfwWindowHint(GLFW_SAMPLES, info.samples);
	glfwWindowHint(GLFW_RED_BITS, info.redBits);
	glfwWindowHint(GLFW_GREEN_BITS, info.greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, info.blueBits);
	glfwWindowHint(GLFW_ALPHA_BITS, info.alphaBits);
	glfwWindowHint(GLFW_DEPTH_BITS, info.depthBits);
	glfwWindowHint(GLFW_STENCIL_BITS, info.stencilBits);

	auto api = GLFW_NO_API;
	auto isOpenGLAPI = false;
	switch(info.api) {
	case WindowCreationInfo::API::OpenGL:
		isOpenGLAPI = true;
		api = GLFW_OPENGL_API;
		break;
	case WindowCreationInfo::API::OpenGLES:
		isOpenGLAPI = true;
		api = GLFW_OPENGL_ES_API;
		break;
	}
	glfwWindowHint(GLFW_CLIENT_API, api);

	if(isOpenGLAPI && umath::is_flag_set(info.flags, WindowCreationInfo::Flags::DebugContext))
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);

	GLFWmonitor *monitor = nullptr;
	if(info.monitor.has_value())
		monitor = const_cast<GLFWmonitor *>(info.monitor->GetGLFWMonitor());
	glfwWindowHint(GLFW_VISIBLE, umath::is_flag_set(info.flags, WindowCreationInfo::Flags::Windowless) ? GLFW_FALSE : GLFW_TRUE);
	auto *sharedContextWindow = info.sharedContextWindow ? const_cast<GLFWwindow *>(info.sharedContextWindow->GetGLFWWindow()) : nullptr;
	auto *window = glfwCreateWindow(info.width, info.height, info.title.c_str(), monitor, sharedContextWindow);
	if(window == nullptr)
		throw std::runtime_error("Unable to create GLFW Window!");
	auto vkWindow = std::unique_ptr<Window>(new Window(window));
	glfwSetWindowRefreshCallback(window, [](GLFWwindow *window) {
		auto *vkWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->RefreshCallback();
	});
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
		auto *vkWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->RefreshCallback();
	});
	glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
		auto *vkWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->KeyCallback(key, scancode, action, mods);
	});
	glfwSetCharCallback(window, [](GLFWwindow *window, unsigned int c) {
		auto *vkWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->CharCallback(c);
	});
	glfwSetCharModsCallback(window, [](GLFWwindow *window, unsigned int c, int mods) {
		auto *vkWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->CharModsCallback(c, mods);
	});
	glfwSetCursorEnterCallback(window, [](GLFWwindow *window, int entered) {
		auto *vkWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->CursorEnterCallback(entered);
	});
	glfwSetCursorPosCallback(window, [](GLFWwindow *window, double x, double y) {
		auto *vkWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->CursorPosCallback(x, y);
	});
	glfwSetDropCallback(window, [](GLFWwindow *window, int count, const char **paths) {
		auto *vkWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
#ifdef __linux__
		auto platform = get_platform();
		if(platform == Platform::Wayland) {
			// The drag callback is currently not supported for wayland.
			// As a work-around, we just trigger DragEnterCallback and DragExitCallback
			// on drop, to make sure the respective code is still executed.
			// We also introduce a small delay to simulate a
			// real drag-and-drop interaction.
			if(!vkWindow->m_pendingWaylandDragAndDrop) {
				vkWindow->m_pendingWaylandDragAndDrop = std::unique_ptr<WaylandDragAndDropInfo> {new WaylandDragAndDropInfo {}};
				vkWindow->m_pendingWaylandDragAndDrop->t = std::chrono::steady_clock::now();
				vkWindow->DragEnterCallback();
			}
			auto &info = *vkWindow->m_pendingWaylandDragAndDrop;
			info.files.reserve(info.files.size() + count);
			for(size_t i = 0; i < count; ++i)
				info.files.push_back(paths[i]);
			return;
		}
#endif
		vkWindow->DropCallback(count, paths);
	});
	// glfwSetDragCallback is currently only implemented for Linux, and only for X11.
	// Windows drag support is located in file_drop_target.cppm
	// Wayland is currently not supported (see work-around above).
	// Once glfwSetDragCallback is officially implemented for all major platforms,
	// the windows implementation can be removed and the code below can be used.
	// See https://github.com/glfw/glfw/issues/1898
#ifdef __linux__
	glfwSetDragCallback(window, [](GLFWwindow *window, int entered) {
		auto *vkWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		if(entered == 1)
			vkWindow->DragEnterCallback();
		else
			vkWindow->DragExitCallback();
	});
#endif
	glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button, int action, int mods) {
		auto *vkWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->MouseButtonCallback(button, action, mods);
	});
	glfwSetScrollCallback(window, [](GLFWwindow *window, double x, double y) {
		auto *vkWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->ScrollCallback(x, y);
	});
	glfwSetWindowCloseCallback(window, [](GLFWwindow *window) {
		auto *vkWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr || vkWindow->m_callbackInterface.closeCallback == nullptr)
			return;
		vkWindow->m_callbackInterface.closeCallback(*vkWindow);
	});
	glfwSetWindowFocusCallback(window, [](GLFWwindow *window, int focused) {
		auto *vkWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->FocusCallback(focused);
	});
	glfwSetWindowIconifyCallback(window, [](GLFWwindow *window, int iconified) {
		auto *vkWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->IconifyCallback(iconified);
	});
	glfwSetWindowPosCallback(window, [](GLFWwindow *window, int x, int y) {
		auto *vkWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->WindowPosCallback(x, y);
	});
	glfwSetWindowSizeCallback(window, [](GLFWwindow *window, int w, int h) {
		auto *vkWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->WindowSizeCallback(w, h);
	});
	glfwSetPreeditCallback(window, [](GLFWwindow *window, int preedit_count, unsigned int *preedit_string, int block_count, int *block_sizes, int focused_block, int caret) {
		auto *vkWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->PreeditCallback(preedit_count, preedit_string, block_count, block_sizes, focused_block, caret);
	});
	glfwSetIMEStatusCallback(window, [](GLFWwindow *window) {
		auto *vkWindow = static_cast<Window *>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->IMEStatusCallback();
	});
	glfwSetWindowUserPointer(window, vkWindow.get());
	vkWindow->m_creationInfo = info;
	vkWindow->m_windowTitle = info.title;
	g_windows.push_back(vkWindow.get());
	return vkWindow;
}
