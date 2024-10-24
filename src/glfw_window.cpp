/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "iglfw/glfw_window.h"

#ifdef _WIN32

#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32

#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")

#else
#define GLFW_EXPOSE_NATIVE_X11
#endif
#include <GLFW/glfw3native.h>
#include <mathutil/color.h>
#include <sharedutils/util.h>
#include <algorithm>
#include <cassert>
#include "file_drop_target.hpp"

#undef API
#undef None

namespace GLFW {
	DEFINE_BASE_HANDLE(, GLFW::Window, Window);
};

GLFW::WindowCreationInfo::WindowCreationInfo()
    : resizable(true), visible(true), decorated(true), focused(true), autoIconify(true), floating(false), stereo(false), srgbCapable(false), doublebuffer(true), refreshRate(GLFW_DONT_CARE), samples(0), redBits(8), greenBits(8), blueBits(8), alphaBits(8), depthBits(24), stencilBits(8),
      width(800), height(600), monitor(nullptr)
{
}

GLFW::Window::Window(GLFWwindow *window) : m_handle(new PtrWindow(this)), m_window(window), m_monitor(nullptr)
{
	auto *monitor = glfwGetWindowMonitor(m_window);
	if(monitor != nullptr)
		m_monitor = std::make_unique<Monitor>(monitor);
#ifdef _WIN32
	InitFileDropHandler();
#endif
}

GLFW::WindowHandle GLFW::Window::GetHandle() { return m_handle; }

void GLFW::Window::Remove() { delete this; }

void GLFW::Window::KeyCallback(int key, int scancode, int action, int mods)
{
	if(m_callbackInterface.keyCallback != nullptr)
		m_callbackInterface.keyCallback(*this, static_cast<Key>(key), scancode, static_cast<KeyState>(action), static_cast<Modifier>(mods));
}

void GLFW::Window::RefreshCallback()
{
	if(m_callbackInterface.refreshCallback != nullptr)
		m_callbackInterface.refreshCallback(*this);
}

void GLFW::Window::ResizeCallback(int width, int height)
{
	if(m_callbackInterface.resizeCallback != nullptr)
		m_callbackInterface.resizeCallback(*this, Vector2i(width, height));
}

void GLFW::Window::CharCallback(unsigned int c)
{
	if(m_callbackInterface.charCallback != nullptr)
		m_callbackInterface.charCallback(*this, c);
}
void GLFW::Window::CharModsCallback(unsigned int c, int mods)
{
	if(m_callbackInterface.charModsCallback != nullptr)
		m_callbackInterface.charModsCallback(*this, c, static_cast<Modifier>(mods));
}
void GLFW::Window::CursorEnterCallback(int e)
{
	if(m_callbackInterface.cursorEnterCallback != nullptr)
		m_callbackInterface.cursorEnterCallback(*this, (e == GLFW_TRUE) ? true : false);
}
void GLFW::Window::CursorPosCallback(double x, double y)
{
	if(m_callbackInterface.cursorPosCallback != nullptr)
		m_callbackInterface.cursorPosCallback(*this, Vector2(x, y));
}
void GLFW::Window::DropCallback(int count, const char **paths)
{
	if(m_callbackInterface.dropCallback != nullptr) {
		std::vector<std::string> files;
		files.reserve(count);
		for(auto i = decltype(count) {0}; i < count; ++i)
			files.push_back(paths[i]);
		m_callbackInterface.dropCallback(*this, files);
	}
}
void GLFW::Window::DragEnterCallback()
{
	if(m_callbackInterface.dragEnterCallback != nullptr)
		m_callbackInterface.dragEnterCallback(*this);
}
void GLFW::Window::DragExitCallback()
{
	if(m_callbackInterface.dragExitCallback != nullptr)
		m_callbackInterface.dragExitCallback(*this);
}
void GLFW::Window::MouseButtonCallback(int button, int action, int mods)
{
	if(m_callbackInterface.mouseButtonCallback != nullptr)
		m_callbackInterface.mouseButtonCallback(*this, static_cast<MouseButton>(button), static_cast<KeyState>(action), static_cast<Modifier>(mods));
}
void GLFW::Window::ScrollCallback(double xoffset, double yoffset)
{
	if(m_callbackInterface.scrollCallback != nullptr)
		m_callbackInterface.scrollCallback(*this, Vector2(xoffset, yoffset));
}
void GLFW::Window::FocusCallback(int focused)
{
	if(m_callbackInterface.focusCallback != nullptr)
		m_callbackInterface.focusCallback(*this, (focused == GLFW_TRUE) ? true : false);
}
void GLFW::Window::IconifyCallback(int iconified)
{
	if(m_callbackInterface.iconifyCallback != nullptr)
		m_callbackInterface.iconifyCallback(*this, (iconified == GLFW_TRUE) ? true : false);
}
void GLFW::Window::WindowPosCallback(int x, int y)
{
	if(m_callbackInterface.windowPosCallback != nullptr)
		m_callbackInterface.windowPosCallback(*this, Vector2i(x, y));
}
void GLFW::Window::WindowSizeCallback(int w, int h)
{
	if(m_callbackInterface.windowSizeCallback != nullptr)
		m_callbackInterface.windowSizeCallback(*this, Vector2i(w, h));
}
void GLFW::Window::PreeditCallback(int preedit_count, unsigned int *preedit_string, int block_count, int *block_sizes, int focused_block, int caret)
{
	if(m_callbackInterface.preeditCallback != nullptr)
		m_callbackInterface.preeditCallback(*this, preedit_count, preedit_string, block_count, block_sizes, focused_block, caret);
}
void GLFW::Window::IMEStatusCallback()
{
	if(m_callbackInterface.imeStatusCallback != nullptr)
		m_callbackInterface.imeStatusCallback(*this);
}

const GLFW::Monitor *GLFW::Window::GetMonitor() const { return m_monitor.get(); }

const GLFW::WindowCreationInfo &GLFW::Window::GetCreationInfo() const { return m_creationInfo; }
GLFW::WindowCreationInfo::API GLFW::Window::GetAPI() const { return m_creationInfo.api; }

void GLFW::Window::MakeContextCurrent() const
{
	if(GetAPI() == WindowCreationInfo::API::None)
		return;
	glfwMakeContextCurrent(m_window);
	if(umath::is_flag_set(m_creationInfo.flags, WindowCreationInfo::Flags::DisableVSync))
		glfwSwapInterval(0);
	else
		glfwSwapInterval(1);
}

const GLFWwindow *GLFW::Window::GetGLFWWindow() const { return m_window; }

void GLFW::Window::SetKeyCallback(const std::function<void(Window &, Key, int, KeyState, Modifier)> &callback) { m_callbackInterface.keyCallback = callback; }
void GLFW::Window::SetRefreshCallback(const std::function<void(Window &)> &callback) { m_callbackInterface.refreshCallback = callback; }
void GLFW::Window::SetResizeCallback(const std::function<void(Window &, Vector2i)> &callback) { m_callbackInterface.resizeCallback = callback; }
void GLFW::Window::SetCharCallback(const std::function<void(Window &, unsigned int)> &callback) { m_callbackInterface.charCallback = callback; }
void GLFW::Window::SetCharModsCallback(const std::function<void(Window &, unsigned int, Modifier)> &callback) { m_callbackInterface.charModsCallback = callback; }
void GLFW::Window::SetCursorEnterCallback(const std::function<void(Window &, bool)> &callback) { m_callbackInterface.cursorEnterCallback = callback; }
void GLFW::Window::SetCursorPosCallback(const std::function<void(Window &, Vector2)> &callback) { m_callbackInterface.cursorPosCallback = callback; }
void GLFW::Window::SetDropCallback(const std::function<void(Window &, std::vector<std::string> &)> &callback) { m_callbackInterface.dropCallback = callback; }
void GLFW::Window::SetDragEnterCallback(const std::function<void(Window &)> &callback) { m_callbackInterface.dragEnterCallback = callback; }
void GLFW::Window::SetDragExitCallback(const std::function<void(Window &)> &callback) { m_callbackInterface.dragExitCallback = callback; }
void GLFW::Window::SetMouseButtonCallback(const std::function<void(Window &, MouseButton, KeyState, Modifier)> &callback) { m_callbackInterface.mouseButtonCallback = callback; }
void GLFW::Window::SetScrollCallback(const std::function<void(Window &, Vector2)> &callback) { m_callbackInterface.scrollCallback = callback; }
void GLFW::Window::SetCloseCallback(const std::function<void(Window &)> &callback) { m_callbackInterface.closeCallback = callback; }
void GLFW::Window::SetFocusCallback(const std::function<void(Window &, bool)> &callback) { m_callbackInterface.focusCallback = callback; }
void GLFW::Window::SetIconifyCallback(const std::function<void(Window &, bool)> &callback) { m_callbackInterface.iconifyCallback = callback; }
void GLFW::Window::SetWindowPosCallback(const std::function<void(Window &, Vector2i)> &callback) { m_callbackInterface.windowPosCallback = callback; }
void GLFW::Window::SetWindowSizeCallback(const std::function<void(Window &, Vector2i)> &callback) { m_callbackInterface.windowSizeCallback = callback; }
void GLFW::Window::SetPreeditCallback(const std::function<void(Window &, int, unsigned int *, int, int *, int, int)> &callback) { m_callbackInterface.preeditCallback = callback; }
void GLFW::Window::SetIMEStatusCallback(const std::function<void(Window &)> &callback) { m_callbackInterface.imeStatusCallback = callback; }
void GLFW::Window::SetOnShouldCloseCallback(const std::function<bool(Window &)> &callback) { m_callbackInterface.onShouldClose = callback; }
void GLFW::Window::SetCallbacks(const CallbackInterface &callbacks) { m_callbackInterface = callbacks; }
const GLFW::CallbackInterface &GLFW::Window::GetCallbacks() const { return m_callbackInterface; }

bool GLFW::Window::ShouldClose() const { return (glfwWindowShouldClose(const_cast<GLFWwindow *>(GetGLFWWindow())) == GLFW_TRUE) ? true : false; }
void GLFW::Window::SetShouldClose(bool b) { glfwSetWindowShouldClose(const_cast<GLFWwindow *>(GetGLFWWindow()), (b == true) ? GLFW_TRUE : GLFW_FALSE); }

GLFW::KeyState GLFW::Window::GetKeyState(Key key) { return static_cast<KeyState>(glfwGetKey(const_cast<GLFWwindow *>(GetGLFWWindow()), static_cast<uint32_t>(key))); }
GLFW::KeyState GLFW::Window::GetMouseButtonState(MouseButton button) { return static_cast<KeyState>(glfwGetMouseButton(const_cast<GLFWwindow *>(GetGLFWWindow()), static_cast<uint32_t>(button))); }

std::string GLFW::Window::GetClipboardString() const
{
	auto *str = glfwGetClipboardString(const_cast<GLFWwindow *>(GetGLFWWindow()));
	if(!str)
		return {};
	return str;
}
void GLFW::Window::SetClipboardString(const std::string &str) { glfwSetClipboardString(const_cast<GLFWwindow *>(GetGLFWWindow()), str.c_str()); }

Vector2 GLFW::Window::GetCursorPos() const
{
	if(m_cursorPosOverride.has_value())
		return *m_cursorPosOverride;
	double x = 0.0;
	double y = 0.0;
	glfwGetCursorPos(const_cast<GLFWwindow *>(GetGLFWWindow()), &x, &y);
	return Vector2(x, y);
}
void GLFW::Window::SetCursorPosOverride(const Vector2 &pos) { m_cursorPosOverride = pos; }
void GLFW::Window::ClearCursorPosOverride() { m_cursorPosOverride = {}; }
void GLFW::Window::SetCursorPos(const Vector2 &pos) { glfwSetCursorPos(const_cast<GLFWwindow *>(GetGLFWWindow()), pos.x, pos.y); }
void GLFW::Window::SetCursorInputMode(CursorMode mode) { return glfwSetInputMode(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_CURSOR, static_cast<int>(mode)); }
GLFW::CursorMode GLFW::Window::GetCursorInputMode() const { return static_cast<CursorMode>(glfwGetInputMode(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_CURSOR)); }
void GLFW::Window::SetStickyKeysEnabled(bool b) { return glfwSetInputMode(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_STICKY_KEYS, (b == true) ? GLFW_TRUE : GLFW_FALSE); }
bool GLFW::Window::GetStickyKeysEnabled() const { return (glfwGetInputMode(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_STICKY_KEYS) == GLFW_TRUE) ? true : false; }
void GLFW::Window::SetStickyMouseButtonsEnabled(bool b) { return glfwSetInputMode(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_STICKY_MOUSE_BUTTONS, (b == true) ? GLFW_TRUE : GLFW_FALSE); }
bool GLFW::Window::GetStickyMouseButtonsEnabled() const { return (glfwGetInputMode(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_STICKY_MOUSE_BUTTONS) == GLFW_TRUE) ? true : false; }
void GLFW::Window::SetPreeditCursorRectangle(int32_t x, int32_t y, int32_t w, int32_t h) { glfwSetPreeditCursorRectangle(const_cast<GLFWwindow *>(GetGLFWWindow()), x, y, w, h); }
void GLFW::Window::GetPreeditCursorRectangle(int32_t &outX, int32_t &outY, int32_t &outW, int32_t &outH) const { glfwGetPreeditCursorRectangle(const_cast<GLFWwindow *>(GetGLFWWindow()), &outX, &outY, &outW, &outH); }
void GLFW::Window::ResetPreeditText() { glfwResetPreeditText(const_cast<GLFWwindow *>(GetGLFWWindow())); }
void GLFW::Window::SetIMEEnabled(bool enabled) { return glfwSetInputMode(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_IME, enabled ? GLFW_TRUE : GLFW_FALSE); }
bool GLFW::Window::IsIMEEnabled() const { return (glfwGetInputMode(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_IME) == GLFW_TRUE) ? true : false; }

void GLFW::Window::SetVSyncEnabled(bool enabled)
{
	umath::set_flag(m_creationInfo.flags, WindowCreationInfo::Flags::DisableVSync, !enabled);
	glfwSwapInterval(enabled ? 1 : 0);
}
bool GLFW::Window::IsVSyncEnabled() const { return !umath::is_flag_set(m_creationInfo.flags, WindowCreationInfo::Flags::DisableVSync); }

void GLFW::Window::SwapBuffers() const { glfwSwapBuffers(const_cast<GLFWwindow *>(GetGLFWWindow())); }
void GLFW::Window::SetWindowTitle(const std::string &title)
{
	glfwSetWindowTitle(const_cast<GLFWwindow *>(GetGLFWWindow()), title.c_str());
	m_windowTitle = title;
}
const std::string &GLFW::Window::GetWindowTitle() const { return m_windowTitle; }
void GLFW::Window::SetWindowIcon(uint32_t width, uint32_t height, const uint8_t *data)
{
	GLFWimage iconData {};
	iconData.width = width;
	iconData.height = height;
	iconData.pixels = const_cast<unsigned char *>(reinterpret_cast<const unsigned char *>(data));
	glfwSetWindowIcon(const_cast<GLFWwindow *>(GetGLFWWindow()), 1, &iconData);
}

Vector2i GLFW::Window::GetPos() const
{
	int x = 0;
	int y = 0;
	glfwGetWindowPos(const_cast<GLFWwindow *>(GetGLFWWindow()), &x, &y);
	return Vector2i(x, y);
}

void GLFW::Window::SetBorderColor(const Color &color)
{
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
void GLFW::Window::SetTitleBarColor(const Color &color)
{
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

void GLFW::Window::SetPos(const Vector2i &pos) { glfwSetWindowPos(const_cast<GLFWwindow *>(GetGLFWWindow()), pos.x, pos.y); }
Vector2i GLFW::Window::GetSize() const
{
	int w = 0;
	int h = 0;
	glfwGetWindowSize(const_cast<GLFWwindow *>(GetGLFWWindow()), &w, &h);
	return Vector2i(w, h);
}
void GLFW::Window::SetSize(const Vector2i &size) { glfwSetWindowSize(const_cast<GLFWwindow *>(GetGLFWWindow()), size.x, size.y); }

void GLFW::Window::Poll()
{
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
void GLFW::Window::UpdateWindow(const WindowCreationInfo &info)
{
	GLFWmonitor *monitor = nullptr;
	if(info.monitor)
		monitor = const_cast<GLFWmonitor *>(info.monitor->GetGLFWMonitor());
	// If the title bar is visible, we'll move the window down slightly
	auto yOffset = info.decorated ? 30 : 0;
	glfwSetWindowMonitor(m_window, monitor, 0, yOffset, info.width, info.height, GLFW_DONT_CARE);
	glfwSetWindowAttrib(m_window, GLFW_DECORATED, info.decorated ? GLFW_TRUE : GLFW_FALSE);
}

Vector2i GLFW::Window::GetFramebufferSize() const
{
	int w = 0;
	int h = 0;
	glfwGetFramebufferSize(const_cast<GLFWwindow *>(GetGLFWWindow()), &w, &h);
	return Vector2i(w, h);
}

Vector4i GLFW::Window::GetFrameSize() const
{
	int left = 0;
	int top = 0;
	int right = 0;
	int bottom = 0;
	glfwGetWindowFrameSize(const_cast<GLFWwindow *>(GetGLFWWindow()), &left, &top, &right, &bottom);
	return Vector4i(left, top, right, bottom);
}

void GLFW::Window::Iconify() const { glfwIconifyWindow(const_cast<GLFWwindow *>(GetGLFWWindow())); }
void GLFW::Window::Restore() const { glfwRestoreWindow(const_cast<GLFWwindow *>(GetGLFWWindow())); }
void GLFW::Window::Show() const { glfwShowWindow(const_cast<GLFWwindow *>(GetGLFWWindow())); }
void GLFW::Window::Hide() const { glfwHideWindow(const_cast<GLFWwindow *>(GetGLFWWindow())); }

void GLFW::Window::Maximize() { glfwMaximizeWindow(const_cast<GLFWwindow *>(GetGLFWWindow())); }
bool GLFW::Window::IsMaximized() const { return (glfwGetWindowAttrib(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_MAXIMIZED) != GLFW_FALSE) ? true : false; }
std::optional<GLFW::MonitorBounds> GLFW::Window::GetMonitorBounds() const
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

bool GLFW::Window::IsFocused() const { return (glfwGetWindowAttrib(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_FOCUSED) != GLFW_FALSE) ? true : false; }
bool GLFW::Window::IsIconified() const { return (glfwGetWindowAttrib(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_ICONIFIED) != GLFW_FALSE) ? true : false; }
bool GLFW::Window::IsVisible() const { return (glfwGetWindowAttrib(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_VISIBLE) != GLFW_FALSE) ? true : false; }
bool GLFW::Window::IsResizable() const { return (glfwGetWindowAttrib(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_RESIZABLE) != GLFW_FALSE) ? true : false; }
bool GLFW::Window::IsDecorated() const { return (glfwGetWindowAttrib(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_DECORATED) != GLFW_FALSE) ? true : false; }
bool GLFW::Window::IsFloating() const { return (glfwGetWindowAttrib(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_FLOATING) != GLFW_FALSE) ? true : false; }
void GLFW::Window::SetResizable(bool resizable) { glfwSetWindowAttrib(const_cast<GLFWwindow *>(GetGLFWWindow()), GLFW_RESIZABLE, resizable ? GLFW_TRUE : GLFW_FALSE); }

void GLFW::Window::SetCursor(const Cursor &cursor)
{
	auto *c = cursor.GetGLFWCursor();
	glfwSetCursor(const_cast<GLFWwindow *>(GetGLFWWindow()), const_cast<GLFWcursor *>(c));
}
void GLFW::Window::ClearCursor() { glfwSetCursor(const_cast<GLFWwindow *>(GetGLFWWindow()), nullptr); }

#ifdef _WIN32
HWND GLFW::Window::GetWin32Handle() const { return glfwGetWin32Window(const_cast<GLFWwindow *>(GetGLFWWindow())); }
HGLRC GLFW::Window::GetOpenGLContextHandle() const { return glfwGetWGLContext(const_cast<GLFWwindow *>(GetGLFWWindow())); }
#endif

static std::vector<GLFW::Window *> g_windows;
std::vector<GLFW::Window *> &GLFW::Window::GetWindows() { return g_windows; }

GLFW::Window::~Window()
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

void GLFW::Window::Reinitialize(const WindowCreationInfo &info)
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

std::unique_ptr<GLFW::Window> GLFW::Window::Create(const WindowCreationInfo &info)
{
	if(GLFW::initialize() == false)
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
		vkWindow->DropCallback(count, paths);
	});
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
