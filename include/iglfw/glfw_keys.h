/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef __GLFW_KEYS_H__
#define __GLFW_KEYS_H__

#include "glfw_includes.h"
#include <stdint.h>
#include <mathutil/umath.h>

namespace GLFW
{
	enum class DLLGLFW Key : int32_t
	{
		Unknown = GLFW_KEY_UNKNOWN,
		Space = GLFW_KEY_SPACE,
		Apostrophe = GLFW_KEY_APOSTROPHE,
		Comma = GLFW_KEY_COMMA,
		Minus = GLFW_KEY_MINUS,
		Period = GLFW_KEY_PERIOD,
		Slash = GLFW_KEY_SLASH,
		N0 = GLFW_KEY_0,
		N1 = GLFW_KEY_1,
		N2 = GLFW_KEY_2,
		N3 = GLFW_KEY_3,
		N4 = GLFW_KEY_4,
		N5 = GLFW_KEY_5,
		N6 = GLFW_KEY_6,
		N7 = GLFW_KEY_7,
		N8 = GLFW_KEY_8,
		N9 = GLFW_KEY_9,
		Semicolon = GLFW_KEY_SEMICOLON,
		Equal = GLFW_KEY_EQUAL,
		A = GLFW_KEY_A,
		B = GLFW_KEY_B,
		C = GLFW_KEY_C,
		D = GLFW_KEY_D,
		E = GLFW_KEY_E,
		F = GLFW_KEY_F,
		G = GLFW_KEY_G,
		H = GLFW_KEY_H,
		I = GLFW_KEY_I,
		J = GLFW_KEY_J,
		K = GLFW_KEY_K,
		L = GLFW_KEY_L,
		M = GLFW_KEY_M,
		N = GLFW_KEY_N,
		O = GLFW_KEY_O,
		P = GLFW_KEY_P,
		Q = GLFW_KEY_Q,
		R = GLFW_KEY_R,
		S = GLFW_KEY_S,
		T = GLFW_KEY_T,
		U = GLFW_KEY_U,
		V = GLFW_KEY_V,
		W = GLFW_KEY_W,
		X = GLFW_KEY_X,
		Y = GLFW_KEY_Y,
		Z = GLFW_KEY_Z,
		LeftBracket = GLFW_KEY_LEFT_BRACKET,
		Backslash = GLFW_KEY_BACKSLASH,
		RightBracket = GLFW_KEY_RIGHT_BRACKET,
		GraveAccent = GLFW_KEY_GRAVE_ACCENT,
		World1 = GLFW_KEY_WORLD_1,
		World2 = GLFW_KEY_WORLD_2,
		Escape = GLFW_KEY_ESCAPE,
		Enter = GLFW_KEY_ENTER,
		Tab = GLFW_KEY_TAB,
		Backspace = GLFW_KEY_BACKSPACE,
		Insert = GLFW_KEY_INSERT,
		Delete = GLFW_KEY_DELETE,
		Right = GLFW_KEY_RIGHT,
		Left = GLFW_KEY_LEFT,
		Down = GLFW_KEY_DOWN,
		Up = GLFW_KEY_UP,
		PageUp = GLFW_KEY_PAGE_UP,
		PageDown = GLFW_KEY_PAGE_DOWN,
		Home = GLFW_KEY_HOME,
		End = GLFW_KEY_END,
		CapsLock = GLFW_KEY_CAPS_LOCK,
		ScrollLock = GLFW_KEY_SCROLL_LOCK,
		NumLock = GLFW_KEY_NUM_LOCK,
		PrintScreen = GLFW_KEY_PRINT_SCREEN,
		Pause = GLFW_KEY_PAUSE,
		F1 = GLFW_KEY_F1,
		F2 = GLFW_KEY_F2,
		F3 = GLFW_KEY_F3,
		F4 = GLFW_KEY_F4,
		F5 = GLFW_KEY_F5,
		F6 = GLFW_KEY_F6,
		F7 = GLFW_KEY_F7,
		F8 = GLFW_KEY_F8,
		F9 = GLFW_KEY_F9,
		F10 = GLFW_KEY_F10,
		F11 = GLFW_KEY_F11,
		F12 = GLFW_KEY_F12,
		F13 = GLFW_KEY_F13,
		F14 = GLFW_KEY_F14,
		F15 = GLFW_KEY_F15,
		F16 = GLFW_KEY_F16,
		F17 = GLFW_KEY_F17,
		F18 = GLFW_KEY_F18,
		F19 = GLFW_KEY_F19,
		F20 = GLFW_KEY_F20,
		F21 = GLFW_KEY_F21,
		F22 = GLFW_KEY_F22,
		F23 = GLFW_KEY_F23,
		F24 = GLFW_KEY_F24,
		F25 = GLFW_KEY_F25,
		Kp0 = GLFW_KEY_KP_0,
		Kp1 = GLFW_KEY_KP_1,
		Kp2 = GLFW_KEY_KP_2,
		Kp3 = GLFW_KEY_KP_3,
		Kp4 = GLFW_KEY_KP_4,
		Kp5 = GLFW_KEY_KP_5,
		Kp6 = GLFW_KEY_KP_6,
		Kp7 = GLFW_KEY_KP_7,
		Kp8 = GLFW_KEY_KP_8,
		Kp9 = GLFW_KEY_KP_9,
		KpDecimal = GLFW_KEY_KP_DECIMAL,
		KpDivide = GLFW_KEY_KP_DIVIDE,
		KpMultiply = GLFW_KEY_KP_MULTIPLY,
		KpSubtract = GLFW_KEY_KP_SUBTRACT,
		KpAdd = GLFW_KEY_KP_ADD,
		KpEnter = GLFW_KEY_KP_ENTER,
		KpEqual = GLFW_KEY_KP_EQUAL,
		LeftShift = GLFW_KEY_LEFT_SHIFT,
		LeftControl = GLFW_KEY_LEFT_CONTROL,
		LeftAlt = GLFW_KEY_LEFT_ALT,
		LeftSuper = GLFW_KEY_LEFT_SUPER,
		RightShift = GLFW_KEY_RIGHT_SHIFT,
		RightControl = GLFW_KEY_RIGHT_CONTROL,
		RightAlt = GLFW_KEY_RIGHT_ALT,
		RightSuper = GLFW_KEY_RIGHT_SUPER,
		Menu = GLFW_KEY_MENU,
		Last = GLFW_KEY_LAST
	};
	REGISTER_BASIC_ARITHMETIC_OPERATORS(Key);

	enum class DLLGLFW MouseButton : uint32_t
	{
		N1 = GLFW_MOUSE_BUTTON_1,
		N2 = GLFW_MOUSE_BUTTON_2,
		N3 = GLFW_MOUSE_BUTTON_3,
		N4 = GLFW_MOUSE_BUTTON_4,
		N5 = GLFW_MOUSE_BUTTON_5,
		N6 = GLFW_MOUSE_BUTTON_6,
		N7 = GLFW_MOUSE_BUTTON_7,
		N8 = GLFW_MOUSE_BUTTON_8,
		Last = GLFW_MOUSE_BUTTON_LAST,
		Left = GLFW_MOUSE_BUTTON_LEFT,
		Right = GLFW_MOUSE_BUTTON_RIGHT,
		Middle = GLFW_MOUSE_BUTTON_MIDDLE
	};
	REGISTER_BASIC_ARITHMETIC_OPERATORS(MouseButton);

	enum class DLLGLFW KeyState : uint32_t
	{
		Invalid = std::numeric_limits<uint32_t>::max(),
		Press = GLFW_PRESS,
		Release = GLFW_RELEASE,
		Repeat = GLFW_REPEAT,
		Held = Repeat +1
	};
	REGISTER_BASIC_ARITHMETIC_OPERATORS(KeyState);

	enum class DLLGLFW Modifier : uint32_t
	{
		None = 0,
		Shift = GLFW_MOD_SHIFT,
		Control = GLFW_MOD_CONTROL,
		Alt = GLFW_MOD_ALT,
		Super = GLFW_MOD_SUPER,
		AxisInput = Super<<1,
		AxisPress = AxisInput<<1,
		AxisRelease = AxisPress<<1,
		AxisNegative = AxisRelease<<1
	};
	REGISTER_BASIC_ARITHMETIC_OPERATORS(Modifier);

	enum class DLLGLFW InputMode : uint32_t
	{
		Cursor = GLFW_CURSOR,
		StickyKeys = GLFW_STICKY_KEYS,
		StickyMouseButtons = GLFW_STICKY_MOUSE_BUTTONS
	};
	REGISTER_BASIC_ARITHMETIC_OPERATORS(InputMode);
}

#endif
