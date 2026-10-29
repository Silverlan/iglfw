// SPDX-FileCopyrightText: (c) 2025 Silverlan <opensource@pragma-engine.com>
// SPDX-License-Identifier: MIT

module;

#include "includes.hpp"
#include "util_enum_flags.hpp"

// For system key defines
#if __linux__
#include <linux/input-event-codes.h>
#else
#include <Windows.h>
#endif

export module pragma.platform:keys;

import pragma.math;

#undef max

export {
	namespace pragma::platform {
		enum class Key : int32_t {
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

		enum class MouseButton : uint32_t {
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

		enum class KeyState : uint32_t { Invalid = std::numeric_limits<uint32_t>::max(), Press = GLFW_PRESS, Release = GLFW_RELEASE, Repeat = GLFW_REPEAT, Held = Repeat + 1 };
		enum class Modifier : uint32_t { None = 0, Shift = GLFW_MOD_SHIFT, Control = GLFW_MOD_CONTROL, Alt = GLFW_MOD_ALT, Super = GLFW_MOD_SUPER, AxisInput = Super << 1, AxisPress = AxisInput << 1, AxisRelease = AxisPress << 1, AxisNegative = AxisRelease << 1 };
		enum class InputMode : uint32_t { Cursor = GLFW_CURSOR, StickyKeys = GLFW_STICKY_KEYS, StickyMouseButtons = GLFW_STICKY_MOUSE_BUTTONS };

		constexpr int32_t to_system_key(Key key)
		{
			int32_t systemKey = -1;
			switch(key) {
#ifdef _WIN32
			case Key::Escape:
				systemKey = VK_ESCAPE;
				break;
			case Key::Enter:
				systemKey = VK_RETURN;
				break;
			case Key::Tab:
				systemKey = VK_TAB;
				break;
			case Key::Backspace:
				systemKey = VK_BACK;
				break;
			case Key::Insert:
				systemKey = VK_INSERT;
				break;
			case Key::Delete:
				systemKey = VK_DELETE;
				break;
			case Key::Right:
				systemKey = VK_RIGHT;
				break;
			case Key::Left:
				systemKey = VK_LEFT;
				break;
			case Key::Down:
				systemKey = VK_DOWN;
				break;
			case Key::Up:
				systemKey = VK_UP;
				break;
			case Key::PageUp:
				systemKey = VK_PRIOR;
				break;
			case Key::PageDown:
				systemKey = VK_NEXT;
				break;
			case Key::Home:
				systemKey = VK_HOME;
				break;
			case Key::End:
				systemKey = VK_END;
				break;
			case Key::CapsLock:
				systemKey = VK_CAPITAL;
				break;
			case Key::ScrollLock:
				systemKey = VK_SCROLL;
				break;
			case Key::NumLock:
				systemKey = VK_NUMLOCK;
				break;
			case Key::PrintScreen:
				systemKey = VK_PRINT;
				break;
			case Key::Pause:
				systemKey = VK_PAUSE;
				break;
			case Key::F1:
				systemKey = VK_F1;
				break;
			case Key::F2:
				systemKey = VK_F2;
				break;
			case Key::F3:
				systemKey = VK_F3;
				break;
			case Key::F4:
				systemKey = VK_F4;
				break;
			case Key::F5:
				systemKey = VK_F5;
				break;
			case Key::F6:
				systemKey = VK_F6;
				break;
			case Key::F7:
				systemKey = VK_F7;
				break;
			case Key::F8:
				systemKey = VK_F8;
				break;
			case Key::F9:
				systemKey = VK_F9;
				break;
			case Key::F10:
				systemKey = VK_F10;
				break;
			case Key::F11:
				systemKey = VK_F11;
				break;
			case Key::F12:
				systemKey = VK_F12;
				break;
			case Key::F13:
				systemKey = VK_F13;
				break;
			case Key::F14:
				systemKey = VK_F14;
				break;
			case Key::F15:
				systemKey = VK_F15;
				break;
			case Key::F16:
				systemKey = VK_F16;
				break;
			case Key::F17:
				systemKey = VK_F17;
				break;
			case Key::F18:
				systemKey = VK_F18;
				break;
			case Key::F19:
				systemKey = VK_F19;
				break;
			case Key::F20:
				systemKey = VK_F20;
				break;
			case Key::F21:
				systemKey = VK_F21;
				break;
			case Key::F22:
				systemKey = VK_F22;
				break;
			case Key::F23:
				systemKey = VK_F23;
				break;
			case Key::F24:
				systemKey = VK_F24;
				break;
				//case Key::F25:
				//	systemKey = VK_F25;
				//	break;
			case Key::Kp0:
				systemKey = VK_NUMPAD0;
				break;
			case Key::Kp1:
				systemKey = VK_NUMPAD1;
				break;
			case Key::Kp2:
				systemKey = VK_NUMPAD2;
				break;
			case Key::Kp3:
				systemKey = VK_NUMPAD3;
				break;
			case Key::Kp4:
				systemKey = VK_NUMPAD4;
				break;
			case Key::Kp5:
				systemKey = VK_NUMPAD5;
				break;
			case Key::Kp6:
				systemKey = VK_NUMPAD6;
				break;
			case Key::Kp7:
				systemKey = VK_NUMPAD7;
				break;
			case Key::Kp8:
				systemKey = VK_NUMPAD8;
				break;
			case Key::Kp9:
				systemKey = VK_NUMPAD9;
				break;
			case Key::KpDecimal:
				systemKey = VK_DECIMAL;
				break;
			case Key::KpDivide:
				systemKey = VK_DIVIDE;
				break;
			case Key::KpMultiply:
				systemKey = VK_MULTIPLY;
				break;
			case Key::KpSubtract:
				systemKey = VK_SUBTRACT;
				break;
			case Key::KpAdd:
				systemKey = VK_ADD;
				break;
			case Key::KpEnter:
				systemKey = VK_RETURN;
				break;
				//case Key::KpEqual:
				//	systemKey = VK_KP_EQUAL;
				//	break;
			case Key::LeftShift:
				systemKey = VK_LSHIFT;
				break;
			case Key::LeftControl:
				systemKey = VK_LCONTROL;
				break;
			case Key::LeftAlt:
				systemKey = VK_MENU;
				break;
			case Key::LeftSuper:
				systemKey = VK_LWIN;
				break;
			case Key::RightShift:
				systemKey = VK_LSHIFT;
				break;
			case Key::RightControl:
				systemKey = VK_LCONTROL;
				break;
			case Key::RightAlt:
				systemKey = VK_MENU;
				break;
			case Key::RightSuper:
				systemKey = VK_RWIN;
				break;
			case Key::Menu:
				systemKey = VK_MENU;
				break;
#else
				// Linux Equivalent
			case Key::Escape:
				systemKey = KEY_ESC;
				break;
			case Key::Enter:
				systemKey = KEY_ENTER;
				break;
			case Key::Tab:
				systemKey = KEY_TAB;
				break;
			case Key::Backspace:
				systemKey = KEY_BACKSPACE;
				break;
			case Key::Insert:
				systemKey = KEY_INSERT;
				break;
			case Key::Delete:
				systemKey = KEY_DELETE;
				break;
			case Key::Right:
				systemKey = KEY_RIGHT;
				break;
			case Key::Left:
				systemKey = KEY_LEFT;
				break;
			case Key::Down:
				systemKey = KEY_DOWN;
				break;
			case Key::Up:
				systemKey = KEY_UP;
				break;
			case Key::PageUp:
				systemKey = KEY_PAGEUP;
				break;
			case Key::PageDown:
				systemKey = KEY_PAGEDOWN;
				break;
			case Key::Home:
				systemKey = KEY_HOME;
				break;
			case Key::End:
				systemKey = KEY_END;
				break;
			case Key::CapsLock:
				systemKey = KEY_CAPSLOCK;
				break;
			case Key::ScrollLock:
				systemKey = KEY_SCROLLLOCK;
				break;
			case Key::NumLock:
				systemKey = KEY_NUMLOCK;
				break;
			case Key::PrintScreen:
				systemKey = KEY_PRINT;
				break;
			case Key::Pause:
				systemKey = KEY_PAUSE;
				break;
			case Key::F1:
				systemKey = KEY_F1;
				break;
			case Key::F2:
				systemKey = KEY_F2;
				break;
			case Key::F3:
				systemKey = KEY_F3;
				break;
			case Key::F4:
				systemKey = KEY_F4;
				break;
			case Key::F5:
				systemKey = KEY_F5;
				break;
			case Key::F6:
				systemKey = KEY_F6;
				break;
			case Key::F7:
				systemKey = KEY_F7;
				break;
			case Key::F8:
				systemKey = KEY_F8;
				break;
			case Key::F9:
				systemKey = KEY_F9;
				break;
			case Key::F10:
				systemKey = KEY_F10;
				break;
			case Key::F11:
				systemKey = KEY_F11;
				break;
			case Key::F12:
				systemKey = KEY_F12;
				break;
			case Key::F13:
				systemKey = KEY_F13;
				break;
			case Key::F14:
				systemKey = KEY_F14;
				break;
			case Key::F15:
				systemKey = KEY_F15;
				break;
			case Key::F16:
				systemKey = KEY_F16;
				break;
			case Key::F17:
				systemKey = KEY_F17;
				break;
			case Key::F18:
				systemKey = KEY_F18;
				break;
			case Key::F19:
				systemKey = KEY_F19;
				break;
			case Key::F20:
				systemKey = KEY_F20;
				break;
			case Key::F21:
				systemKey = KEY_F21;
				break;
			case Key::F22:
				systemKey = KEY_F22;
				break;
			case Key::F23:
				systemKey = KEY_F23;
				break;
			case Key::F24:
				systemKey = KEY_F24;
				break;
				//case Key::F25:
				//	systemKey = VK_F25;
				//	break;
			case Key::Kp0:
				systemKey = KEY_KP0;
				break;
			case Key::Kp1:
				systemKey = KEY_KP1;
				break;
			case Key::Kp2:
				systemKey = KEY_KP2;
				break;
			case Key::Kp3:
				systemKey = KEY_KP3;
				break;
			case Key::Kp4:
				systemKey = KEY_KP4;
				break;
			case Key::Kp5:
				systemKey = KEY_KP5;
				break;
			case Key::Kp6:
				systemKey = KEY_KP6;
				break;
			case Key::Kp7:
				systemKey = KEY_KP7;
				break;
			case Key::Kp8:
				systemKey = KEY_KP8;
				break;
			case Key::Kp9:
				systemKey = KEY_KP9;
				break;
			case Key::KpDecimal:
				systemKey = KEY_KPDOT;
				break;
			case Key::KpDivide:
				systemKey = KEY_KPSLASH;
				break;
			case Key::KpMultiply:
				systemKey = KEY_KPASTERISK;
				break;
			case Key::KpSubtract:
				systemKey = KEY_KPMINUS;
				break;
			case Key::KpAdd:
				systemKey = KEY_KPPLUS;
				break;
			case Key::KpEnter:
				systemKey = KEY_KPENTER;
				break;
				//case Key::KpEqual:
				//	systemKey = VK_KP_EQUAL;
				//	break;
			case Key::LeftShift:
				systemKey = KEY_LEFTSHIFT;
				break;
			case Key::LeftControl:
				systemKey = KEY_LEFTCTRL;
				break;
			case Key::LeftAlt:
				systemKey = KEY_LEFTALT;
				break;
			case Key::LeftSuper:
				systemKey = KEY_LEFTMETA;
				break;
			case Key::RightShift:
				systemKey = KEY_RIGHTSHIFT;
				break;
			case Key::RightControl:
				systemKey = KEY_LEFTCTRL; //separate key here?
				break;
			case Key::RightAlt:
				systemKey = KEY_RIGHTALT;
				break;
			case Key::RightSuper:
				systemKey = KEY_RIGHTMETA;
				break;
			case Key::Menu:
				systemKey = KEY_MENU;
				break;
#endif
			}
			return systemKey;
		}

		// See https://w3c.github.io/uievents-code/
		constexpr std::string_view glfw_to_w3c_keyboard_event_code(Key k)
		{
			switch(k) {
			case Key::Unknown:
				return "Unidentified";
			case Key::Space:
				return "Space";
			case Key::Apostrophe:
				return "Quote";
			case Key::Comma:
				return "Comma";
			case Key::Minus:
				return "Minus";
			case Key::Period:
				return "Period";
			case Key::Slash:
				return "Slash";
			case Key::N0:
				return "Digit0";
			case Key::N1:
				return "Digit1";
			case Key::N2:
				return "Digit2";
			case Key::N3:
				return "Digit3";
			case Key::N4:
				return "Digit4";
			case Key::N5:
				return "Digit5";
			case Key::N6:
				return "Digit6";
			case Key::N7:
				return "Digit7";
			case Key::N8:
				return "Digit8";
			case Key::N9:
				return "Digit9";
			case Key::Semicolon:
				return "Semicolon";
			case Key::Equal:
				return "Equal";
			case Key::A:
				return "KeyA";
			case Key::B:
				return "KeyB";
			case Key::C:
				return "KeyC";
			case Key::D:
				return "KeyD";
			case Key::E:
				return "KeyE";
			case Key::F:
				return "KeyF";
			case Key::G:
				return "KeyG";
			case Key::H:
				return "KeyH";
			case Key::I:
				return "KeyI";
			case Key::J:
				return "KeyJ";
			case Key::K:
				return "KeyK";
			case Key::L:
				return "KeyL";
			case Key::M:
				return "KeyM";
			case Key::N:
				return "KeyN";
			case Key::O:
				return "KeyO";
			case Key::P:
				return "KeyP";
			case Key::Q:
				return "KeyQ";
			case Key::R:
				return "KeyR";
			case Key::S:
				return "KeyS";
			case Key::T:
				return "KeyT";
			case Key::U:
				return "KeyU";
			case Key::V:
				return "KeyV";
			case Key::W:
				return "KeyW";
			case Key::X:
				return "KeyX";
			case Key::Y:
				return "KeyY";
			case Key::Z:
				return "KeyZ";
			case Key::LeftBracket:
				return "BracketLeft";
			case Key::Backslash:
				return "Backslash";
			case Key::RightBracket:
				return "BracketRight";
			case Key::GraveAccent:
				return "Backquote";
			case Key::World1:
			case Key::World2:
				return "Unidentified";

			case Key::Escape:
				return "Escape";
			case Key::Enter:
				return "Enter";
			case Key::Tab:
				return "Tab";
			case Key::Backspace:
				return "Backspace";
			case Key::Insert:
				return "Insert";
			case Key::Delete:
				return "Delete";
			case Key::Right:
				return "ArrowRight";
			case Key::Left:
				return "ArrowLeft";
			case Key::Down:
				return "ArrowDown";
			case Key::Up:
				return "ArrowUp";
			case Key::PageUp:
				return "PageUp";
			case Key::PageDown:
				return "PageDown";
			case Key::Home:
				return "Home";
			case Key::End:
				return "End";

			case Key::CapsLock:
				return "CapsLock";
			case Key::ScrollLock:
				return "ScrollLock";
			case Key::NumLock:
				return "NumLock";
			case Key::PrintScreen:
				return "PrintScreen";
			case Key::Pause:
				return "Pause";

			case Key::F1:
				return "F1";
			case Key::F2:
				return "F2";
			case Key::F3:
				return "F3";
			case Key::F4:
				return "F4";
			case Key::F5:
				return "F5";
			case Key::F6:
				return "F6";
			case Key::F7:
				return "F7";
			case Key::F8:
				return "F8";
			case Key::F9:
				return "F9";
			case Key::F10:
				return "F10";
			case Key::F11:
				return "F11";
			case Key::F12:
				return "F12";
			case Key::F13:
				return "F13";
			case Key::F14:
				return "F14";
			case Key::F15:
				return "F15";
			case Key::F16:
				return "F16";
			case Key::F17:
				return "F17";
			case Key::F18:
				return "F18";
			case Key::F19:
				return "F19";
			case Key::F20:
				return "F20";
			case Key::F21:
				return "F21";
			case Key::F22:
				return "F22";
			case Key::F23:
				return "F23";
			case Key::F24:
				return "F24";
			case Key::F25:
				return "F25";

			case Key::Kp0:
				return "Numpad0";
			case Key::Kp1:
				return "Numpad1";
			case Key::Kp2:
				return "Numpad2";
			case Key::Kp3:
				return "Numpad3";
			case Key::Kp4:
				return "Numpad4";
			case Key::Kp5:
				return "Numpad5";
			case Key::Kp6:
				return "Numpad6";
			case Key::Kp7:
				return "Numpad7";
			case Key::Kp8:
				return "Numpad8";
			case Key::Kp9:
				return "Numpad9";
			case Key::KpDecimal:
				return "NumpadDecimal";
			case Key::KpDivide:
				return "NumpadDivide";
			case Key::KpMultiply:
				return "NumpadMultiply";
			case Key::KpSubtract:
				return "NumpadSubtract";
			case Key::KpAdd:
				return "NumpadAdd";
			case Key::KpEnter:
				return "NumpadEnter";
			case Key::KpEqual:
				return "NumpadEqual";

			case Key::LeftShift:
				return "ShiftLeft";
			case Key::LeftControl:
				return "ControlLeft";
			case Key::LeftAlt:
				return "AltLeft";
			case Key::LeftSuper:
				return "MetaLeft";
			case Key::RightShift:
				return "ShiftRight";
			case Key::RightControl:
				return "ControlRight";
			case Key::RightAlt:
				return "AltRight";
			case Key::RightSuper:
				return "MetaRight";

			case Key::Menu:
				return "ContextMenu";
			}
			return "Unidentified";
		}

		// Based on US keyboard layout
		// See https://www.w3.org/TR/uievents-key/
		constexpr std::string_view glfw_to_w3c_keyboard_event_key(Key k)
		{
			switch(k) {
			case Key::Unknown:
				return "Unidentified";
			case Key::Space:
				return " ";
			case Key::Apostrophe:
				return "'";
			case Key::Comma:
				return ",";
			case Key::Minus:
				return "-";
			case Key::Period:
				return ".";
			case Key::Slash:
				return "/";
			case Key::N0:
				return "0";
			case Key::N1:
				return "1";
			case Key::N2:
				return "2";
			case Key::N3:
				return "3";
			case Key::N4:
				return "4";
			case Key::N5:
				return "5";
			case Key::N6:
				return "6";
			case Key::N7:
				return "7";
			case Key::N8:
				return "8";
			case Key::N9:
				return "9";
			case Key::Semicolon:
				return ";";
			case Key::Equal:
				return "=";
			case Key::A:
				return "a";
			case Key::B:
				return "b";
			case Key::C:
				return "c";
			case Key::D:
				return "d";
			case Key::E:
				return "e";
			case Key::F:
				return "f";
			case Key::G:
				return "g";
			case Key::H:
				return "h";
			case Key::I:
				return "i";
			case Key::J:
				return "j";
			case Key::K:
				return "k";
			case Key::L:
				return "l";
			case Key::M:
				return "m";
			case Key::N:
				return "n";
			case Key::O:
				return "o";
			case Key::P:
				return "p";
			case Key::Q:
				return "q";
			case Key::R:
				return "r";
			case Key::S:
				return "s";
			case Key::T:
				return "t";
			case Key::U:
				return "u";
			case Key::V:
				return "v";
			case Key::W:
				return "w";
			case Key::X:
				return "x";
			case Key::Y:
				return "y";
			case Key::Z:
				return "z";
			case Key::LeftBracket:
				return "[";
			case Key::Backslash:
				return "\\";
			case Key::RightBracket:
				return "]";
			case Key::GraveAccent:
				return "`";
			case Key::World1:
			case Key::World2:
				return "Unidentified";

			case Key::Escape:
				return "Escape";
			case Key::Enter:
				return "Enter";
			case Key::Tab:
				return "Tab";
			case Key::Backspace:
				return "Backspace";
			case Key::Insert:
				return "Insert";
			case Key::Delete:
				return "Delete";
			case Key::Right:
				return "ArrowRight";
			case Key::Left:
				return "ArrowLeft";
			case Key::Down:
				return "ArrowDown";
			case Key::Up:
				return "ArrowUp";
			case Key::PageUp:
				return "PageUp";
			case Key::PageDown:
				return "PageDown";
			case Key::Home:
				return "Home";
			case Key::End:
				return "End";

			case Key::CapsLock:
				return "CapsLock";
			case Key::ScrollLock:
				return "ScrollLock";
			case Key::NumLock:
				return "NumLock";
			case Key::PrintScreen:
				return "PrintScreen";
			case Key::Pause:
				return "Pause";

			case Key::F1:
				return "F1";
			case Key::F2:
				return "F2";
			case Key::F3:
				return "F3";
			case Key::F4:
				return "F4";
			case Key::F5:
				return "F5";
			case Key::F6:
				return "F6";
			case Key::F7:
				return "F7";
			case Key::F8:
				return "F8";
			case Key::F9:
				return "F9";
			case Key::F10:
				return "F10";
			case Key::F11:
				return "F11";
			case Key::F12:
				return "F12";
			case Key::F13:
				return "F13";
			case Key::F14:
				return "F14";
			case Key::F15:
				return "F15";
			case Key::F16:
				return "F16";
			case Key::F17:
				return "F17";
			case Key::F18:
				return "F18";
			case Key::F19:
				return "F19";
			case Key::F20:
				return "F20";
			case Key::F21:
				return "F21";
			case Key::F22:
				return "F22";
			case Key::F23:
				return "F23";
			case Key::F24:
				return "F24";
			case Key::F25:
				return "F25";

			case Key::Kp0:
				return "0";
			case Key::Kp1:
				return "1";
			case Key::Kp2:
				return "2";
			case Key::Kp3:
				return "3";
			case Key::Kp4:
				return "4";
			case Key::Kp5:
				return "5";
			case Key::Kp6:
				return "6";
			case Key::Kp7:
				return "7";
			case Key::Kp8:
				return "8";
			case Key::Kp9:
				return "9";
			case Key::KpDecimal:
				return ".";
			case Key::KpDivide:
				return "/";
			case Key::KpMultiply:
				return "*";
			case Key::KpSubtract:
				return "-";
			case Key::KpAdd:
				return "+";
			case Key::KpEnter:
				return "Enter";
			case Key::KpEqual:
				return "=";

			case Key::LeftShift:
			case Key::RightShift:
				return "Shift";
			case Key::LeftControl:
			case Key::RightControl:
				return "Control";
			case Key::LeftAlt:
			case Key::RightAlt:
				return "Alt";
			case Key::LeftSuper:
			case Key::RightSuper:
				return "Meta";

			case Key::Menu:
				return "ContextMenu";
			}
			return "Unidentified";
		}

		using namespace pragma::math::scoped_enum::bitwise;
	}

	REGISTER_ENUM_FLAGS(pragma::platform::Modifier)
}
