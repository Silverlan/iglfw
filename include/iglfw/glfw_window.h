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

	struct DLLGLFW WindowCreationInfo
	{
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
		std::unique_ptr<Monitor> monitor;
	};
	class DLLGLFW Window
	{
	private:
		Window(GLFWwindow *window);
		GLFWwindow *m_window;
		std::unique_ptr<Monitor> m_monitor;
		WindowHandle m_handle;
		std::function<void(Window&,Key,int,KeyState,Modifier)> m_keyCallback;
		std::function<void(Window&)> m_refreshCallback;
		std::function<void(Window&,Vector2i)> m_resizeCallback;
		std::function<void(Window&,unsigned int)> m_charCallback;
		std::function<void(Window&,unsigned int,Modifier)> m_charModsCallback;
		std::function<void(Window&,bool)> m_cursorEnterCallback;
		std::function<void(Window&,Vector2)> m_cursorPosCallback;
		std::function<void(Window&,std::vector<std::string>&)> m_dropCallback;
		std::function<void(Window&,MouseButton,KeyState,Modifier)> m_mouseButtonCallback;
		std::function<void(Window&,Vector2)> m_scrollCallback;
		std::function<void(Window&)> m_closeCallback;
		std::function<void(Window&,bool)> m_focusCallback;
		std::function<void(Window&,bool)> m_iconifyCallback;
		std::function<void(Window&,Vector2i)> m_windowPosCallback;
		std::function<void(Window&,Vector2i)> m_windowSizeCallback;
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
		bool ShouldClose() const;
		void SetShouldClose(bool b);
		KeyState GetKeyState(Key key);
		KeyState GetMouseButtonState(MouseButton button);
		std::string GetClipboardString() const;
		void SetClipboardString(const std::string &str);
		Vector2 GetCursorPos() const;
		void SetCursorPosOverride(const Vector2 &pos);
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
		const Monitor *GetMonitor() const;

		bool IsFocused() const;
		bool IsIconified() const;
		bool IsVisible() const;
		bool IsResizable() const;
		bool IsDecorated() const;
		bool IsFloating() const;

		void SetCursor(const Cursor &cursor);
		void ClearCursor();
	};
};
#pragma warning(pop)

#endif
