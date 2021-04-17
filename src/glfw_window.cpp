/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "iglfw/glfw_window.h"

#define GLFW_EXPOSE_NATIVE_WGL
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

using namespace GLFW;

DEFINE_BASE_HANDLE(,Window,Window);

WindowCreationInfo::WindowCreationInfo()
	: resizable(true),visible(true),
	decorated(true),focused(true),
	autoIconify(true),floating(false),
	stereo(false),srgbCapable(false),
	doublebuffer(true),refreshRate(GLFW_DONT_CARE),
	samples(0),redBits(8),
	greenBits(8),blueBits(8),
	alphaBits(8),depthBits(24),
	stencilBits(8),width(800),
	height(600),monitor(nullptr)
{}

Window::Window(GLFWwindow *window)
	: m_handle(new PtrWindow(this)),m_window(window),
	m_monitor(nullptr)
{
	auto *monitor = glfwGetWindowMonitor(m_window);
	if(monitor != nullptr)
		m_monitor = std::make_unique<Monitor>(monitor);
}

Window::~Window()
{
	m_handle.Invalidate();
	glfwDestroyWindow(m_window);
}

WindowHandle Window::GetHandle() {return m_handle;}

void Window::Remove() {delete this;}

void Window::KeyCallback(int key,int scancode,int action,int mods)
{
	if(m_callbackInterface.keyCallback != nullptr)
		m_callbackInterface.keyCallback(*this,static_cast<Key>(key),scancode,static_cast<KeyState>(action),static_cast<Modifier>(mods));
}

void Window::RefreshCallback()
{
	if(m_callbackInterface.refreshCallback != nullptr)
		m_callbackInterface.refreshCallback(*this);
}

void Window::ResizeCallback(int width,int height)
{
	if(m_callbackInterface.resizeCallback != nullptr)
		m_callbackInterface.resizeCallback(*this,Vector2i(width,height));
}

void Window::CharCallback(unsigned int c)
{
	if(m_callbackInterface.charCallback != nullptr)
		m_callbackInterface.charCallback(*this,c);
}
void Window::CharModsCallback(unsigned int c,int mods)
{
	if(m_callbackInterface.charModsCallback != nullptr)
		m_callbackInterface.charModsCallback(*this,c,static_cast<Modifier>(mods));
}
void Window::CursorEnterCallback(int e)
{
	if(m_callbackInterface.cursorEnterCallback != nullptr)
		m_callbackInterface.cursorEnterCallback(*this,(e == GLFW_TRUE) ? true : false);
}
void Window::CursorPosCallback(double x,double y)
{
	if(m_callbackInterface.cursorPosCallback != nullptr)
		m_callbackInterface.cursorPosCallback(*this,Vector2(x,y));
}
void Window::DropCallback(int count,const char **paths)
{
	if(m_callbackInterface.dropCallback != nullptr)
	{
		std::vector<std::string> files;
		files.reserve(count);
		for(auto i=decltype(count){0};i<count;++i)
			files.push_back(paths[i]);
		m_callbackInterface.dropCallback(*this,files);
	}
}
void Window::MouseButtonCallback(int button,int action,int mods)
{
	if(m_callbackInterface.mouseButtonCallback != nullptr)
		m_callbackInterface.mouseButtonCallback(*this,static_cast<MouseButton>(button),static_cast<KeyState>(action),static_cast<Modifier>(mods));
}
void Window::ScrollCallback(double xoffset,double yoffset)
{
	if(m_callbackInterface.scrollCallback != nullptr)
		m_callbackInterface.scrollCallback(*this,Vector2(xoffset,yoffset));
}
void Window::FocusCallback(int focused)
{
	if(m_callbackInterface.focusCallback != nullptr)
		m_callbackInterface.focusCallback(*this,(focused == GLFW_TRUE) ? true : false);
}
void Window::IconifyCallback(int iconified)
{
	if(m_callbackInterface.iconifyCallback != nullptr)
		m_callbackInterface.iconifyCallback(*this,(iconified == GLFW_TRUE) ? true : false);
}
void Window::WindowPosCallback(int x,int y)
{
	if(m_callbackInterface.windowPosCallback != nullptr)
		m_callbackInterface.windowPosCallback(*this,Vector2i(x,y));
}
void Window::WindowSizeCallback(int w,int h)
{
	if(m_callbackInterface.windowSizeCallback != nullptr)
		m_callbackInterface.windowSizeCallback(*this,Vector2i(w,h));
}

const Monitor *Window::GetMonitor() const
{
	return m_monitor.get();
}

WindowCreationInfo::API Window::GetAPI() const {return m_api;}

void Window::MakeContextCurrent() const
{
	if(GetAPI() == WindowCreationInfo::API::None)
		return;
	glfwMakeContextCurrent(m_window);
	if(umath::is_flag_set(m_flags,WindowCreationInfo::Flags::DisableVSync))
		glfwSwapInterval(0);
	else
		glfwSwapInterval(1);
}

const GLFWwindow *Window::GetGLFWWindow() const {return m_window;}

void Window::SetKeyCallback(const std::function<void(Window&,Key,int,KeyState,Modifier)> &callback) {m_callbackInterface.keyCallback = callback;}
void Window::SetRefreshCallback(const std::function<void(Window&)> &callback) {m_callbackInterface.refreshCallback = callback;}
void Window::SetResizeCallback(const std::function<void(Window&,Vector2i)> &callback) {m_callbackInterface.resizeCallback = callback;}
void Window::SetCharCallback(const std::function<void(Window&,unsigned int)> &callback) {m_callbackInterface.charCallback = callback;}
void Window::SetCharModsCallback(const std::function<void(Window&,unsigned int,Modifier)> &callback) {m_callbackInterface.charModsCallback = callback;}
void Window::SetCursorEnterCallback(const std::function<void(Window&,bool)> &callback) {m_callbackInterface.cursorEnterCallback = callback;}
void Window::SetCursorPosCallback(const std::function<void(Window&,Vector2)> &callback) {m_callbackInterface.cursorPosCallback = callback;}
void Window::SetDropCallback(const std::function<void(Window&,std::vector<std::string>&)> &callback) {m_callbackInterface.dropCallback = callback;}
void Window::SetMouseButtonCallback(const std::function<void(Window&,MouseButton,KeyState,Modifier)> &callback) {m_callbackInterface.mouseButtonCallback = callback;}
void Window::SetScrollCallback(const std::function<void(Window&,Vector2)> &callback) {m_callbackInterface.scrollCallback = callback;}
void Window::SetCloseCallback(const std::function<void(Window&)> &callback) {m_callbackInterface.closeCallback = callback;}
void Window::SetFocusCallback(const std::function<void(Window&,bool)> &callback) {m_callbackInterface.focusCallback = callback;}
void Window::SetIconifyCallback(const std::function<void(Window&,bool)> &callback) {m_callbackInterface.iconifyCallback = callback;}
void Window::SetWindowPosCallback(const std::function<void(Window&,Vector2i)> &callback) {m_callbackInterface.windowPosCallback = callback;}
void Window::SetWindowSizeCallback(const std::function<void(Window&,Vector2i)> &callback) {m_callbackInterface.windowSizeCallback = callback;}
void Window::SetCallbacks(const CallbackInterface &callbacks) {m_callbackInterface = callbacks;}
const CallbackInterface &Window::GetCallbacks() const {return m_callbackInterface;}

bool Window::ShouldClose() const {return (glfwWindowShouldClose(const_cast<GLFWwindow*>(GetGLFWWindow())) == GLFW_TRUE) ? true : false;}
void Window::SetShouldClose(bool b) {glfwSetWindowShouldClose(const_cast<GLFWwindow*>(GetGLFWWindow()),(b == true) ? GLFW_TRUE : GLFW_FALSE);}

KeyState Window::GetKeyState(Key key) {return static_cast<KeyState>(glfwGetKey(const_cast<GLFWwindow*>(GetGLFWWindow()),static_cast<uint32_t>(key)));}
KeyState Window::GetMouseButtonState(MouseButton button) {return static_cast<KeyState>(glfwGetMouseButton(const_cast<GLFWwindow*>(GetGLFWWindow()),static_cast<uint32_t>(button)));}

std::string Window::GetClipboardString() const {return glfwGetClipboardString(const_cast<GLFWwindow*>(GetGLFWWindow()));}
void Window::SetClipboardString(const std::string &str) {glfwSetClipboardString(const_cast<GLFWwindow*>(GetGLFWWindow()),str.c_str());}

Vector2 Window::GetCursorPos() const
{
	if(m_cursorPosOverride.has_value())
		return *m_cursorPosOverride;
	double x = 0.0;
	double y = 0.0;
	glfwGetCursorPos(const_cast<GLFWwindow*>(GetGLFWWindow()),&x,&y);
	return Vector2(x,y);
}
void Window::SetCursorPosOverride(const Vector2 &pos) {m_cursorPosOverride = pos;}
void Window::ClearCursorPosOverride() {m_cursorPosOverride = {};}
void Window::SetCursorPos(const Vector2 &pos) {glfwSetCursorPos(const_cast<GLFWwindow*>(GetGLFWWindow()),pos.x,pos.y);}
void Window::SetCursorInputMode(CursorMode mode) {return glfwSetInputMode(const_cast<GLFWwindow*>(GetGLFWWindow()),GLFW_CURSOR,static_cast<int>(mode));}
CursorMode Window::GetCursorInputMode() const {return static_cast<CursorMode>(glfwGetInputMode(const_cast<GLFWwindow*>(GetGLFWWindow()),GLFW_CURSOR));}
void Window::SetStickyKeysEnabled(bool b) {return glfwSetInputMode(const_cast<GLFWwindow*>(GetGLFWWindow()),GLFW_STICKY_KEYS,(b == true) ? GLFW_TRUE : GLFW_FALSE);}
bool Window::GetStickyKeysEnabled() const {return (glfwGetInputMode(const_cast<GLFWwindow*>(GetGLFWWindow()),GLFW_STICKY_KEYS) == GLFW_TRUE) ? true : false;}
void Window::SetStickyMouseButtonsEnabled(bool b) {return glfwSetInputMode(const_cast<GLFWwindow*>(GetGLFWWindow()),GLFW_STICKY_MOUSE_BUTTONS,(b == true) ? GLFW_TRUE : GLFW_FALSE);}
bool Window::GetStickyMouseButtonsEnabled() const {return (glfwGetInputMode(const_cast<GLFWwindow*>(GetGLFWWindow()),GLFW_STICKY_MOUSE_BUTTONS) == GLFW_TRUE) ? true : false;}

void Window::SetVSyncEnabled(bool enabled)
{
	umath::set_flag(m_flags,WindowCreationInfo::Flags::DisableVSync,!enabled);
	glfwSwapInterval(enabled ? 1 : 0);
}
bool Window::IsVSyncEnabled() const {return !umath::is_flag_set(m_flags,WindowCreationInfo::Flags::DisableVSync);}

void Window::SwapBuffers() const {glfwSwapBuffers(const_cast<GLFWwindow*>(GetGLFWWindow()));}
void Window::SetWindowTitle(const std::string &title) {glfwSetWindowTitle(const_cast<GLFWwindow*>(GetGLFWWindow()),title.c_str());}

Vector2i Window::GetPos() const
{
	int x = 0;
	int y = 0;
	glfwGetWindowPos(const_cast<GLFWwindow*>(GetGLFWWindow()),&x,&y);
	return Vector2i(x,y);
}
void Window::SetPos(const Vector2i &pos) {glfwSetWindowPos(const_cast<GLFWwindow*>(GetGLFWWindow()),pos.x,pos.y);}
Vector2i Window::GetSize() const
{
	int w = 0;
	int h = 0;
	glfwGetWindowSize(const_cast<GLFWwindow*>(GetGLFWWindow()),&w,&h);
	return Vector2i(w,h);
}
void Window::SetSize(const Vector2i &size) {glfwSetWindowSize(const_cast<GLFWwindow*>(GetGLFWWindow()),size.x,size.y);}

void Window::UpdateWindow(const WindowCreationInfo &info)
{
	GLFWmonitor *monitor = nullptr;
	if(info.monitor)
		monitor = const_cast<GLFWmonitor*>(info.monitor->GetGLFWMonitor());
	// If the title bar is visible, we'll move the window down slightly
	auto yOffset = info.decorated ? 30 : 0;
	glfwSetWindowMonitor(m_window,monitor,0,yOffset,info.width,info.height,GLFW_DONT_CARE);
	glfwSetWindowAttrib(m_window,GLFW_DECORATED,info.decorated ? GLFW_TRUE : GLFW_FALSE);
}

Vector2i Window::GetFramebufferSize() const
{
	int w = 0;
	int h = 0;
	glfwGetFramebufferSize(const_cast<GLFWwindow*>(GetGLFWWindow()),&w,&h);
	return Vector2i(w,h);
}

Vector4i Window::GetFrameSize() const
{
	int left = 0;
	int top = 0;
	int right = 0;
	int bottom = 0;
	glfwGetWindowFrameSize(const_cast<GLFWwindow*>(GetGLFWWindow()),&left,&top,&right,&bottom);
	return Vector4i(left,top,right,bottom);
}

void Window::Iconify() const {glfwIconifyWindow(const_cast<GLFWwindow*>(GetGLFWWindow()));}
void Window::Restore() const {glfwRestoreWindow(const_cast<GLFWwindow*>(GetGLFWWindow()));}
void Window::Show() const {glfwShowWindow(const_cast<GLFWwindow*>(GetGLFWWindow()));}
void Window::Hide() const {glfwHideWindow(const_cast<GLFWwindow*>(GetGLFWWindow()));}

bool Window::IsFocused() const {return (glfwGetWindowAttrib(const_cast<GLFWwindow*>(GetGLFWWindow()),GLFW_FOCUSED) != GLFW_FALSE) ? true : false;}
bool Window::IsIconified() const {return (glfwGetWindowAttrib(const_cast<GLFWwindow*>(GetGLFWWindow()),GLFW_ICONIFIED) != GLFW_FALSE) ? true : false;}
bool Window::IsVisible() const {return (glfwGetWindowAttrib(const_cast<GLFWwindow*>(GetGLFWWindow()),GLFW_VISIBLE) != GLFW_FALSE) ? true : false;}
bool Window::IsResizable() const {return (glfwGetWindowAttrib(const_cast<GLFWwindow*>(GetGLFWWindow()),GLFW_RESIZABLE) != GLFW_FALSE) ? true : false;}
bool Window::IsDecorated() const {return (glfwGetWindowAttrib(const_cast<GLFWwindow*>(GetGLFWWindow()),GLFW_DECORATED) != GLFW_FALSE) ? true : false;}
bool Window::IsFloating() const {return (glfwGetWindowAttrib(const_cast<GLFWwindow*>(GetGLFWWindow()),GLFW_FLOATING) != GLFW_FALSE) ? true : false;}

void Window::SetCursor(const Cursor &cursor)
{
	auto *c = cursor.GetGLFWCursor();
	glfwSetCursor(const_cast<GLFWwindow*>(GetGLFWWindow()),const_cast<GLFWcursor*>(c));
}
void Window::ClearCursor() {glfwSetCursor(const_cast<GLFWwindow*>(GetGLFWWindow()),nullptr);}

#ifdef _WIN32
HWND Window::GetWin32Handle() const
{
	return glfwGetWin32Window(const_cast<GLFWwindow*>(GetGLFWWindow()));
}
#endif
HGLRC Window::GetOpenGLContextHandle() const
{
	return glfwGetWGLContext(const_cast<GLFWwindow*>(GetGLFWWindow()));
}

std::unique_ptr<Window> Window::Create(const WindowCreationInfo &info)
{
	if(GLFW::initialize() == false)
		throw std::runtime_error("Unable to create GLFW Window: GLFW hasn't been initialized!");
	glfwDefaultWindowHints();
	glfwWindowHint(GLFW_RESIZABLE,info.resizable);
	glfwWindowHint(GLFW_VISIBLE,info.visible);
	glfwWindowHint(GLFW_DECORATED,info.decorated);
	glfwWindowHint(GLFW_FOCUSED,info.focused);
	glfwWindowHint(GLFW_AUTO_ICONIFY,info.autoIconify);
	glfwWindowHint(GLFW_FLOATING,info.floating);
	glfwWindowHint(GLFW_STEREO,info.stereo);
	glfwWindowHint(GLFW_SRGB_CAPABLE,info.srgbCapable);
	glfwWindowHint(GLFW_DOUBLEBUFFER,info.doublebuffer);
	glfwWindowHint(GLFW_REFRESH_RATE,info.refreshRate);
	glfwWindowHint(GLFW_SAMPLES,info.samples);
	glfwWindowHint(GLFW_RED_BITS,info.redBits);
	glfwWindowHint(GLFW_GREEN_BITS,info.greenBits);
	glfwWindowHint(GLFW_BLUE_BITS,info.blueBits);
	glfwWindowHint(GLFW_ALPHA_BITS,info.alphaBits);
	glfwWindowHint(GLFW_DEPTH_BITS,info.depthBits);
	glfwWindowHint(GLFW_STENCIL_BITS,info.stencilBits);

	auto api = GLFW_NO_API;
	auto isOpenGLAPI = false;
	switch(info.api)
	{
	case WindowCreationInfo::API::OpenGL:
		isOpenGLAPI = true;
		api = GLFW_OPENGL_API;
		break;
	case WindowCreationInfo::API::OpenGLES:
		isOpenGLAPI = true;
		api = GLFW_OPENGL_ES_API;
		break;
	}
	glfwWindowHint(GLFW_CLIENT_API,api);

	if(isOpenGLAPI && umath::is_flag_set(info.flags,WindowCreationInfo::Flags::DebugContext))
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,GLFW_TRUE);

	GLFWmonitor *monitor = nullptr;
	if(info.monitor.has_value())
		monitor = const_cast<GLFWmonitor*>(info.monitor->GetGLFWMonitor());
	auto *window = glfwCreateWindow(info.width,info.height,info.title.c_str(),monitor,nullptr);
	if(window == nullptr)
		throw std::runtime_error("Unable to create GLFW Window!");
	auto vkWindow = std::unique_ptr<Window>(new Window(window));
	glfwSetWindowRefreshCallback(window,[](GLFWwindow *window) {
		auto *vkWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->RefreshCallback();
	});
	glfwSetFramebufferSizeCallback(window,[](GLFWwindow *window,int width,int height) {
		auto *vkWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->RefreshCallback();
	});
	glfwSetKeyCallback(window,[](GLFWwindow *window,int key,int scancode,int action,int mods) {
		auto *vkWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->KeyCallback(key,scancode,action,mods);
	});
	glfwSetCharCallback(window,[](GLFWwindow *window,unsigned int c) {
		auto *vkWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->CharCallback(c);
	});
	glfwSetCharModsCallback(window,[](GLFWwindow *window,unsigned int c,int mods) {
		auto *vkWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->CharModsCallback(c,mods);
	});
	glfwSetCursorEnterCallback(window,[](GLFWwindow *window,int entered) {
		auto *vkWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->CursorEnterCallback(entered);
	});
	glfwSetCursorPosCallback(window,[](GLFWwindow *window,double x,double y) {
		auto *vkWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->CursorPosCallback(x,y);
	});
	glfwSetDropCallback(window,[](GLFWwindow *window,int count,const char **paths) {
		auto *vkWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->DropCallback(count,paths);
	});
	glfwSetMouseButtonCallback(window,[](GLFWwindow *window,int button,int action,int mods) {
		auto *vkWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->MouseButtonCallback(button,action,mods);
	});
	glfwSetScrollCallback(window,[](GLFWwindow *window,double x,double y) {
		auto *vkWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->ScrollCallback(x,y);
	});
	glfwSetWindowCloseCallback(window,[](GLFWwindow *window) {
		auto *vkWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr || vkWindow->m_callbackInterface.closeCallback == nullptr)
			return;
		vkWindow->m_callbackInterface.closeCallback(*vkWindow);
	});
	glfwSetWindowFocusCallback(window,[](GLFWwindow *window,int focused) {
		auto *vkWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->FocusCallback(focused);
	});
	glfwSetWindowIconifyCallback(window,[](GLFWwindow *window,int iconified) {
		auto *vkWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->IconifyCallback(iconified);
	});
	glfwSetWindowPosCallback(window,[](GLFWwindow *window,int x,int y) {
		auto *vkWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->WindowPosCallback(x,y);
	});
	glfwSetWindowSizeCallback(window,[](GLFWwindow *window,int w,int h) {
		auto *vkWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if(vkWindow == nullptr)
			return;
		vkWindow->WindowSizeCallback(w,h);
	});
	glfwSetWindowUserPointer(window,vkWindow.get());
	vkWindow->m_api = info.api;
	vkWindow->m_flags = info.flags;
	return vkWindow;
}
