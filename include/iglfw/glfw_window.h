/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __GLFW_WINDOW_H__
#define __GLFW_WINDOW_H__

#include "glfw.h"
#include <sharedutils/def_handle.h>
#include <mathutil/uvec.h>
#include <memory>
#include <string>
#include <optional>
#include <functional>
#include "glfw_cursor.h"
#include "glfw_monitor.h"

#pragma warning(push)
#pragma warning(disable : 4251)
namespace GLFW
{
	class Window;
	DECLARE_BASE_HANDLE(DLLGLFW,Window,Window);

	struct DLLGLFW MonitorBounds
	{
		Vector2 monitorPos;
		Vector2 monitorSize;

		Vector2 workPos;
		Vector2 workSize;
	};

	struct DLLGLFW WindowCreationInfo
	{
		enum class API : uint8_t
		{
			None = 0,
			OpenGL,
			OpenGLES
		};
		enum class Flags : uint32_t
		{
			None = 0u,
			DebugContext = 1u,
			DisableVSync = DebugContext<<1u // OpenGL only
		};
		WindowCreationInfo();
		bool resizable;
		bool visible;
		bool decorated;
		bool focused;
		bool autoIconify;
		bool floating;
		bool stereo;
		bool srgbCapable;
		bool doublebuffer;
		int refreshRate;
		int samples;
		int redBits;
		int greenBits;
		int blueBits;
		int alphaBits;
		int depthBits;
		int stencilBits;
		std::string title;
		uint32_t width;
		uint32_t height;
		std::optional<Monitor> monitor {};
		API api = API::None;
		Flags flags = Flags::None;
	};

	struct DLLGLFW CallbackInterface
	{
		std::function<void(Window&,Key,int,KeyState,Modifier)> keyCallback = nullptr;
		std::function<void(Window&)> refreshCallback = nullptr;
		std::function<void(Window&,Vector2i)> resizeCallback = nullptr;
		std::function<void(Window&,unsigned int)> charCallback = nullptr;
		std::function<void(Window&,unsigned int,Modifier)> charModsCallback = nullptr;
		std::function<void(Window&,bool)> cursorEnterCallback = nullptr;
		std::function<void(Window&,Vector2)> cursorPosCallback = nullptr;
		std::function<void(Window&,std::vector<std::string>&)> dropCallback = nullptr;
		std::function<void(Window&,MouseButton,KeyState,Modifier)> mouseButtonCallback = nullptr;
		std::function<void(Window&,Vector2)> scrollCallback = nullptr;
		std::function<void(Window&)> closeCallback = nullptr;
		std::function<void(Window&,bool)> focusCallback = nullptr;
		std::function<void(Window&,bool)> iconifyCallback = nullptr;
		std::function<void(Window&,Vector2i)> windowPosCallback = nullptr;
		std::function<void(Window&,Vector2i)> windowSizeCallback = nullptr;
	};

	class DLLGLFW Window
	{
	public:
		static std::unique_ptr<Window> Create(const WindowCreationInfo &info);
		~Window();
		const GLFWwindow *GetGLFWWindow() const;
		WindowHandle GetHandle();
		void Remove();

		void SetKeyCallback(const std::function<void(Window&,Key,int,KeyState,Modifier)> &callback);
		void SetRefreshCallback(const std::function<void(Window&)> &callback);
		void SetResizeCallback(const std::function<void(Window&,Vector2i)> &callback);
		void SetCharCallback(const std::function<void(Window&,unsigned int)> &callback);
		void SetCharModsCallback(const std::function<void(Window&,unsigned int,Modifier)> &callback);
		void SetCursorEnterCallback(const std::function<void(Window&,bool)> &callback);
		void SetCursorPosCallback(const std::function<void(Window&,Vector2)> &callback);
		void SetDropCallback(const std::function<void(Window&,std::vector<std::string>&)> &callback);
		void SetMouseButtonCallback(const std::function<void(Window&,MouseButton,KeyState,Modifier)> &callback);
		void SetScrollCallback(const std::function<void(Window&,Vector2)> &callback);
		void SetCloseCallback(const std::function<void(Window&)> &callback);
		void SetFocusCallback(const std::function<void(Window&,bool)> &callback);
		void SetIconifyCallback(const std::function<void(Window&,bool)> &callback);
		void SetWindowPosCallback(const std::function<void(Window&,Vector2i)> &callback);
		void SetWindowSizeCallback(const std::function<void(Window&,Vector2i)> &callback);
		void SetCallbacks(const CallbackInterface &callbacks);
		const CallbackInterface &GetCallbacks() const;

		bool ShouldClose() const;
		void SetShouldClose(bool b);
		KeyState GetKeyState(Key key);
		KeyState GetMouseButtonState(MouseButton button);
		std::string GetClipboardString() const;
		void SetClipboardString(const std::string &str);
		Vector2 GetCursorPos() const;
		void SetCursorPosOverride(const Vector2 &pos);
		const std::optional<Vector2> &GetCursorPosOverride() const {return m_cursorPosOverride;}
		void ClearCursorPosOverride();
		void SetCursorPos(const Vector2 &pos);
		void SetCursorInputMode(CursorMode mode);
		CursorMode GetCursorInputMode() const;
		void SetStickyKeysEnabled(bool b);
		bool GetStickyKeysEnabled() const;
		void SetStickyMouseButtonsEnabled(bool b);
		bool GetStickyMouseButtonsEnabled() const;
		void SwapBuffers() const;
		void SetWindowTitle(const std::string &title);
		Vector2i GetPos() const;
		void SetPos(const Vector2i &pos);
		Vector2i GetSize() const;
		void SetSize(const Vector2i &size);
		Vector2i GetFramebufferSize() const;
		Vector4i GetFrameSize() const;
		void Iconify() const;
		void Restore() const;
		void Show() const;
		void Hide() const;
		void Maximize();
		bool IsMaximized() const;
		std::optional<MonitorBounds> GetMonitorBounds() const;
		const Monitor *GetMonitor() const;
		WindowCreationInfo::API GetAPI() const;
		void MakeContextCurrent() const;
		void UpdateWindow(const WindowCreationInfo &info);

#ifdef _WIN32
		HWND GetWin32Handle() const;
		HGLRC GetOpenGLContextHandle() const;
#endif

		// OpenGL only
		void SetVSyncEnabled(bool enabled);
		bool IsVSyncEnabled() const;

		bool IsFocused() const;
		bool IsIconified() const;
		bool IsVisible() const;
		bool IsResizable() const;
		bool IsDecorated() const;
		bool IsFloating() const;

		void SetCursor(const Cursor &cursor);
		void ClearCursor();
	private:
		Window(GLFWwindow *window);
		GLFWwindow *m_window;
		std::unique_ptr<Monitor> m_monitor;
		WindowHandle m_handle;
		WindowCreationInfo::API m_api = WindowCreationInfo::API::None;
		WindowCreationInfo::Flags m_flags = WindowCreationInfo::Flags::None;
		CallbackInterface m_callbackInterface {};
		std::optional<Vector2> m_cursorPosOverride = {};
		void KeyCallback(int key,int scancode,int action,int mods);
		void RefreshCallback();
		void ResizeCallback(int width,int height);
		void CharCallback(unsigned int c);
		void CharModsCallback(unsigned int c,int mods);
		void CursorEnterCallback(int e);
		void CursorPosCallback(double x,double y);
		void DropCallback(int count,const char **paths);
		void MouseButtonCallback(int button,int action,int mods);
		void ScrollCallback(double xoffset,double yoffset);
		void FocusCallback(int focused);
		void IconifyCallback(int iconified);
		void WindowPosCallback(int x,int y);
		void WindowSizeCallback(int w,int h);
	};
};
REGISTER_BASIC_BITWISE_OPERATORS(GLFW::WindowCreationInfo::Flags)
#pragma warning(pop)

#endif
